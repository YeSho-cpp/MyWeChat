//
// Created by YeSho on 2024/8/18.
//

#ifndef GATESERVER_LOGICSYSTEM_H
#define GATESERVER_LOGICSYSTEM_H
#include "const.h"

class HttpConnection;

typedef std::function<void(std::shared_ptr<HttpConnection>)>HttpHandler;

class LogicSystem: public Singleton<LogicSystem>{
  friend class Singleton<LogicSystem>;
public:
  ~LogicSystem();
  bool HandleGet(const std::string&, std::shared_ptr<HttpConnection>);
  void RegGet(const std::string&,const HttpHandler& handler);
  void RegPost(const std::string&,const HttpHandler& handler);
  bool HandlePost(const std::string& path, std::shared_ptr<HttpConnection> con);
private:
  LogicSystem();
  std::map<std::string,HttpHandler>_post_handlers;
  std::map<std::string,HttpHandler>_get_handlers; // 存储 GET 请求的处理器映射。
};


#endif//GATESERVER_LOGICSYSTEM_H
