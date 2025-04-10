//
// Created by YeSho on 2024/9/11.
//

#include "VerifyGrpcClient.h"

#include <memory>
#include "ConfigMgr.h"

RPConPool::RPConPool(size_t poolsize, const std::string& host, const std::string& port):poolsize_(poolsize),host_(host),port_(port),b_stop(false) {

  std::string address = host;
  address += ":";
  address += port;
  for(int i=0;i<poolsize;i++){
    std::shared_ptr<Channel> channel=grpc::CreateChannel(address,grpc::InsecureChannelCredentials());
    connections_.push(VarifyService::NewStub(channel));
  }

}
RPConPool::~RPConPool() {
  std::lock_guard<std::mutex>lock(mutex_);
  Close();
  while(!connections_.empty()){
    connections_.pop();
  }
}
void RPConPool::Close() {
  b_stop=true;
  cond_.notify_all();
}
std::unique_ptr<VarifyService::Stub> RPConPool::getConnection() {
  std::unique_lock<std::mutex> lock(mutex_);
  cond_.wait(lock, [this] {
    if (b_stop) {
      return true;
    }
    return !connections_.empty();
  });

  //如果停止则直接返回空指针
  if (b_stop) {
    return  nullptr;
  }
  auto context = std::move(connections_.front());
  connections_.pop();
  return context;
}
void RPConPool::returnConnection(std::unique_ptr<VarifyService::Stub> context) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (b_stop) {
    return;
  }
  connections_.push(std::move(context));
  cond_.notify_one();
}
VerifyGrpcClient::VerifyGrpcClient() {
  auto& gCfgMgr=ConfigMgr::Inst();
  std::string host = gCfgMgr["VarifyServer"]["Host"];
  std::string port = gCfgMgr["VarifyServer"]["Port"];
  pool_ = std::make_unique<RPConPool>(5, host, port);
}
