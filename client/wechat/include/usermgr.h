//
// Created by YeSho on 2024/9/25.
//

#ifndef WECHAT_USERMGR_H
#define WECHAT_USERMGR_H


#include <QObject>
#include <memory>
#include <singleton.h>
#include "userdata.h"
#include <vector>
class UserMgr:public QObject,public Singleton<UserMgr>,
                public std::enable_shared_from_this<UserMgr>
{
  Q_OBJECT
public:
  friend class Singleton<UserMgr>;
  ~ UserMgr() override;
  void SetUserInfo(std::shared_ptr<UserInfo> user_info);
  void SetToken(QString token);
  int GetUid();
  QString GetName();
  QString GetIcon();
  std::shared_ptr<UserInfo> GetUserInfo();
  void AppendApplyList(const QJsonArray& array);
  void AppendFriendList(const QJsonArray& array);
  std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
  void AddApplyList(std::shared_ptr<ApplyInfo> app);
  bool AlreadyApply(int uid);
  std::vector<std::shared_ptr<FriendInfo>> GetChatListPerPage();
  bool IsLoadChatFin();
  void UpdateChatLoadedCount();
  std::vector<std::shared_ptr<FriendInfo>> GetConListPerPage();
  void UpdateContactLoadedCount();
  bool IsLoadConFin();
  bool CheckFriendById(int uid);
  void AddFriend(std::shared_ptr<AuthRsp> auth_rsp); // 我们自己认证别人服务器返回的回包
  void AddFriend(std::shared_ptr<AuthInfo> auth_info); // 对方收到的我们这样一个通知加到好友里面
  std::shared_ptr<FriendInfo> GetFriendById(int uid); // 通过id查到具体的好友信息
  void AppendFriendChatMsg(int friend_id,std::vector<std::shared_ptr<TextChatData>>);
private:
  UserMgr();
  std::shared_ptr<UserInfo> _user_info;
  std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
  std::vector<std::shared_ptr<FriendInfo>> _friend_list;
  QMap<int, std::shared_ptr<FriendInfo>> _friend_map;
  QString _token;
  int _chat_loaded;
  int _contact_loaded;

public slots:
  void SlotAddFriendRsp(std::shared_ptr<AuthRsp> rsp);
  void SlotAddFriendAuth(std::shared_ptr<AuthInfo> auth);
};



#endif//WECHAT_USERMGR_H
