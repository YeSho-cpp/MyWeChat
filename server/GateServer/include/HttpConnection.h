//
// Created by YeSho on 2024/8/17.
//

#ifndef GATESERVER_HTTPCONNECTION_H
#define GATESERVER_HTTPCONNECTION_H

#include "const.h"

class HttpConnection: public std::enable_shared_from_this<HttpConnection>{
  friend class LogicSystem;
public:
  explicit HttpConnection(boost::asio::io_context & ioc);
  void Start();
  tcp::socket& GetSocket(){
    return _socket;
  }
private:
  void CheckDeadline(); // 发送完一个数据，我们要进行超时检测
  void WriteResponse(); // 收到数据，我们要应答一下
  void HandleReq(); // 处理请求
  void PreParseGetParam(); //参数解析
  tcp::socket _socket;
  // 假设我们需要一个8k的缓冲区去接收http发过来的数据
  beast::flat_buffer _buffer{8192};
  http::request<http::dynamic_body> _request;
  http::response<http::dynamic_body> _response;
  net::steady_timer deadline_{
          _socket.get_executor(),std::chrono::seconds(60)
  }; // 设置计时器的初始超时时间为 60 秒。
  std::string _get_url;
  std::unordered_map<std::string, std::string> _get_params;
};


#endif//GATESERVER_HTTPCONNECTION_H
