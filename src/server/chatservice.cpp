#include "chatservice.h"
#include "public.h"
// 构造函数
ChatService::ChatService()
{
    _msgHandlerMap.insert({LOG_IN, std::bind(&ChatService::login, this, _1, _2, _3)});
    _msgHandlerMap.insert({REG, std::bind(&ChatService::reg, this, _1, _2, _3)});
    _msgHandlerMap.insert({ONE_CHAT, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_FRIEND, std::bind(&ChatService::addFriend, this, _1, _2, _3)});
    _msgHandlerMap.insert({CREATE_GROUP, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_TO_GROUP, std::bind(&ChatService::addToGroup, this, _1, _2, _3)});
    _msgHandlerMap.insert({GROUP_CHAT, std::bind(&ChatService::groupChat, this, _1, _2, _3)});
    _msgHandlerMap.insert({LOG_OUT, std::bind(&ChatService::logout, this, _1, _2, _3)});

    // 连接redis
    if (_redisModel.connect())
    {
        _redisModel.init_notify_handler(std::bind(&ChatService::redisNotifyHandler, this, _1, _2));
    }
}
// 单例模式获取类对象的对外接口函数
ChatService *ChatService::getInstance()
{
    // 单例模式的饿汉模式：线程安全的
    static ChatService _service;
    return &_service;
}

// 暴露给chatserver的接口，用int整型匹配对应的处理函数，完成业务处理
msgHandler ChatService::getHandler(int msgid)
{
    if (_msgHandlerMap.count(msgid))
    {
        return _msgHandlerMap[msgid];
    }
    else
    {
        // 使用lamda表达式，返回一个 格式相同，但是只调用日志输出的函数。
        return [=](const TcpConnectionPtr &, Json &, Timestamp)
        { LOG_ERROR << "msgid: " << msgid << " can not find the msgHandler！"; };
    }
}

// 不同功能的业务处理函数
// 登录
void ChatService::login(const TcpConnectionPtr &conn, Json &js, Timestamp time)
{
    // 调用日志打印输出测试一下
    LOG_INFO << "logining...";
    /*
    登录分为四种情况：
        1. 找不到账号
        2. 账号正确，但是密码错误
        3. 账号正确，密码正确，未登录用户
        4. 账号正确，密码正确，已登录用户
    */
    int id = js[ID].get<int>();
    string password = js[PWD];
    // 能否找到id，并且匹配密码
    // 数据的增删改查的线程安全由mysql来保证
    User usr = _userModel.query(id);
    // 创建返回消息的JSON
    Json response; // 每个线程都有自己的栈，局部变量不会有线程安全问题

    response[MSG_ID] = LOG_IN_MSG_ACK;
    // 找不到账号
    if (usr.getId() == -1)
    {
        response[ERRNO] = 1;
        response[MSG] = "can not find the id!";
    }
    else if (usr.getId() == id)
    {
        // 找到了账号，但是密码不对
        if (usr.getPassword() != password)
        {
            response[ERRNO] = 2;
            response[MSG] = "incorrect password!";
        }
        // 账号和密码都正确，已经登录了
        else if (usr.getState() == "online")
        {
            response[ERRNO] = 3;
            response[MSG] = "already online! can not login again";
        }
        // 账号密码正确，未登录
        else
        {
            // 登录成功：
            response[ERRNO] = 0;
            response[ID] = usr.getId();
            response[NAME] = usr.getName();
            // 更新mysql数据库内用户的登录状态
            usr.setState("online");
            // 并发的修改数据库，线程安全不用我们担心
            _userModel.updateState(usr);
            // 并发访问id<-->tcpconnectionPtr映射表
            {
                std::unique_lock<std::mutex> lk(_connMtx); // 上锁
                // 添加连接
                _usrConnMap.insert({usr.getId(), conn});
            }
            // 拉取离线消息
            std::vector<std::string> offmsg;
            if (_offMsgModel.query(usr.getId(), offmsg))
            {
                _offMsgModel.remove(usr.getId());
            }
            if (!offmsg.empty())
            {
                response[OFF_MSG] = offmsg;
            }
            // 显示自己的好友
            std::vector<std::string> friends;
            _friendModel.query(usr.getId(), friends);
            if (!friends.empty())
            {
                response[FRIEND_LIST] = friends;
            }
            // 拉取群
            std::vector<Group> groupVec;
            _groupModel.queryAllGroup(usr.getId(), groupVec);
            if (!groupVec.empty())
            {
                std::vector<std::string> groupInfos;
                for (Group &gp : groupVec)
                {
                    Json oneGroupInfo;
                    oneGroupInfo[GROUP_ID] = gp.getID();
                    oneGroupInfo[GROUP_NAME] = gp.getGName();
                    oneGroupInfo[GROUP_DESC] = gp.getGDesc();
                    std::vector<std::string> groupMem;
                    for (GroupUser &mem : gp.getUsers())
                    {
                        Json oneMemInfo;
                        oneMemInfo[ID] = mem.getId();
                        oneMemInfo[NAME] = mem.getName();
                        oneMemInfo[STATE] = mem.getState();
                        oneMemInfo[GROUP_ROLE] = mem.getRole();
                        groupMem.push_back(oneMemInfo.dump());
                    }
                    oneGroupInfo[GROUP_MEMS] = groupMem;
                    groupInfos.push_back(oneGroupInfo.dump());
                }
                response[GROUP_LIST] = groupInfos;
            }
            // 在redis中要订阅频道-->自身的id。如果别人向我这个id的频道发了消息，我需要被提醒然后感知到消息
            _redisModel.subscribe(usr.getId());
            /*
            服务器推送
            当客户端登录成功后，应该告知当前用户的所有好友，“我已上线”
            */
            // 访问好友列表
            // 要访问_usrConnMap，需要上锁
            std::string tt = getCurrentTime();
            {
                //上锁
                std::unique_lock<std::mutex> lk(_connMtx);
                for (auto &f : friends)
                {
                    // 正好利用上面代码得到的好友列表
                    Json fInfo = Json::parse(f);
                    int id = fInfo[ID].get<int>();
                    // 推送消息
                    Json pushMsg;
                    pushMsg[MSG_ID] = FRIEND_ONLINE;
                    pushMsg[ID] = usr.getId();
                    pushMsg[TIME] = tt;
                    if (_usrConnMap.count(id))
                    {
                        _usrConnMap[id]->send(pushMsg.dump());
                    }
                    // 此服务器没有这个好友的sokcet，但是需要判断是否在其他服务器
                    else if (_userModel.query(id).getState() == "online")
                    {
                        // 用户在线，在其他服务器上，将消息发布在消息队列上
                        _redisModel.publish(id, pushMsg.dump());
                    }
                }
            }
        }
    }
    // 向客户端发送登录状态
    conn->send(response.dump() + "\n");
}
// 注册
// 注册需要：name 和 password
void ChatService::reg(const TcpConnectionPtr &conn, Json &js, Timestamp time)
{
    LOG_INFO << "reging...";
    string name = js[NAME];
    string password = js[PWD];

    User usr;
    usr.setName(name);
    usr.setPassword(password);
    bool state = _userModel.insert(usr);
    // 创建返回消息的JSON
    Json response;
    response[MSG_ID] = REG_MSG_ACK;
    if (state)
    {
        // 注册成功，向客户端通报注册成功消息
        response[ERRNO] = 0;
        response[ID] = usr.getId();
    }
    else
    {
        // 注册失败，向客户端通报注册失败消息
        response[ERRNO] = -1;
    }
    conn->send(response.dump() + "\n");
}
// 处理客户端异常退出
void ChatService::clientCloseException(const TcpConnectionPtr &conn)
{
    /*
    客户端异常退出，服务器不会接收到退出的json（业务层面无法做到对客户端异常退出的检测），但是断开连接会被
    网络I/O层感知到并在connection函数中得到反馈
    再一次需要网络层和业务层相互配合完成任务
    退出的客户端，在服务器需要完成如下操作：
        - _usrConnMap需要删除对应的连接和id
        - 数据库要修改state状态为offline
    */
    User usr;
    // 虽然这里只涉及读取，但是不能保证其他线程不会对该哈希表产生写入修改操作
    {
        std::unique_lock<std::mutex> lk(_connMtx); // 上锁
        for (auto &it : _usrConnMap)
        {
            if (it.second == conn)
            {
                usr.setId(it.first);
                break;
            }
        }
    }
    if (usr.getId() != -1)
    {
        // 删除操作，上锁
        {
            std::unique_lock<std::mutex> lk(_connMtx);
            _usrConnMap.erase(usr.getId());
        }
        // 修改数据库
        _userModel.updateState(usr);
    }
}
// 处理服务器异常宕机
void ChatService::serverCloseException()
{
    _userModel.offlineAll();
}

// 一对一的聊天业务
void ChatService::oneChat(const TcpConnectionPtr &conn, Json &js, Timestamp)
{
    /*
    一对一聊天，消息格式：
    {
        MSG_ID: ONE_CHAT,
        ID: the sender,
        FRIEND_ID: the reciver,
        MSG: the chat message
    }
    */
    /*
                js           js
    消息发送方 -----> 服务器 -----> 消息接收方
     */
    int destID = js[FRIEND_ID].get<int>();
    // 由destID找到对应的socket连接
    {
        std::unique_lock<std::mutex> lk(_connMtx); // 上锁
        auto iter = _usrConnMap.find(destID);
        if (iter != _usrConnMap.end())
        {
            LOG_INFO << "接收方在线";
            iter->second->send(js.dump());
            return;
        }
    }
    // 也许在其他服务器上
    if (_userModel.query(destID).getState() == "online")
    {
        _redisModel.publish(destID, js.dump());
    }
    else
    {
        _offMsgModel.insert(destID, js.dump());
        LOG_INFO << "离线消息保存完毕";
    }
}

// 添加好友的业务
void ChatService::addFriend(const TcpConnectionPtr &conn, Json &js, Timestamp)
{
    // 获取自身的id和好友的id
    int id = js[ID].get<int>();
    int friendid = js[FRIEND_ID].get<int>();

    // 插入
    Json response;
    response[MSG_ID] = ADD_FRIEND_ACK;
    response[TIME] = getCurrentTime();
    // 调用fmodel去修改friend表，如果fid不合法，是不会修改的
    if (_friendModel.insert(id, friendid))
    {
        response[ERRNO] = 0;
        response[MSG] = "add friend successed!";
        // 还需要返回添加的好友的user信息
        User friendInfo = _userModel.query(friendid);
        response[ID] = friendInfo.getId();
        response[NAME] = friendInfo.getName();
        response[STATE] = friendInfo.getState();
    }
    else
    {
        response[ERRNO] = -1;
        response[MSG] = "add friend failed!";
    }
    conn->send(response.dump());
}

// 创建群组
void ChatService::createGroup(const TcpConnectionPtr &conn, Json &js, Timestamp)
{
    int usrID = js[ID].get<int>();
    std::string gname = js[GROUP_NAME];
    std::string gdesc = js[GROUP_DESC];
    Group group(-1, gname, gdesc);

    Json response;
    response[MSG_ID] = CREATE_GROUP_ACK;
    response[TIME] = getCurrentTime();
    if (_groupModel.creatGroup(group))
    {
        _groupModel.addUser(usrID, group.getID(), "creator"); // 将 用户 和 组 绑定在一起
        response[GROUP_ID] = group.getID();
        response[GROUP_NAME] = gname;
        response[GROUP_DESC] = gdesc;
        response[MSG] = "create group success!";
        response[ERRNO] = 0;
    }
    else
    {
        response[MSG] = "create group failed!";
        response[ERRNO] = -1;
    }
    conn->send(response.dump());
}
// 添加用户至群组
void ChatService::addToGroup(const TcpConnectionPtr &conn, Json &js, Timestamp)
{
    int usrID = js[ID].get<int>();
    int groupID = js[GROUP_ID].get<int>();
    // 检查有没有这个群组
    Json response;
    response[MSG_ID] = ADD_TO_GROUP_ACK;
    response[TIME] = getCurrentTime();
    if (_groupModel.addUser(usrID, groupID, "normal"))
    {
        response[GROUP_ID] = groupID;
        response[GROUP_NAME] = "unknown";
        response[GROUP_DESC] = "unknown";
        response[MSG] = "add to group success!";
        response[ERRNO] = 0;
        // 获取这个群的完整信息
        std::vector<GroupUser> tmp;
        _groupModel.queryOneGroup(usrID, groupID, tmp);
        std::vector<std::string> members;
        for (auto &it : tmp)
        {
            Json mem;
            mem[ID] = it.getId();
            mem[NAME] = it.getName();
            mem[STATE] = it.getState();
            mem[GROUP_ROLE] = it.getRole();
            members.push_back(mem.dump());
        }
        response[GROUP_MEMS] = members;
    }
    else
    {
        response[MSG] = "add to group failed!";
        response[ERRNO] = -1;
    }
    conn->send(response.dump());
}
// 群聊业务
void ChatService::groupChat(const TcpConnectionPtr &conn, Json &js, Timestamp)
{
    int usrID = js[ID].get<int>();
    int groupID = js[GROUP_ID].get<int>();
    std::string msg = js[MSG];
    // 如果usr不是群组成员呢？这部分客户端自检
    std::vector<int> groupMem;
    if (_groupModel.queryOneGroup(usrID, groupID, groupMem))
    {
        std::unique_lock<std::mutex> lk(_connMtx);
        for (int memid : groupMem)
        {
            if (_usrConnMap.count(memid))
            {
                // 在线
                _usrConnMap[memid]->send(js.dump());
            }
            else if (_userModel.query(memid).getState() == "online")
            {
                // 不在当前服务器上
                _redisModel.publish(memid, js.dump());
            }
            else
            {
                // 离线
                _offMsgModel.insert(memid, js.dump());
            }
        }
    }
}

// 注销退出
void ChatService::logout(const TcpConnectionPtr &conn, Json &js, Timestamp)
{
    User usr;
    usr.setId(js[ID].get<int>());
    usr.setState("offline");
    // 删除操作，上锁
    {
        std::unique_lock<std::mutex> lk(_connMtx);
        _usrConnMap.erase(usr.getId());
    }
    // 修改数据库
    _userModel.updateState(usr);
    // redis取消订阅
    _redisModel.unsubscribe(usr.getId());
    // 服务器向该用户的所有好友推送，此用户已下线
    std::vector<std::string> fVec;
    _friendModel.query(usr.getId(), fVec);
    if (!fVec.empty())
    {
        {
            std::unique_lock<std::mutex> lk(_connMtx);
            std::string tt = getCurrentTime();
            for (string &str : fVec)
            {
                Json fInfo = Json::parse(str);
                int id = fInfo[ID].get<int>();
                // 通知的下线消息
                Json pushMsg;
                pushMsg[MSG_ID] = FRIEND_OFFLINE;
                pushMsg[ID] = usr.getId();
                pushMsg[TIME] = tt;
                if (_usrConnMap.count(id))
                {
                    _usrConnMap[id]->send(pushMsg.dump());
                }
                else if (_userModel.query(id).getState() == "online")
                {
                    _redisModel.publish(id, pushMsg.dump());
                }
            }
        }
    }
}

// 订阅频道有更新，获取消息
void ChatService::redisNotifyHandler(int id, std::string msg)
{
    {
        std::unique_lock<std::mutex> lk(_connMtx);
        auto iter = _usrConnMap.find(id);
        if(iter != _usrConnMap.end())
        {
            iter->second->send(msg);
        }
    }
}