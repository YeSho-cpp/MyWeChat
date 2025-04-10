//
// Created by YeSho on 2024/8/17.
//

#ifndef GATESERVER_CSERVER_H
#define GATESERVER_CSERVER_H

#include <const.h>
#include "HttpConnection.h"
class CServer: public std::enable_shared_from_this<CServer>{
public:
  CServer(boost::asio::io_context& ioc,unsigned short &port); // 这里epoll要添加端口的监听任务，由io_context负责
  void start();
private:
  tcp::acceptor _acceptor; // 是一个接收器，主要接收对端连接，会跟底层事件循环去通信，底层就绪事件会抛给它
  net::io_context& _ioc;
  // tcp::socket _socket; // 可以复用，每有一个连接，就可以绑定它
};


#endif//GATESERVER_CSERVER_H
