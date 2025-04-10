//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_CHATUSERWID_H
#define WECHAT_CHATUSERWID_H

#include <QWidget>
#include "listitembase.h"
#include "userdata.h"
namespace Ui {
  class ChatUserWid;
}

class ChatUserWid : public ListItemBase
{
  Q_OBJECT
public:
  explicit ChatUserWid(QWidget *parent = nullptr);
  ~ChatUserWid() override;
  [[nodiscard]] QSize sizeHint() const override;
  void SetInfo(std::shared_ptr<UserInfo> user_info);
  void SetInfo(const std::shared_ptr<FriendInfo>& friend_info);
  void ShowRedPoint(bool bshow);
  std::shared_ptr<UserInfo> GetUserInfo();
  void updateLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs);
private:
  Ui::ChatUserWid *ui;
  std::shared_ptr<UserInfo> _user_info;
};

#endif//WECHAT_CHATUSERWID_H
