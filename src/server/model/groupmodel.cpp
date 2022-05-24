#include "groupmodel.h"
#include "db.h"
#include <string.h>

// 创建一个新的群
bool GroupModel::creatGroup(Group &group)
{
    // 组装sql语句
    char sql[1024];
    sprintf(sql, "insert into allgroup(groupname, groupdesc) values('%s', '%s')",
            group.getGName().c_str(), group.getGDesc().c_str());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            group.setID(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}
// 向群里添加一个用户
bool GroupModel::addUser(int usrid, int gID, const std::string &role)
{
    // adduser 需要先检查gID的合法性

    // 组装sql语句
    char sql[1024];

    sprintf(sql, "select id from allgroup where id = %d", gID);

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr && mysql_fetch_row(res) != nullptr)
        {
            // gID 合法
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "insert into groupuser values(%d, %d, '%s')",
                    gID, usrid, role.c_str());
            mysql_free_result(res);
            return mysql.update(sql);
        }
    }
    return false;
}
// 获取一个用户全部的群
void GroupModel::queryAllGroup(int usrid, std::vector<Group> &groupVec)
{
    // 1. 组装SQL语句
    char sql[1024] = {0};
    // 对两个表进行联合查询
    sprintf(sql, "select a.id, a.groupname, a.groupdesc from allgroup a inner join \
            groupuser b on a.id = b.groupid where b.userid = %d",
            usrid);

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                Group g;
                g.setID(atoi(row[0]));
                g.setGName(row[1]);
                g.setGDesc(row[2]);
                groupVec.push_back(g);
            }
        }
    }
    // 继续查询。查询每一个群中的所有用户user信息
    for (Group &g : groupVec)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select a.id, a.name, a.state, b.grouprole from user a \
            inner join groupuser b on b.userid = a.id where b.groupid = %d",
                g.getID());
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                GroupUser gu;
                gu.setId(atoi(row[0]));
                gu.setName(row[1]);
                gu.setState(row[2]);
                gu.setRole(row[3]);
                g.getUsers().push_back(gu);
            }
        }
        mysql_free_result(res); // 释放资源
    }
}

// 获取某个群的所有成员(id表示用户)
bool GroupModel::queryOneGroup(int usrid, int gID, std::vector<int> &usridVec)
{
    // 组装sql语句
    char sql[1024];
    sprintf(sql, "select userid from groupuser where groupid = %d and userid != %d", gID, usrid);
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                usridVec.push_back(atoi(row[0]));
            }
            mysql_free_result(res);
            return true; // 连接成功、查询成功，返回TRUE；
        }
        mysql_free_result(res);
    }
    return false;
}
// 加一个重载函数，让其能够查询到一个群组的群成员的完整信息
bool GroupModel::queryOneGroup(int usrid, int gID, std::vector<GroupUser> &usrVec)
{
    // 组装sql语句
    char sql[1024];
    sprintf(sql, "select a.id, a.name, a.state, b.grouprole from user a \
            inner join groupuser b on b.userid = a.id where b.groupid = %d",
            gID);
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                GroupUser gu;
                gu.setId(atoi(row[0]));
                gu.setName(row[1]);
                gu.setState(row[2]);
                gu.setRole(row[3]);
                usrVec.push_back(gu);
            }
            mysql_free_result(res);
            return true;
        }
        mysql_free_result(res);
    }
    return false;
}
// 检查gid是否为服务器拥有的群
bool GroupModel::checkGroup(int gID)
{
    // 组装sql语句
    char sql[1024];
    sprintf(sql, "select id from allgroup where id = %d", gID);
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            // 找到这样的群组和用户映射关系
            mysql_free_result(res);
            return true;
        }
        mysql_free_result(res);
    }
    return false;
}