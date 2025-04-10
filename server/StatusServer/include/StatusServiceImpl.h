//
// Created by YeSho on 2024/9/15.
//

#ifndef STATUSSERVER_STATUSSERVICEIMPL_H
#define STATUSSERVER_STATUSSERVICEIMPL_H

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include <mutex>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::LoginReq;
using message::LoginRsp;
using message::StatusService;


class  ChatServer {
public:
  ChatServer():host(""),port(""),name(""),con_count(0){}
  ChatServer(const ChatServer& cs):host(cs.host), port(cs.port), name(cs.name), con_count(cs.con_count){}
  ChatServer& operator=(const ChatServer& cs) {
    if (&cs == this) {
      return *this;
    }

    host = cs.host;
    name = cs.name;
    port = cs.port;
    con_count = cs.con_count;
    return *this;
  }
  std::string host;
  std::string port;
  std::string name;
  int con_count;
};


class StatusServiceImpl final : public StatusService::Service
{
public:
  StatusServiceImpl();
  Status GetChatServer(ServerContext* context, const GetChatServerReq* request,
                       GetChatServerRsp* reply) override;
//  std::vector<ChatServer> _servers;
//  int _server_index; // 用于_server_index
  Status Login(ServerContext* context, const LoginReq* request,
               LoginRsp* reply) override;

private:
  void insertToken(int uid, const std::string& token);
  ChatServer getChatServer();
  std::unordered_map<std::string, ChatServer> _servers;
  std::mutex _server_mtx;
};


#endif // STATUSSERVER_STATUSSERVICEIMPL_H
