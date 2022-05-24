#include "friendmodel.h"

// 添加一个好友
bool FriendModel::insert(int id, int fid)
{
    // 组装sql语句
    // 做一个优化，一次连接数据库，多次查询，避免掉多次连接数据库的操作
    // 第一步：查user表，fid是否合法

    char sql[1024];
    sprintf(sql, "select id from user where id = %d", fid);
    // sprintf(sql, "insert into friend(userid, friendid) values(%d, %d)", id, fid);

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr && mysql_fetch_row(res) != nullptr)
        {
            mysql_free_result(res);
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "insert into friend(userid, friendid) values(%d, %d)", id, fid);
            return mysql.update(sql);
        }
        mysql_free_result(res);
    }
    return false;
}
// 返回自身id对应的好友列表
void FriendModel::query(int usrId, std::vector<std::string> &result)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    MySQL mysql;
    if (mysql.connect())
    {
        sprintf(sql, "select a.id,a.name,a.state from user a \
                    inner join friend b on b.friendid = a.id where b.userid = %d",
                usrId);
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            // 把userid用户的所有离线消息放入vec中返回
            MYSQL_ROW row;
            nlohmann::json js;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                js["id"] = atoi(row[0]);
                js["name"] = row[1];
                js["state"] = row[2];
                result.push_back(js.dump());
            }
            mysql_free_result(res);
        }
        // 还需要再查一次，把userID作为friendID查
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select a.id,a.name,a.state from user a \
                        inner join friend b on b.userid = a.id where b.friendid = %d",
                usrId);
        res = mysql.query(sql);
        if (res != nullptr)
        {
            // 把userid用户的所有离线消息放入vec中返回
            MYSQL_ROW row;
            nlohmann::json js;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                js["id"] = atoi(row[0]);
                js["name"] = row[1];
                js["state"] = row[2];
                result.push_back(js.dump());
            }
            mysql_free_result(res);
        }
    }
}