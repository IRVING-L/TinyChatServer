#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <functional>
#include "json.hpp"
using namespace muduo;
using namespace muduo::net;
typedef nlohmann::json Json;

// 聊天服务器的主类
class ChatServer
{
public:
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg);

    ~ChatServer();

    void start();

private:
    // 处理连接的函数
    void onConnection(const TcpConnectionPtr &arg);
    // 上报读写事件相关信息的回调函数
    void onMessage(const TcpConnectionPtr &arg,
                   Buffer *,
                   Timestamp);

    TcpServer _server; // 实现服务器功能的类对象
    EventLoop *_loop;   // eventloop对象指针
};

#endif