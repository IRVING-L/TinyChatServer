# 基于Linux的C++集群聊天服务器


## Introduction 

本项目为C++11编写的聊天服务器，其特点为：
- 使用**Muduo**库提供底层的网络I/O；
- 使用**MySQL**数据库存放好友、群组等信息；
- 使用**Nginx**拓展服务器集群，提高服务器的并发量；
- 基于**Redis**的发布-订阅服务设计了消息队列，用于跨服务器的客户端通信。

## Envoirment 

- OS：Ubuntu 20.04
- Complier: g++ 9.4
- C++ 11
- MySQL 8.0.28
- Nginx 1.21.6
- Redis

本项目需要配置的环境较为复杂，请查看[项目环境配置教程](docs/环境配置.md)

## Build

	./autobuild.sh
