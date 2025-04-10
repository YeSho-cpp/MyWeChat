//
// Created by YeSho on 2024/9/15.
//

#include "StatusGrpcClient.h"

#include <memory>
GetChatServerRsp StatusGrpcClient::GetChatServer(int uid) {
  ClientContext context;
  GetChatServerRsp reply;
  GetChatServerReq request;
  request.set_uid(uid);
  auto stub = pool_->getConnection();
  Status status = stub->GetChatServer(&context, request, &reply);
  Defer defer([&stub, this]() {
    pool_->returnConnection(std::move(stub));
  });
  if (status.ok()) {
    return reply;
  }
  else {
    reply.set_error(ErrorCodes::RPCFailed);
    return reply;
  }
}

LoginRsp StatusGrpcClient::Login(int uid, const std::string& token) {

  ClientContext context;
  LoginRsp reply;
  LoginReq request;
  request.set_uid(uid);
  request.set_token(token);

  auto stub = pool_->getConnection();
  Status status = stub->Login(&context, request, &reply);
  Defer defer([&stub, this]() {
    pool_->returnConnection(std::move(stub));
  });
  if (status.ok()) {
    return reply;
  }
  else {
    reply.set_error(ErrorCodes::RPCFailed);
    return reply;
  }

}

StatusGrpcClient::StatusGrpcClient() {
  auto& gCfgMgr = ConfigMgr::Inst();
  std::string host = gCfgMgr["StatusServer"]["Host"];
  std::string port = gCfgMgr["StatusServer"]["Port"];
  pool_ = std::make_unique<StatusConPool>(5, host, port);
}
