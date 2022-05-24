#ifndef GROUPUSER_H
#define GROUPUSER_H

// 定义一个派生类GroupUser，与User是“is-a”关系
#include <string>
#include "user.h"

class GroupUser : public User
{
public:
    void setRole(const std::string &role){ _role = role; }
    std::string getRole() const { return _role; }
private:
    std::string _role; // 该用户在群组中的角色：管理员 or 普通成员
};

#endif