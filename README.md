# MyWeChat

使用C++开发的跨平台全栈仿微信聊天系统

<img src="http://yesho-web.oss-cn-hangzhou.aliyuncs.com/img/20250416143846.png" alt="image.png" style="zoom:80%;" />


# 项目简介

- 项目描述：使用C++开发的跨平台全栈仿微信聊天系统，实现了包括用户注册、登录、即时通讯、离线消息、文件传输等核心功能。项目采用分布式架构，涵盖了从前端界面到后端服务的完整技术栈。
- 技术栈：客户端：C++/Qt, Asio (TCP通信)， Beast (HTTP请求)，服务端：C++， Asio， Beast， GRPC， Node.js (验证服务)，数据存储：Redis (缓存)， MySQL (持久化存储)，开发环境：Linux，CLion
- 主要工作：  
    1. 使用Qt框架设计并实现跨平台客户端界面，包括气泡聊天对话框、好友添加、好友通信、聊天记录展示等功能，并进行qss优化。  
    2. 基于Asio库开发高性能的异步TCP服务器，实现客户端与服务器间的实时通信；利用Beast库构建HTTP网关服务（GateServer），处理客户端连接请求和用户注册流程。  
    3. 设计并实现基于GRPC的微服务架构，包括验证服务（VarifyServer）和状态服务（StatusServer），并分布式部署多个ChatServer承载用户聊天连接。  
    4. 集成Redis用于缓存处理和会话管理，提高系统响应速度和用户体验；实现数据持久化存储，使用MySQL数据库管理用户信息和聊天记录，封装了基于mysqlconnector的连接池。  
    5. 开发文件传输功能，支持断点续传，提高大文件传输的可靠性和效率。  
    6. 基于Asio实现异步通信，通过连接池管理连接，包括Redis连接池和gRPC连接池。
- 项目难点：  
    1. 设计高并发、低延迟的分布式系统架构，确保消息的实时性和系统的可扩展性。  
    2. 实现复杂的网络通信模型，包括TCP长连接、HTTP短连接和GRPC微服务通信。  
    3. 优化大规模并发连接下的服务器性能，合理利用Redis缓存减轻数据库压力，单服务器支持8000连接，多服务器分布部署可支持1W~2W活跃用户。


- 这是一个全栈的即时通讯项目，前端基于QT实现气泡聊天对话框，通过`QListWidget`实现好友列表，利用`GridLayout`和`QPainter`封装气泡聊天框组件，基于`QT network`模块封装`http`和`tcp`服务。支持添加好友，好友通信，聊天记录展示等功能，仿微信布局并使用`qss`优化界面

- 后端采用分布式设计，分为`GateServer`网关服务，多个`ChatServer`聊天服务，`StatusServer`状态服务以及`VerifyServer`验证服务。

- 各服务通过`grpc`通信，支持断线重连。`GateServer`网关对外采用`http`服务，负责处理用户登录和注册功能。登录时`GateServer`从`StatusServer`查询聊天服务达到负载均衡，`ChatServer`聊天服务采用`asio`实现tcp可靠长链接异步通信和转发, 采用多线程模式封装`iocontext`池提升并发性能。数据存储采用mysql服务，并基于`mysqlconnector`库封装连接池，同时封装`redis`连接池处理缓存数据，以及`grpc`连接池保证多服务并发访问。

经测试单服务器支持8000连接，多服务器分布部署可支持1W~2W活跃用户。

# 项目结构


<img src="https://cdn.llfc.club/1709009717000.jpg" alt="image.png" style="zoom:60%;" />



1. GateServer为网关服务，主要应对客户端的连接和注册请求，因为服务器是是分布式，所以GateServer收到用户连接请求后会查询状态服务选择一个负载较小的Server地址给客户端，客户端拿着这个地址直接和Server通信建立长连接。
2. 当用户注册时会发送给GateServer, GateServer调用VarifyServer验证注册的合理性并发送验证码给客户端，客户端拿着这个验证码去GateServer注册即可。
3. StatusServer， ServerA， ServerB都可以直接访问Redis和Mysql服务。

<img src="http://yesho-web.oss-cn-hangzhou.aliyuncs.com/img/20241009222820.png" alt="image.png" style="zoom:60%;" />

<img src="http://yesho-web.oss-cn-hangzhou.aliyuncs.com/img/20241009222856.png" alt="image.png" style="zoom:60%;" />


# 项目目录

📁 ​**​客户端​**​  
└─ <a href="./client/wechat">wechat</a>  

🖥️ ​**​服务端​**​  
├─ <a href="./server/GateServer">GateServer</a>  
├─ <a href="./server/VarifyServer">VarifyServer</a>  
├─ <a href="./server/StatusServer">StatusServer</a>  
├─ <a href="./server/ChatServer">ChatServer</a>  
└─ <a href="./server/ChatServer2">ChatServer2</a>  
