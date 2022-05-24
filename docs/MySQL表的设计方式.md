此次服务器项目使用MySQL数据库作为聊天数据的落地存储。



## 聊天程序基本必要的信息：

- 用户信息

  - 登录、注册、添加好友都需要查询用户信息

- 群组信息

  - 创建群、加入群、群聊天需要使用群的信息以及用户的信息

- 离线消息

  - 当用户处于离线状态，但其好友发送了消息，此消息称为离线消息。这里因为方便，我们也使用MySQL进行存储



## 表的设计：

- **User表**：保存用户信息的表。用于唯一标识用户的id（主键、自增）、用户姓名、用户密码、用户登录状态

![img](https://cdn.nlark.com/yuque/0/2022/png/23097089/1650207064390-df97fc61-8c29-4e04-85a9-5e976d32fdcc.png)

- **AllGroup表：** 保存群信息的表。用于唯一标识群的id（主键、自增）、群的名称、群的描述

![img](https://cdn.nlark.com/yuque/0/2022/png/23097089/1650207130748-839e5845-b6e8-4060-a468-68519189a169.png)

- **Friend表：** 表明好友关系的表。用户id，用户的好友friendid。

  - 为了避免好友关系的重复，这里将id和friendid设置为联合主键。例如，id为5其好友friendid为13，等价于id为13其好友friendid为5。

![img](https://cdn.nlark.com/yuque/0/2022/png/23097089/1650207299244-207c6766-03a5-4d28-9cd8-f2ed1e9a9964.png)

- **GroupUser表：** 表明群和用户之间关系的表。群的groupid，用户userid，以及用户在群中的角色。同样地，群与用户设置为联合主键避免重复

![img](https://cdn.nlark.com/yuque/0/2022/png/23097089/1650207428733-4aafe5a0-dfa1-4905-83dc-8bc09319b22b.png)

- **OfflineMessage表：** 用户userid，离线消息message。

![img](https://cdn.nlark.com/yuque/0/2022/png/23097089/1650208489854-7c9b14a4-cf64-4365-90e6-9fb5c21bd557.png)

五个表的关联性如下图所示：

![img](https://cdn.nlark.com/yuque/0/2022/png/23097089/1650208691215-839ac9e2-de2c-40d7-a5a3-ef797dcfca5c.png)