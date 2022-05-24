#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H
#include "db.h"
#include "user.h"
#include <vector>
#include <string>
#include "json.hpp"
class FriendModel
{
public:
    // 定义接口方法
    // 添加一个好友
    bool insert(int id, int fid);
    // 返回自身id对应的好友列表
    void query(int usrId, std::vector<std::string> &result);
};

#endif