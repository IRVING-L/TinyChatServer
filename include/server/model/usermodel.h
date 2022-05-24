#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.h"
#include "db/db.h"

// User表的增删改查操作类

class UserModel
{
public:
    // User表的增加方法
    bool insert(User &usr);
    // 用id查询user表中的数据
    User query(int id);
    // 更新usr的数据的登录状态
    bool updateState(User &usr);
    // 服务器异常宕机，下线所有的用户
    bool offlineAll();
private:
};

#endif