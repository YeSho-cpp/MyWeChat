//
// Created by YeSho on 2024/9/11.
//

#ifndef GATESERVER_VERIFYGRPCCLIENT_H
#define GATESERVER_VERIFYGRPCCLIENT_H

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"


using grpc::Channel; // 表示到 gRPC 服务器的连接 可以被多个客户端stub共享
using grpc::Status;
using grpc::ClientContext; // RPC调用定制特定的行为和需求 为每封特殊邮件定制一个特殊的信封


using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool {
public:
  RPConPool(size_t poolsize,const std::string& host,const std::string& port);
  ~RPConPool();
  void Close();
  std::unique_ptr<VarifyService::Stub> getConnection();
  void returnConnection(std::unique_ptr<VarifyService::Stub> context);

private:
  std::atomic<bool>b_stop; // 指示池是否已停止。
  size_t poolsize_;
  std::string host_;
  std::string port_;
  std::queue<std::unique_ptr<VarifyService::Stub>> connections_; // 存储可用连接的队列。
  std::condition_variable cond_; // 条件变量
  std::mutex mutex_; // 互斥锁
};

class VerifyGrpcClient: public Singleton<VerifyGrpcClient>{
  friend class Singleton<VerifyGrpcClient>;
public:
  GetVarifyRsp GetVarifyCode(const std::string& email){
    ClientContext context;
    GetVarifyRsp reply;
    GetVarifyReq request;
    request.set_email(email);
    auto stub=pool_->getConnection();
    Status status= stub->GetVarifyCode(&context,request,&reply);
    if(status.ok()) {
      pool_->returnConnection(std::move(stub));
      return reply;
    }
    else{
      pool_->returnConnection(std::move(stub));
      reply.set_error(ErrorCodes::RPCFailed);
      return reply;
    }
  }

private:
  VerifyGrpcClient();
  std::unique_ptr<RPConPool> pool_;
//  std::unique_ptr<VarifyService::Stub> stub_;
};


#endif//GATESERVER_VERIFYGRPCCLIENT_H
