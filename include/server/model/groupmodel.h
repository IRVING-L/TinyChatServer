#ifndef GROUPMODEL_H
#define GROUPMODEL_H
#include "group.h"
#include "groupuser.h"
#include <string>
#include <vector>
// 对数据库中的两张群组的表进行操作的一个类

class GroupModel
{
public:
    // 创建一个新的群
    bool creatGroup(Group &group);
    // 向群里添加一个用户
    bool addUser(int usrid, int gID, const std::string &role);
    // 获取一个用户全部的群
    void queryAllGroup(int usrid, std::vector<Group> &groupVec);
    // 获取某个群的所有成员(id表示用户)
    bool queryOneGroup(int usrid, int gID, std::vector<int> &usridVec);
    // 加一个重载函数，让其能够查询到一个群组的群成员的完整信息
    bool queryOneGroup(int usrid, int gID, std::vector<GroupUser> &usrVec);
    // 检查gid是否为服务器拥有的群
    bool checkGroup(int gID);

private:
};

#endif