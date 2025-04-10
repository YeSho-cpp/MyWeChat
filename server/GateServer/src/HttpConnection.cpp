//
// Created by YeSho on 2024/8/17.
//

#include "HttpConnection.h"
#include "LogicSystem.h"

HttpConnection::HttpConnection(boost::asio::io_context &ioc): _socket(ioc) {

}


void HttpConnection::Start() {
  auto self=shared_from_this();
  http::async_read(_socket,_buffer,_request,[self](beast::error_code ec,std::size_t bytes_transferred){
    try {
        if(ec){
          std::cout <<"http read err is "<<ec.what()<<std::endl;
          return;
        }
        boost::ignore_unused(bytes_transferred); // 忽略掉这个信息
        self->HandleReq();
        self->CheckDeadline();
    }catch(std::exception& exp){
        std::cout<<"exception is "<<exp.what()<<std::endl;
    }
  });
}

//char 转为16进制
unsigned char ToHex(unsigned char x)
{
  return  x > 9 ? x + 55 : x + 48;
}

// 16机制转 char
unsigned char FromHex(unsigned char x)
{
  unsigned char y;
  if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
  else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
  else if (x >= '0' && x <= '9') y = x - '0';
  else assert(0);
  return y;
}

// 例子 微信项目 2023年->%E5%BE%AE%E4%BF%A1%E9%A1%B9%E7%9B%AE+2023%E5%B9%B4
std::string UrlEncode(const std::string& str)
{
  std::string strTemp;
  size_t length = str.length();
  for (size_t i = 0; i < length; i++)
  {
    //判断是否仅有数字和字母构成
    if (isalnum((unsigned char)str[i]) ||
        (str[i] == '-') ||
        (str[i] == '_') ||
        (str[i] == '.') ||
        (str[i] == '~'))
      strTemp += str[i];
    else if (str[i] == ' ') //为空字符
      strTemp += "+";
    else
    {
      //其他字符需要提前加%并且高四位和低四位分别转为16进制
      strTemp += '%';
      strTemp += ToHex((unsigned char)str[i] >> 4);
      strTemp += ToHex((unsigned char)str[i] & 0x0F);
    }
  }
  return strTemp;
}


std::string UrlDecode(const std::string& str)
{
  std::string strTemp;
  size_t length = str.length();
  for (size_t i = 0; i < length; i++)
  {
    //还原+为空
    if (str[i] == '+') strTemp += ' ';
    //遇到%将后面的两个字符从16进制转为char再拼接
    else if (str[i] == '%')
    {
      assert(i + 2 < length);
      unsigned char high = FromHex((unsigned char)str[++i]);
      unsigned char low = FromHex((unsigned char)str[++i]);
      strTemp += high * 16 + low;
    }
    else strTemp += str[i];
  }
  return strTemp;
}

void HttpConnection::HandleReq() {
  // 设置版本
  _response.version(_request.version());
  _response.keep_alive(true);
  if(_request.method()==http::verb::get){
    // get请求要先进行解析
    PreParseGetParam();
   bool success =LogicSystem::getInstance()->HandleGet(_get_url,shared_from_this());
   if(!success){
     _response.result(http::status::not_found);
     _response.set(http::field::content_type,"text/plain");
     beast::ostream(_response.body())<<"url not found\r\n";
     WriteResponse();
     return;
   }

   _response.result(http::status::ok);
   _response.set(http::field::server,"GateServer");
   WriteResponse();
   return;
  }

  if (_request.method() == http::verb::post) {
    bool success = LogicSystem::getInstance()->HandlePost(_request.target(), shared_from_this());
    if (!success) {
      _response.result(http::status::not_found);
      _response.set(http::field::content_type, "text/plain");
      beast::ostream(_response.body()) << "url not found\r\n";
      WriteResponse();
      return;
    }
    _response.result(http::status::ok);
    _response.set(http::field::server, "GateServer");
    WriteResponse();
    return;
  }
}


void HttpConnection::WriteResponse() {
  auto self=shared_from_this();
  _response.content_length(_response.body().size());
  http::async_write(_socket,_response,[self](beast::error_code ec,std::size_t bytes_transferred){
    self->_socket.shutdown(tcp::socket::shutdown_send,ec);
    self->deadline_.cancel(); // 取消任何未完成的计时器
  });
}

void HttpConnection::CheckDeadline() {
  auto self=shared_from_this();
  deadline_.async_wait([self](beast::error_code ec){ // 启动计时器，设置一个回调函数
    if (!ec){
      self->_socket.close(ec);
    }
  });
}

/*
 * GET /api/user?name=%E5%BC%A0%E4%B8%89&age=30&city=%E5%8C%97%E4%BA%AC HTTP/1.1
 * Host: example.com
 */

/*
 * 第一个参数对：key = "name", value = "张三"（解码后）
  第二个参数对：key = "age", value = "30"
  第三个参数对：key = "city", value = "北京"（解码后）
 */

void HttpConnection::PreParseGetParam() {
  // 提取 URI
  auto uri = _request.target();
  // 查找查询字符串的开始位置（即 '?' 的位置）
  auto query_pos = uri.find('?');
  if (query_pos == std::string::npos) {
    _get_url = uri;
    return;
  }
  _get_url = uri.substr(0, query_pos);
  std::string query_string = uri.substr(query_pos + 1);
  std::string key;
  std::string value;
  size_t pos = 0;
  while ((pos = query_string.find('&')) != std::string::npos) {
    auto pair = query_string.substr(0, pos);
    size_t eq_pos = pair.find('=');
    if (eq_pos != std::string::npos) {
      key = UrlDecode(pair.substr(0, eq_pos)); // 假设有 url_decode 函数来处理URL解码
      value = UrlDecode(pair.substr(eq_pos + 1));
      _get_params[key] = value;
    }
    query_string.erase(0, pos + 1);
  }
  // 处理最后一个参数对（如果没有 & 分隔符）
  if (!query_string.empty()) {
    size_t eq_pos = query_string.find('=');
    if (eq_pos != std::string::npos) {
      key = UrlDecode(query_string.substr(0, eq_pos));
      value = UrlDecode(query_string.substr(eq_pos + 1));
      _get_params[key] = value;
    }
  }
}
