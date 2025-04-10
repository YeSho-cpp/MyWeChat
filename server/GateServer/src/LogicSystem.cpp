//
// Created by YeSho on 2024/8/18.
//

#include "LogicSystem.h"


#include "HttpConnection.h"
#include "VerifyGrpcClient.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"
#include "StatusGrpcClient.h"
void LogicSystem::RegGet(const std::string& url, const HttpHandler& handler) {
  _get_handlers.insert(std::make_pair(url,handler));
}

void LogicSystem::RegPost(const std::string& url, const HttpHandler& handler) {
  _post_handlers.insert(std::make_pair(url,handler));
}

LogicSystem::LogicSystem() {
  RegGet("/get_test",[](const std::shared_ptr<HttpConnection>&connection){
    beast::ostream(connection->_response.body())<<"receive get_test req";
    int i=0;
    for(auto& elem :connection->_get_params){
      i++;
      beast::ostream(connection->_response.body())<<"param "<<i<<"key is "<<elem.first;
      beast::ostream(connection->_response.body())<<"param "<<i<<"value is "<<elem.second<<std::endl;
    }
  });
  RegPost("/get_varifycode",[](const std::shared_ptr<HttpConnection>&connection){
      auto body_str=boost::beast::buffers_to_string(connection->_request.body().data());
      std::cout<<"receive body is "<<body_str<<std::endl;
      connection->_response.set(http::field::content_type,"text/json");
      Json::Value root;
      Json::Reader reader;
      Json::Value src_root;
      bool parse_success = reader.parse(body_str,src_root);
      if(!parse_success){
        std::cout<<"Fail to parse JSON data!"<<std::endl;
        root["error"]=ErrorCodes::Error_Json;
        std::string jsonstr =root.toStyledString();
        beast::ostream(connection->_response.body())<<jsonstr;
        return true;
      }

      if (!src_root.isMember("email")) {
        std::cout << "Failed to parse JSON data!" << std::endl;
        root["error"] = ErrorCodes::Error_Json;
        std::string jsonstr = root.toStyledString();
        beast::ostream(connection->_response.body()) << jsonstr;
        return true;
      }

      auto email =src_root["email"].asString();
      GetVarifyRsp rsp=VerifyGrpcClient::getInstance()->GetVarifyCode(email); // 里面会进行验证服务
      std::cout<<"email is "<<email<<std::endl;
      root["error"]=rsp.error();
      root["email"]=src_root["email"];
      std::string jsonstr=root.toStyledString();
      beast::ostream(connection->_response.body())<<jsonstr;
      return true;

  });

  RegPost("/user_register", [](const std::shared_ptr<HttpConnection>& connection) {
    auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
    std::cout << "receive body is " << body_str << std::endl;
    connection->_response.set(http::field::content_type, "text/json");
    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;
    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success) {
      std::cout << "Failed to parse JSON data!" << std::endl;
      root["error"] = ErrorCodes::Error_Json;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    auto email = src_root["email"].asString();
    auto name = src_root["user"].asString();
    auto pwd = src_root["passwd"].asString();
    auto confirm = src_root["confirm"].asString();

    if(pwd!=confirm){
      std::cout << "password err " << std::endl;
      root["error"] = ErrorCodes::PasswdErr;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    //先查找redis中email对应的验证码是否合理
    std::string  varify_code;
    bool b_get_varify = RedisMgr::getInstance()->Get(CODEPREFIX+src_root["email"].asString(), varify_code);
    if (!b_get_varify) {
      std::cout << " get varify code expired" << std::endl;
      root["error"] = ErrorCodes::VarifyExpired;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    if (varify_code != src_root["varifycode"].asString()) {
      std::cout << " varify code error" << std::endl;
      root["error"] = ErrorCodes::VarifyCodeErr;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

//    //访问redis查找
//    bool b_usr_exist = RedisMgr::getInstance()->ExistsKey(src_root["user"].asString());
//    if (b_usr_exist) {
//      std::cout << " user exist" << std::endl;
//      root["error"] = ErrorCodes::UserExist;
//      std::string jsonstr = root.toStyledString();
//      beast::ostream(connection->_response.body()) << jsonstr;
//      return true;
//    }

    //查找数据库判断用户是否存在
    int uid=MysqlMgr::getInstance()->RegUser(name,email,pwd);
    if(uid==0||uid==-1){ // 0代表用户或者邮箱已经存在了
      std::cout<<" user or email exist"<<std::endl;
      root["error"]=ErrorCodes::UserExist;
      std::string jsonstr=root.toStyledString();
      beast::ostream(connection->_response.body())<<jsonstr;
      return true;
    }

    root["error"] = 0;
    root["email"] = src_root["email"];
    root["uid"] = uid;
    root["user"]= name;
    root["passwd"] = pwd;
    root["confirm"] = confirm;
    root["varifycode"] = src_root["varifycode"].asString();
    std::string jsonstr = root.toStyledString();
    beast::ostream(connection->_response.body()) << jsonstr;
    return true;

  });

  //重置回调逻辑
  RegPost("/reset_pwd", [](const std::shared_ptr<HttpConnection>& connection) {

    auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
    std::cout << "receive body is " << body_str << std::endl;
    connection->_response.set(http::field::content_type, "text/json");

    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;

    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success) {
      std::cout << "Failed to parse JSON data!" << std::endl;
      root["error"] = ErrorCodes::Error_Json;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    auto email = src_root["email"].asString();
    auto name = src_root["user"].asString();
    auto pwd = src_root["passwd"].asString();

    //先查找redis中email对应的验证码是否合理
    std::string  varify_code;
    bool b_get_varify = RedisMgr::getInstance()->Get(CODEPREFIX + src_root["email"].asString(), varify_code);
    if (!b_get_varify) {
      std::cout << " get varify code expired" << std::endl;
      root["error"] = ErrorCodes::VarifyExpired;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    if (varify_code != src_root["varifycode"].asString()) {
      std::cout << " varify code error" << std::endl;
      root["error"] = ErrorCodes::VarifyCodeErr;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    //查询数据库判断用户名和邮箱是否匹配
    bool email_valid = MysqlMgr::getInstance()->CheckEmail(name, email);
    if (!email_valid) {
      std::cout << " user email not match" << std::endl;
      root["error"] = ErrorCodes::EmailNotMatch;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    //更新密码为最新密码
    bool b_up = MysqlMgr::getInstance()->UpdatePwd(name, pwd);
    if (!b_up) {
      std::cout << " update pwd failed" << std::endl;
      root["error"] = ErrorCodes::PasswdUpFailed;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    std::cout << "succeed to update password " << pwd << std::endl;
    root["error"] = 0;
    root["email"] = email;
    root["user"] = name;
    root["passwd"] = pwd;
    root["varifycode"] = src_root["varifycode"].asString();

    std::string jsonstr = root.toStyledString();
    beast::ostream(connection->_response.body()) << jsonstr;
    return true;
  });

  // 用户登录逻辑
  RegPost("/user_login", [](const std::shared_ptr<HttpConnection>& connection) {

    auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
    std::cout << "receive body is " << body_str << std::endl;
    connection->_response.set(http::field::content_type, "text/json");
    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;
    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success) {
      std::cout << "Failed to parse JSON data!" << std::endl;
      root["error"] = ErrorCodes::Error_Json;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    auto email = src_root["email"].asString();
    auto pwd = src_root["passwd"].asString();
    UserInfo userInfo;
    //查询数据库判断用户名和密码是否匹配
    bool pwd_valid = MysqlMgr::getInstance()->CheckPwd(email, pwd, userInfo);
    if (!pwd_valid) {
      std::cout << " user pwd not match" << std::endl;
      root["error"] = ErrorCodes::PasswdInvalid;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    //查询StatusServer找到合适的连接
    auto reply = StatusGrpcClient::getInstance()->GetChatServer(userInfo.uid);
    if (reply.error()) {
      std::cout << " grpc get chat server failed, error is " << reply.error()<< std::endl;
      root["error"] = ErrorCodes::RPCFailed;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->_response.body()) << jsonstr;
      return true;
    }

    std::cout << "succeed to load userinfo uid is " << userInfo.uid << std::endl;
    root["error"] = 0;
    root["email"] = email;
    root["uid"] = userInfo.uid;
    root["token"] = reply.token();
    root["host"] = reply.host();
    root["port"] = reply.port();
    std::string jsonstr = root.toStyledString();
    beast::ostream(connection->_response.body()) << jsonstr;
    return true;
  });

}
bool LogicSystem::HandleGet(const std::string& path, std::shared_ptr<HttpConnection> con) {
  if(_get_handlers.find(path)==_get_handlers.end()){
    return false;
  }

  _get_handlers[path](std::move(con));
  return true;
}

bool LogicSystem::HandlePost(const std::string& path, std::shared_ptr<HttpConnection> con) {
  if (_post_handlers.find(path) == _post_handlers.end()) {
    return false;
  }
  _post_handlers[path](std::move(con));
  return true;
}

LogicSystem::~LogicSystem() = default;
