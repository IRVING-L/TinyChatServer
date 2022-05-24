#include "json.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <atomic>
// Linux的网络通信所需库函数
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "public.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
// 自定义的一些类所在的头文件
#include "group.h"
#include "user.h"
using Json = nlohmann::json;
using myClock = std::chrono::system_clock;

// 记录当前系统登录的用户信息
User g_currentUser;
// 记录当前登录用户的好友列表信息
std::vector<User> g_currentUserFriendList;
// 记录当前登录用户的群组列表信息
std::vector<Group> g_currentUserGroupList;
// 记录当前登录用户的离线消息
std::vector<std::string> g_currentUserOffMsg;

// 控制主菜单页面程序
bool isMainMenuRunning = false;

// 用于读写线程之间的通信
sem_t rwsem;
// 记录登录状态
std::atomic_bool g_isLoginSuccess{false};

// 接收线程
void readTaskHandler(int clientfd);

// 主聊天页面程序
void mainMenu(int);
// 显示当前登录成功用户的基本信息
void showCurrentUserData();
// 处理注册的响应消息函数
void doRegResponse(Json &js);
// 处理登录的响应消息函数
void doLoginResponse(Json &js);
// 命令函数的声明
void help(int a = 0, std::string s = "");
void chat(int, std::string);
void addfriend(int, std::string);
void creategroup(int, std::string);
void addgroup(int, std::string);
void groupchat(int, std::string);
void logout(int, std::string);
void showFriendList(int a = 0, std::string s = "");
void showGroupList(int a = 0, std::string s = "");
void showOffMsgList(int a = 0, std::string s = "");

// 获取当前用户的好友列表
void getFriendList(Json &response, std::vector<User> &fredList);
// 获取当前用户的群组列表
void getGroupList(Json &response, std::vector<Group> &groupList);
// 获取当前用户的离线消息列表
void getOfflineMsgList(Json &response, std::vector<std::string> &offMsgList);

// 系统支持的客户端命令列表
std::unordered_map<std::string, std::string> commandMap = {
    {"help", "显示所有支持的命令，格式: [help]"},
    {"chat", "一对一聊天，格式: [chat:friendid:message]"},
    {"addfriend", "添加好友，格式: [addfriend:friendid]"},
    {"creategroup", "创建群组，格式: [creategroup:groupname:groupdesc]"},
    {"addgroup", "加入群组，格式: [addgroup:groupid]"},
    {"groupchat", "群聊，格式: [groupchat:groupid:message]"},
    {"logout", "注销，格式: [logout]"},
    {"s-f", "[显示好友列表]"},
    {"s-g", "[显示群组列表]"},
    {"s-off", "[显示离线消息列表]"},
};

// 注册系统支持的客户端命令处理
std::unordered_map<std::string, std::function<void(int, std::string)>> commandHandlerMap = {
    {"help", help},
    {"chat", chat},
    {"addfriend", addfriend},
    {"creategroup", creategroup},
    {"addgroup", addgroup},
    {"groupchat", groupchat},
    {"logout", logout},
    {"s-f", showFriendList},
    {"s-g", showGroupList},
    {"s-off", showOffMsgList}};

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        std::cerr << "command invalid! example: ./client 127.0.0.1 6000" << std::endl;
        exit(-1); // 退出程序
    }
    // 获取IP地址和端口号
    const char *IP = argv[1];
    const uint16_t PORT = atoi(argv[2]);
    // 下面是客户端使用原生的linux进行socket通信

    int clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientFd)
    {
        std::cerr << "socket create error" << std::endl;
        exit(-1);
    }
    // 填写要访问的服务器的结构体
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &serverAddr.sin_addr.s_addr);
    // 客户端主动发起连接
    int ret = connect(clientFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (-1 == ret)
    {
        std::cerr << "connect server error" << std::endl;
        close(clientFd);
        exit(-1);
    }
    // 初始化读写线程通信用的信号量
    sem_init(&rwsem, 0, 0);
    // 连接服务器成功，启动接收子线程
    std::thread readTask(readTaskHandler, clientFd); // pthread_create
    readTask.detach();                               // 设置线程分离，让操作系统自动回收资源

    // 死循环
    while (true)
    {
        // 显示首页面菜单 登录、注册、退出
        std::cout << "========================" << std::endl;
        std::cout << "1. login" << std::endl;
        std::cout << "2. register" << std::endl;
        std::cout << "3. quit" << std::endl;
        std::cout << "========================" << std::endl;
        std::cout << "choice:";
        int choice = 0;
        std::cin >> choice;
        std::cin.get(); // 读掉缓冲区残留的回车
        switch (choice)
        {
        case 1:
        {
            /* 登录 */
            std::cout << "id:" << std::endl;
            int id;
            std::cin >> id;
            std::cout << "password:" << std::endl;
            std::string pwd;
            std::cin >> pwd;
            Json request;
            request[MSG_ID] = LOG_IN;
            request[ID] = id;
            request[PWD] = pwd;
            std::string message = request.dump();
            int ret = send(clientFd, message.c_str(), strlen(message.c_str()) + 1, 0);
            if (-1 == ret)
            {
                std::cerr << "send login msg error:" << message << std::endl;
            }

            sem_wait(&rwsem);

            if (g_isLoginSuccess)
            {
                isMainMenuRunning = true;
                mainMenu(clientFd);
            }
            break;
        }
        case 2:
        {
            /* 注册 */
            std::cout << "name:" << std::endl;
            std::string name;
            std::cin >> name;
            std::cout << "password:" << std::endl;
            std::string pwd;
            std::cin >> pwd;
            Json request;
            request[MSG_ID] = REG;
            request[NAME] = name;
            request[PWD] = pwd;

            std::string message = request.dump();
            int ret = send(clientFd, message.c_str(), strlen(message.c_str()) + 1, 0);
            if (-1 == ret)
            {
                std::cerr << "send reg msg error:" << message << std::endl;
            }
            sem_wait(&rwsem);

            break;
        }
        case 3:
            std::cout << "bye-bye" << std::endl;
            close(clientFd);
            sem_destroy(&rwsem);
            exit(0);
            break;

        default:
            std::cout << "invalid command, try again!" << std::endl;
            break;
        }
    }
    return 0;
}

// 接收线程
void readTaskHandler(int clientfd)
{
    while (true)
    {
        char buffer[8192] = {0};
        int len = recv(clientfd, buffer, sizeof(buffer), 0); // 阻塞了
        if (-1 == len || 0 == len)
        {
            close(clientfd);
            exit(-1);
        }
        /*
        子线程接收来自服务器的消息
        服务器向客户端发送的JSON中，都带有一个msgid
        通过这个msgid，客户端可以用于判断此次接收到的JSON的内容
        */
        Json response = Json::parse(buffer);
        int msgType = response[MSG_ID].get<int>();
        switch (msgType)
        {
        case REG_MSG_ACK:
        {
            doRegResponse(response);
            // 增加资源
            sem_post(&rwsem);
            break;
        }
        case LOG_IN_MSG_ACK:
        {
            doLoginResponse(response);
            // 增加资源
            sem_post(&rwsem);
            break;
        }
        case ONE_CHAT:
        {
            // 接收到了其他客户端发送来的聊天消息
            std::cout << "one chat >>>\t"
                      << "time: " << response[TIME] << std::endl
                      << "user:[" << response[ID] << "]"
                      << " ---> "
                      << "message: " << response[MSG] << std::endl;
            break;
        }
        case FRIEND_ONLINE:
        {
            // 朋友上线了
            std::cout << "friend online >>>\t"
                      << "time: " << response[TIME] << "\t"
                      << "friend[" << response[ID] << "]" << std::endl;
            int friendID = response[ID].get<int>();
            for (auto &iter : g_currentUserFriendList)
            {
                if (iter.getId() == friendID)
                {
                    iter.setState("online");
                }
            }
            break;
        }
        case FRIEND_OFFLINE:
        {
            // 朋友下线了
            std::cout << "friend offline >>>\t"
                      << "time: " << response[TIME] << "\t"
                      << "friend[" << response[ID] << "]" << std::endl;
            int friendID = response[ID].get<int>();
            for (auto &iter : g_currentUserFriendList)
            {
                if (iter.getId() == friendID)
                {
                    iter.setState("offline");
                }
            }
            break;
        }
        case ADD_FRIEND_ACK:
        {
            std::cout << "add friend >>>\t"
                      << "time: " << response[TIME]
                      << "\tinfo:" << response[MSG] << std::endl;
            int isSuccess = response[ERRNO].get<int>();
            if (isSuccess == 0)
            {
                // 添加好友成功，将好友加入到现有的好友列表中
                User frd;
                frd.setId(response[ID].get<int>());
                frd.setName(response[NAME]);
                frd.setState(response[STATE]);
                g_currentUserFriendList.push_back(frd);
            }
            break;
        }
        case CREATE_GROUP_ACK:
        {
            // 创建群组
            std::cout << "create group >>>\t"
                      << "time: " << response[TIME]
                      << "\tinfo:" << response[MSG] << std::endl;
            if (response[ERRNO].get<int>() == 0)
            {
                // 将新建的群组，添加到当前的群组列表中区
                Group newgroup;
                newgroup.setID(response[GROUP_ID].get<int>());
                newgroup.setGName(response[GROUP_NAME]);
                newgroup.setGDesc(response[GROUP_DESC]);
                GroupUser gu;
                gu.setId(g_currentUser.getId());
                gu.setName(g_currentUser.getName());
                gu.setState(g_currentUser.getState());
                gu.setRole("creator");
                newgroup.getUsers().push_back(gu);
                g_currentUserGroupList.push_back(newgroup);
            }
            break;
        }
        case ADD_TO_GROUP_ACK:
        {
            // 添加到群组
            std::cout << "create group >>>\t"
                      << "time: " << response[TIME]
                      << "\tinfo:" << response[MSG] << std::endl;
            // 更新当前用户的群组列表
            if (response[ERRNO].get<int>() == 0)
            {
                // 把新加入的群，更新在当前用户的群组列表中
                Group group;
                group.setID(response[GROUP_ID].get<int>());
                group.setGName(response[GROUP_NAME]);
                group.setGDesc(response[GROUP_DESC]);
                std::vector<std::string> mems;
                for (std::string &str : mems)
                {
                    Json guJs = Json::parse(str);
                    GroupUser gu;
                    gu.setId(guJs[ID].get<int>());
                    gu.setName(guJs[NAME]);
                    gu.setState(guJs[STATE]);
                    gu.setRole(guJs[GROUP_ROLE]);
                    group.getUsers().push_back(gu);
                }
                g_currentUserGroupList.push_back(group);
            }
            break;
        }
        case GROUP_CHAT:
        {
            std::cout << "group chat >>>\t"
                      << "time: " << response[TIME] << std::endl
                      << "group:[" << response[GROUP_ID] << "]"
                      << "-"
                      << "user:[" << response[ID] << "]"
                      << " ---> "
                      << "message: " << response[MSG] << std::endl;
            break;
        }
        default:
            std::cout << "Oops... unexcept event!" << std::endl;
            break;
        }
    }
}

// 处理注册的响应消息函数
void doRegResponse(Json &js)
{
    if (-1 == js["errno"].get<int>())
    {
        std::cout << "register failed!" << std::endl;
        return;
    }
    // 注册成功
    std::cout << "name register success, userid is [" << js[ID] << "], do not forget it!" << std::endl;
}

// 处理登录的响应消息函数
void doLoginResponse(Json &response)
{
    int state = response[ERRNO].get<int>();
    if (0 != state)
    {
        // 登录失败
        std::cerr << "login failed! " << response[MSG] << std::endl;
        g_isLoginSuccess = false;
        return;
    }
    // 登录成功
    g_isLoginSuccess = true;
    g_currentUser.setId(response[ID].get<int>());
    g_currentUser.setName(response[NAME]);

    // 拉取好友列表、群组列表、离线消息
    g_currentUserFriendList.clear(); // 清除上一个用户保存的信息
    g_currentUserGroupList.clear();
    g_currentUserOffMsg.clear();

    getFriendList(response, g_currentUserFriendList);
    // 拉取群组列表
    getGroupList(response, g_currentUserGroupList);
    // 拉取离线消息
    getOfflineMsgList(response, g_currentUserOffMsg);

    showOffMsgList();
}

// 主聊天页面程序
void mainMenu(int clientFd)
{
    help(); // 调用该函数，显示可以执行的命令
    // 获取命令
    std::string command;
    while (isMainMenuRunning)
    {

        // showCurrentUserData(); // 调用该函数，显示用户的好友、群组信息
        std::cin >> command;
        if (command == "quit" || command == "exit")
            break;
        else if (commandMap.count(command))
        {
            auto func = commandHandlerMap.find(command)->second;
            // 执行相应功能的函数
            func(clientFd, command);
        }
        else
        {
            std::cout << "invalid command" << std::endl;
        }
    }
}
// 显示当前登录成功用户的基本信息
void showCurrentUserData()
{
    // 显示好友列表
    std::cout << "friend list >>>" << std::endl;
    for (auto &iter : g_currentUserFriendList)
    {
        std::cout << "ID: " << iter.getId()
                  << "\tName: " << iter.getName()
                  << "\tState: " << iter.getState()
                  << std::endl;
    }

    // 显示群组列表
    std::cout << "group list >>>" << std::endl;
    for (auto &iter : g_currentUserGroupList)
    {
        std::cout << "group ID: " << iter.getID()
                  << "\tgroup name: " << iter.getGName()
                  << "\tgroup desc: " << iter.getGDesc() << std::endl;
        std::cout << "group members: " << std::endl;
        auto &members = iter.getUsers();
        for (auto &m : members)
        {
            std::cout << "\tID: " << m.getId()
                      << "\tName: " << m.getName()
                      << "\tState: " << m.getState()
                      << "\tRole: " << m.getRole()
                      << std::endl;
        }
    }

    // 离线消息？
    std::cout << "offline message >>>" << std::endl;
    for (auto &iter : g_currentUserOffMsg)
    {
        Json offmsg = Json::parse(iter);
        std::cout << "message: " << offmsg["body"] << " from ID: " << offmsg["sourceID"] << std::endl;
    }
}

// 命令函数
void help(int, std::string)
{
    // 提供给用户执行的命令
    std::cout << "show command list >>> " << std::endl;
    for (auto &p : commandMap)
    {
        std::cout << p.first << " :\t " << p.second << std::endl;
    }
    std::cout << std::endl;
}
void chat(int clientFd, std::string)
{
    std::cout << "chat..." << std::endl;
    // 好友列表展示
    std::cout << "show friend list >>> " << std::endl;
    // 这里创建一个哈希表用来检查后面输入的id是否存在
    std::unordered_set<int> idCheck;
    for (auto &usr : g_currentUserFriendList)
    {
        idCheck.insert(usr.getId());
        std::cout << "UserID:" << usr.getId()
                  << "\t\tUserName:" << usr.getName()
                  << "\t\tUserState:" << usr.getState() << std::endl;
    }

    int friendID = -1;
    std::cout << "input ID u want chat: " << std::endl;
    std::cin >> friendID;
    if (!idCheck.count(friendID))
    {
        std::cout << "invalid id!" << std::endl;
        return;
    }
    // 输入要chat的好友id和聊天信息
    Json request;
    request[MSG_ID] = ONE_CHAT;
    request[ID] = g_currentUser.getId();
    request[FRIEND_ID] = friendID;
    // 输出要聊天的消息，长度小于1024个字符
    std::cout << "input chat message! less than 1024 Bytes" << std::endl;
    char str[1024] = {0};
    std::cin.get(); // 吃掉回车的换行符
    std::cin.getline(str, sizeof(str));
    request[MSG] = std::string(str);
    request[TIME] = getCurrentTime();

    // 将这个json发送到服务器上，服务器执行onechat回调函数，会将消息发送给对应的客户端
    std::string message = request.dump();
    int len = send(clientFd, message.c_str(), strlen(message.c_str()) + 1, 0);
    if (-1 == len)
    {
        // 调用send函数发送到服务器【失败】
        std::cerr << "send to server failed!" << std::endl;
        return;
    }
}
void addfriend(int clientFd, std::string)
{
    std::cout << "addfriend..." << std::endl;
    std::cout << "input ID u want to add:" << std::endl;
    int fid = -1;
    std::cin >> fid;
    std::cin.get(); // 吃掉回车的换行符
    Json request;
    request[MSG_ID] = ADD_FRIEND;
    request[ID] = g_currentUser.getId();
    request[FRIEND_ID] = fid;
    std::string message = request.dump();
    /*
    添加好友会有如下两类unexpected event
        - 1. 输入的好友id【不存在】：这个需要服务器拉取user表查看id是否合法
        - 2. 输入的好友id【已经是好友】。这个在客户端中去检查
    */
    // 检查好友是否已经添加上了
    for (auto &iter : g_currentUserFriendList)
    {
        if (iter.getId() == fid)
        {
            // 查到添加上了
            std::cout << "add friend failed! already friend!" << std::endl;
            return;
        }
    }
    int len = send(clientFd, message.c_str(), strlen(message.c_str()) + 1, 0);
    if (-1 == len)
    {
        std::cerr << "send to server failed!" << std::endl;
        return;
    }
}
void creategroup(int clientFd, std::string)
{
    std::cout << "creategroup..." << std::endl;
    Json request;
    request[MSG_ID] = CREATE_GROUP;
    request[ID] = g_currentUser.getId();
    std::cout << "input group name: " << std::endl;
    std::string name;
    std::cin >> name;
    request[GROUP_NAME] = name;
    std::cout << "input group desc: " << std::endl;
    std::string desc;
    std::cin.get(); // 吃掉回车换行
    std::cin >> desc;
    request[GROUP_DESC] = desc;

    std::string message = request.dump();
    int len = send(clientFd, message.c_str(), strlen(message.c_str()) + 1, 0);
    if (-1 == len)
    {
        std::cerr << "send to server failed!" << std::endl;
        return;
    }
}
void addgroup(int clientFd, std::string)
{
    std::cout << "addgroup..." << std::endl;
    Json request;
    request[MSG_ID] = ADD_TO_GROUP;
    std::cout << "input group id: " << std::endl;
    int gid;
    std::cin >> gid;
    request[ID] = g_currentUser.getId();
    request[GROUP_ID] = gid;
    /*
    添加到群组中，也会有两类unexpected event
        - 1. 输入的群组id【不合法】。这只能由服务器帮我们检查合法性
        - 2. 已经在群组里了。客户端自己就可以检查
    */
    for (auto &iter : g_currentUserGroupList)
    {
        if (iter.getID() == gid)
        {
            std::cout << "add to group failed! you already in this group!" << std::endl;
            return;
        }
    }
    std::string message = request.dump();
    int len = send(clientFd, message.c_str(), strlen(message.c_str()) + 1, 0);
    if (-1 == len)
    {
        std::cerr << "send to server failed!" << std::endl;
        return;
    }
}
void groupchat(int clientFd, std::string)
{
    std::cout << "groupchat..." << std::endl;
    Json request;
    request[MSG_ID] = GROUP_CHAT;
    std::cout << "input group id: " << std::endl;
    int gid;
    std::cin >> gid;
    request[ID] = g_currentUser.getId();
    request[GROUP_ID] = gid;
    std::cout << "input the message you want to say: " << std::endl;
    char chatmsg[1024];
    std::cin.get(); // 吃掉回车产生的换行符
    std::cin.getline(chatmsg, sizeof(chatmsg));
    request[MSG] = chatmsg;
    request[TIME] = getCurrentTime();
    // 判断输入的群号是否为自己的群
    for (auto &iter : g_currentUserGroupList)
    {
        if (iter.getID() == request[GROUP_ID])
        {
            std::string message = request.dump();
            int len = send(clientFd, message.c_str(), strlen(message.c_str()) + 1, 0);
            if (-1 == len)
            {
                std::cerr << "send to server failed!" << std::endl;
            }
            return;
        }
    }
    std::cerr << " U do not belong to this group!" << std::endl;
    return;
}
void logout(int clientFd, std::string)
{
    std::cout << "logout..." << std::endl;
    // 客户端下线
    Json request;
    request[MSG_ID] = LOG_OUT;
    request[ID] = g_currentUser.getId();
    std::string message = request.dump();
    int len = send(clientFd, message.c_str(), strlen(message.c_str()) + 1, 0);
    if (-1 == len)
    {
        std::cout << "send to server failed!" << std::endl;
        return;
    }
    isMainMenuRunning = false;
}

// 获取当前用户的好友列表
void getFriendList(Json &response, std::vector<User> &fredList)
{
    if (response.contains(FRIEND_LIST))
    {

        std::vector<std::string> friends = response[FRIEND_LIST];
        for (std::string &str : friends)
        {
            Json info = Json::parse(str);
            User usr;
            usr.setId(info[ID].get<int>());
            usr.setName(info[NAME]);
            usr.setState(info[STATE]);
            fredList.push_back(usr);
        }
    }
}
// 获取当前用户的群组列表
void getGroupList(Json &response, std::vector<Group> &groupList)
{
    if (response.contains(GROUP_LIST))
    {
        /*
        response["groups"] = {string, string...}
        string = oneGroupinfo.dump = {gid, name, desc, {usr...}}
        两次的序列化和反序列化
        */
        std::vector<std::string> groups = response[GROUP_LIST];
        for (std::string &str : groups)
        {
            Json info = Json::parse(str);
            Group oneGroup;
            oneGroup.setID(info[GROUP_ID]);
            oneGroup.setGName(info[GROUP_NAME]);
            oneGroup.setGDesc(info[GROUP_DESC]);
            std::vector<std::string> groupMem = info[GROUP_MEMS];
            for (std::string &mem : groupMem)
            {
                Json m = Json::parse(mem);
                GroupUser gusr;
                gusr.setId(m[ID]);
                gusr.setName(m[NAME]);
                gusr.setState(m[STATE]);
                gusr.setRole(m[GROUP_ROLE]);
                oneGroup.getUsers().push_back(gusr);
            }
            groupList.push_back(oneGroup);
        }
    }
}
// 获取当前用户的离线消息列表
void getOfflineMsgList(Json &response, std::vector<std::string> &offMsgList)
{
    if (response.contains(OFF_MSG))
    {
        std::vector<std::string> vec = (response[OFF_MSG]);
        offMsgList = vec;
    }
}

void showFriendList(int a, std::string s)
{
    // 显示好友列表
    std::cout << "**********<< friend list >>**********" << std::endl;
    for (auto &iter : g_currentUserFriendList)
    {
        std::cout << "ID: " << iter.getId()
                  << "\tName: " << iter.getName()
                  << "\tState: " << iter.getState()
                  << std::endl;
    }
}
void showGroupList(int a, std::string s)
{
    // 显示群组列表
    std::cout << "**********<< group list >>**********" << std::endl;
    for (auto &iter : g_currentUserGroupList)
    {
        std::cout << "group ID: " << iter.getID()
                  << "\tgroup name: " << iter.getGName()
                  << "\tgroup desc: " << iter.getGDesc() << std::endl;
        std::cout << "group members: " << std::endl;
        std::cout << "------------------------------------" << std::endl;
        auto &members = iter.getUsers();
        for (auto &m : members)
        {
            std::cout << "\tID: " << m.getId()
                      << "\tName: " << m.getName()
                      << "\tState: " << m.getState()
                      << "\tRole: " << m.getRole()
                      << std::endl;
        }
    }
}
void showOffMsgList(int a, std::string s)
{
    std::cout << "**********<< OfflineMessage list >>**********" << std::endl;
    for (auto &iter : g_currentUserOffMsg)
    {
        Json offmsg = Json::parse(iter);
        std::cout << "time: " << offmsg[TIME] << " ";
        if (offmsg.contains(GROUP_ID))
        {
            std::cout << "group[" << offmsg[GROUP_ID] << "]-";
        }
        std::cout << "user[" << offmsg[ID] << "] ---> "
                  << "message: " << offmsg[MSG] << std::endl;
    }
}