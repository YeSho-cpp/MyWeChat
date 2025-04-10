//
// Created by YeSho on 2024/10/1.
//

#ifndef CHATSERVER_CHATSERVICEIMPL_H
#define CHATSERVER_CHATSERVICEIMPL_H

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <mutex>
#include "data.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using message::AddFriendReq;
using message::AddFriendRsp;

using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::ChatService;
using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;

class ChatServiceImpl final: public ChatService::Service
{
public:
  ChatServiceImpl();
  Status NotifyAddFriend(ServerContext* context, const AddFriendReq* request,
                         AddFriendRsp* reply) override; // 处理添加好友的通知。

  Status NotifyAuthFriend(ServerContext* context,
                          const AuthFriendReq* request, AuthFriendRsp* response) override; // 处理好友认证的通知。

  Status NotifyTextChatMsg(::grpc::ServerContext* context,
                           const TextChatMsgReq* request, TextChatMsgRsp* response) override; // 处理文本聊天消息的通知

  bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo); // 获取用户的基本信息。

private:
};

#endif//CHATSERVER_CHATSERVICEIMPL_H
