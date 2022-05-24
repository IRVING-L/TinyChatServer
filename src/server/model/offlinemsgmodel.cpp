#include "offlinemsgmodel.h"

// 存储用户的离线消息
bool OfflineMsgModel::insert(const int usrID, const std::string &msg)
{
    // 1. 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage(userid, message) values(%d, '%s')",usrID, msg.c_str());

    MySQL mysql;
    // 建立连接
    bool isConnect = mysql.connect();
    if (isConnect)
    {
        // 插入数据
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}
// 删除用户的离线消息
bool OfflineMsgModel::remove(const int usrID)
{
    // 1. 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "delete from offlinemessage where userid = %d",usrID);

    MySQL mysql;
    // 建立连接
    bool isConnect = mysql.connect();
    if (isConnect)
    {
        // 插入数据
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}
// 将数据库的离线消息输出到一个队列中去
bool OfflineMsgModel::query(const int usrID, std::vector<std::string> &vec)
{
    // 组装sql语句
    char sql[1024];
    sprintf(sql, "select message from offlinemessage where userid = %d", usrID);

    // 创建MySQL对象操作MySQL数据库
    bool ret = false;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                vec.push_back(row[0]);
            }
            ret = true;
        }
        mysql_free_result(res); // 释放动态资源对象res
    }
    return ret;
}