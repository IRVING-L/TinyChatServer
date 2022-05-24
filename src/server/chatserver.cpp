#include "chatserver.h"
#include "chatservice.h"

// 构造函数
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg) : _server(loop, listenAddr, nameArg)
{
    // 注册连接回调函数
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    // 注册消息回调函数
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
    // 设置线程数量
    _server.setThreadNum(8);
}
ChatServer::~ChatServer() {}
void ChatServer::start()
{
    _server.start();
}

// 处理连接的函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // 打印一下新连接的客户端消息
    LOG_INFO << "ChatServer: " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "TRUE" : "FALSE");
    if(conn->disconnected())
    {
        ChatService::getInstance()->clientCloseException(conn);
        conn->shutdown();
    }
}
// 上报读写事件相关信息的回调函数
// 这个函数是多线程的，里面的操作需要保证线程安全
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buf,
                           Timestamp time)
{
    // 在此处打印一下服务器接收到的消息
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " recived " << msg 
             << " | at time: " << time.toString();
    // 转换成json格式
    Json js = Json::parse(msg);
    // 调用类chatservice实例对象，由json中的msgid选择对应的回调函数
    auto Handler = ChatService::getInstance()->getHandler(js["msgid"].get<int>());
    // 执行函数
    Handler(conn, js, time);
}