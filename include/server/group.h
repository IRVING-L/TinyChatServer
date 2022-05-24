#ifndef GROUP_H
#define GROUP_H
#include <string>
#include <vector>
#include "groupuser.h"
// 映射数据库chat中表allgroup的 ORM 类
class Group
{
public:
    // 构造函数
    Group(int id = -1, const std::string &name = "", const std::string &desc = "")
        : _gID(id), _gName(name), _gDesc(desc) {}

    // 对外接口
    void setID(int id) { _gID = id; }
    void setGName(const std::string &name) { _gName = name; }
    void setGDesc(const std::string &desc) { _gDesc = desc; }

    const int getID() const { return _gID; }
    std::string getGName() { return _gName; }
    std::string getGDesc() { return _gDesc; }
    std::vector<GroupUser> &getUsers() { return _users; }

private:
    int _gID;                      // 群聊的id
    std::string _gName;            // 群聊id的组名
    std::string _gDesc;            // 这个群组的描述describe
    std::vector<GroupUser> _users; // 一个组有几个用户？
};

#endif