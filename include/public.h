#ifndef PUBLIC_H
#define PUBLIC_H
#include <string>
#include <ctime>
#include <chrono>
using myClock = std::chrono::system_clock;
// 定义了一个枚举类CHAT_STATUS <---> MSG_ID
enum CHAT_STATUS : int
{
    LOG_IN = 1,       // 登录
    LOG_IN_MSG_ACK,   // 登录返回的ACK
    REG,              // 注册
    REG_MSG_ACK,      // 注册返回ack
    ONE_CHAT,         // 一对一聊天业务
    ADD_FRIEND,       // 添加好友
    ADD_FRIEND_ACK,   // 添加好友回复
    CREATE_GROUP,     // 创建群组
    CREATE_GROUP_ACK, // 创建群组服务器返回
    ADD_TO_GROUP,     // 将用户添加到群组中
    ADD_TO_GROUP_ACK, // 服务器返回 添加到群组中
    GROUP_CHAT,       // 群聊业务
    GROUP_CHAT_ACK,   // 群聊返回
    FRIEND_ONLINE,    // 好友上线，服务器的推送
    FRIEND_OFFLINE,   // 好友下线，服务器的推送
    LOG_OUT,          // 注销退出
};

// 定义一些string字段给JSON用，标准化一下
const std::string MSG_ID = "msgid";
const std::string MSG = "message";
const std::string ERRNO = "errno";
const std::string ERR_MSG = "err_message";
const std::string ID = "id";
const std::string NAME = "name";
const std::string STATE = "state";
const std::string PWD = "password";
const std::string CHAT_MSG = "chat_message";
const std::string FRIEND_ID = "friendid";
const std::string FRIEND_LIST = "friends";
const std::string GROUP_LIST = "group_lists";
const std::string GROUP_ID = "groupid";
const std::string GROUP_NAME = "groupname";
const std::string GROUP_DESC = "groupdesc";
const std::string GROUP_CHAT_MSG = "groupchatmsg";
const std::string GROUP_ROLE = "group_role";
const std::string GROUP_MEMS = "group_members";
const std::string TIME = "current_time";
const std::string OFF_MSG = "offlinemsg";

// 获取系统时间（聊天信息需要添加时间信息）
std::string getCurrentTime()
{
    myClock::time_point rightNow = myClock::now();
    std::time_t tt = myClock::to_time_t(rightNow);
    std::string ret(ctime(&tt));
    ret.erase(ret.end() - 1);
    return ret;
}
#endif