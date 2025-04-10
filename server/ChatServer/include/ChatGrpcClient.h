//
// Created by YeSho on 2024/10/1.
//

#ifndef CHATSERVER_CHATGRPCCLIENT_H
#define CHATSERVER_CHATGRPCCLIENT_H

#include "const.h"
#include "Singleton.h"
#include "ConfigMgr.h"
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <queue>
#include "data.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::AddFriendReq;
using message::AddFriendRsp;

using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::GetChatServerRsp;
using message::LoginRsp;
using message::LoginReq;
using message::ChatService;

using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;

// 连接可以被多个线程访问，为了保证连接的一个安全性，是一个互斥性和高效性，用了池

class ChatConPool {
public:
  ChatConPool(size_t poolSize, std::string host, std::string port) // 对端的地址和端口
      : poolSize_(poolSize), host_(host), port_(port), b_stop_(false) {
    for (size_t i = 0; i < poolSize_; ++i) {

      std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
                                                             grpc::InsecureChannelCredentials()); // 建立通道
      connections_.push(ChatService::NewStub(channel));
    }
  }

  ~ChatConPool() {
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    while (!connections_.empty()) {
      connections_.pop();
    }
  }

  std::unique_ptr<ChatService::Stub> getConnection() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this] {
      if (b_stop_) {
        return true;
      }
      return !connections_.empty();
    });
    //如果停止则直接返回空指针
    if (b_stop_) {
      return  nullptr;
    }
    auto context = std::move(connections_.front());
    connections_.pop();
    return context;
  }

  void returnConnection(std::unique_ptr<ChatService::Stub> context) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (b_stop_) {
      return;
    }
    connections_.push(std::move(context));
    cond_.notify_one();
  }

  void Close() {
    b_stop_ = true;
    cond_.notify_all();
  }

private:
  atomic<bool> b_stop_; // 是否关闭
  size_t poolSize_;
  std::string host_;
  std::string port_;
  std::queue<std::unique_ptr<ChatService::Stub>> connections_; // 连接队列
  std::mutex mutex_;
  std::condition_variable cond_;
};

class ChatGrpcClient :public Singleton<ChatGrpcClient> {
  friend class Singleton<ChatGrpcClient>;
public:
  ~ChatGrpcClient() = default;

  AddFriendRsp NotifyAddFriend(std::string server_ip, const AddFriendReq& req); // 通知对方加好友
  AuthFriendRsp NotifyAuthFriend(std::string server_ip, const AuthFriendReq& req); // 通知对方认证
  bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);
  TextChatMsgRsp NotifyTextChatMsg(std::string server_ip, const TextChatMsgReq& req, const Json::Value& rtvalue);
private:
  ChatGrpcClient();
  unordered_map<std::string, std::unique_ptr<ChatConPool>> _pools; // 每个对端连接都会一个连接的池子
};


#endif//CHATSERVER_CHATGRPCCLIENT_H
