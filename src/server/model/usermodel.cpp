#include "usermodel.h"
#include <string>
#include <mysql/mysql.h>
bool UserModel::insert(User &usr)
{
    // 1. 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')",
            usr.getName().c_str(), usr.getPassword().c_str(), usr.getState().c_str());

    MySQL mysql;
    // 建立连接
    bool isConnect = mysql.connect();
    if (isConnect)
    {
        // 插入数据
        if (mysql.update(sql))
        {
            usr.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}
// 用id查询user表中的数据
User UserModel::query(int id)
{
    // 组装sql语句
    char sql[1024];
    sprintf(sql, "select * from user where id = %d", id);

    // 创建MySQL对象操作MySQL数据库
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                User _usr;
                _usr.setId(atoi(row[0]));
                _usr.setName(row[1]);
                _usr.setPassword(row[2]);
                _usr.setState(row[3]);
                return _usr; // 返回结果
            }
        }
        mysql_free_result(res); // 释放动态资源对象res
    }
    // 没有得到正确查询结果
    return User(); // 返回一个空结果，id默认为-1
}
// 更新id对应的数据的登录状态
bool UserModel::updateState(User &usr)
{
    // 组装sql语句
    char sql[1024];
    sprintf(sql, "update user set state = '%s' where id = %d", usr.getState().c_str(), usr.getId());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}
// 服务器异常宕机，下线所有的用户
bool UserModel::offlineAll()
{
    // 组装sql语句
    char sql[1024];
    sprintf(sql, "update user set state = '%s' where state = '%s'", "offline", "online");

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}