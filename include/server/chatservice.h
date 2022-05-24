#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <unordered_map>
#include <functional>
#include <muduo/net/TcpConnection.h>
#include <muduo/base/Logging.h>
#include <mutex> /*mutex unique_lock*/
#include <iostream>
#include <queue>
#include <chrono>
#include "json.hpp"
#include "usermodel.h"
#include "user.h"
#include "offlinemsgmodel.h"
#include "friendmodel.h"
#include "groupmodel.h"
#include "redis.h"
using namespace muduo;
using namespace muduo::net;
using namespace std;
using myClock = std::chrono::system_clock;
typedef nlohmann::json Json;
// 函数适配器msgHandler，有三个参数：const TcpConnectionPtr &, Json &, Timestamp
typedef std::function<void(const TcpConnectionPtr &, Json &, Timestamp)> msgHandler;

// 单例模式的一个模块：让该类只能产生一个对象

class ChatService
{
public:
    // 单例模式获取类对象的对外接口函数
    static ChatService *getInstance();
    // 暴露给chatserver的接口，用int整型匹配对应的处理函数，完成业务处理
    msgHandler getHandler(int msgid);

    // 不同功能的业务处理函数
    // 登录
    void login(const TcpConnectionPtr &, Json &, Timestamp);
    // 注册
    void reg(const TcpConnectionPtr &, Json &, Timestamp);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &);
    // 处理服务器异常宕机
    void serverCloseException();
    // 一对一的聊天业务
    void oneChat(const TcpConnectionPtr &, Json &, Timestamp);
    // 添加好友的业务
    void addFriend(const TcpConnectionPtr &, Json &, Timestamp);
    // 创建群组
    void createGroup(const TcpConnectionPtr &, Json &, Timestamp);
    // 添加用户至群组
    void addToGroup(const TcpConnectionPtr &, Json &, Timestamp);
    // 群聊业务
    void groupChat(const TcpConnectionPtr &, Json &, Timestamp);
    // 注销退出
    void logout(const TcpConnectionPtr &, Json &, Timestamp);
private:
    // 单列模式的私有构造函数
    ChatService();
    
    // 数字整型 与 业务处理函数的一个映射表
    // 这个表在程序工作的时候，只有读取操作，没有写入，即便是多线程同时访问，也不设计线程安全问题
    std::unordered_map<int, msgHandler> _msgHandlerMap;
    // 建立一个用户id映射用户的socket连接的哈希表
    std::unordered_map<int, TcpConnectionPtr> _usrConnMap;
    // 上面这个映射表会有修改的操作，需要保证线程安全
    std::mutex _connMtx;
    // User表的操作类对象
    UserModel _userModel;
    // offlinemessage表的操作对象
    OfflineMsgModel _offMsgModel;
    // friend表的操作对象
    FriendModel _friendModel;
    // 群组表的操作对象
    GroupModel _groupModel;
    // Redis服务器操作对象
    Redis _redisModel;
    // 订阅频道有更新，获取消息
    void redisNotifyHandler(int, std::string);
    
};

#endif