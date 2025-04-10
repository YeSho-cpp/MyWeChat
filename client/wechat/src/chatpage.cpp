//
// Created by YeSho on 2024/9/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatPage.h" resolved

#include "chatpage.h"
#include "ChatItemBase.h"
#include "PictureBubble.h"
#include "TextBubble.h"
#include "applyfrienditem.h"
#include "tcpmgr.h"
#include "ui_chatpage.h"
#include "usermgr.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QStyleOption>
#include <QUuid>

ChatPage::ChatPage(QWidget *parent) :
                                      QWidget(parent),
                                      ui(new Ui::ChatPage)
{
  ui->setupUi(this);
  //设置按钮样式
  ui->receive_btn->SetState("normal","hover","press");
  ui->send_btn->SetState("normal","hover","press");

  //设置图标样式
  ui->emo_lb->SetState("normal","hover","press","normal","hover","press");
  ui->file_lb->SetState("normal","hover","press","normal","hover","press");

}

ChatPage::~ChatPage()
{
  delete ui;
}

void ChatPage::SetUserInfo(const std::shared_ptr<UserInfo>& user_info)
{
  _user_info = user_info;
  //设置ui界面
  ui->title_lb->setText(_user_info->_name);
  ui->chat_data_list->removeAllItem();
  for(auto & msg : user_info->_chat_msgs){
    AppendChatMsg(msg);
  }
}

void ChatPage::AppendChatMsg(std::shared_ptr<TextChatData> msg)
{
  auto self_info = UserMgr::getInstance()->GetUserInfo();
  ChatRole role;
  //todo... 添加聊天显示
  if (msg->_from_uid == self_info->_uid) {
    role = ChatRole::Self;
    auto* pChatItem = new ChatItemBase(role);

    pChatItem->setUserName(self_info->_name);
    pChatItem->setUserIcon(QPixmap(self_info->_icon));
    QWidget* pBubble = nullptr;
    pBubble = new TextBubble(role, msg->_msg_content);
    pChatItem->setWidget(pBubble);
    ui->chat_data_list->appendChatItem(pChatItem);
  }
  else {
    role = ChatRole::Other;
    auto* pChatItem = new ChatItemBase(role);
    auto friend_info = UserMgr::getInstance()->GetFriendById(msg->_from_uid);
    if (friend_info == nullptr) {
      return;
    }
    pChatItem->setUserName(friend_info->_name);
    pChatItem->setUserIcon(QPixmap(friend_info->_icon));
    QWidget* pBubble = nullptr;
    pBubble = new TextBubble(role, msg->_msg_content);
    pChatItem->setWidget(pBubble);
    ui->chat_data_list->appendChatItem(pChatItem);
  }


}

void ChatPage::paintEvent(QPaintEvent *event)
{
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatPage::on_send_btn_clicked()
{
  if (_user_info == nullptr) {
    qDebug() << "friend_info is empty";
    return;
  }

  auto user_info = UserMgr::getInstance()->GetUserInfo();
  auto pTextEdit = ui->chatEdit;
  ChatRole role = ChatRole::Self;
  QString userName = user_info->_name;
  QString userIcon = user_info->_icon;

  const QVector<MsgInfo>& msgList = pTextEdit->getMsgList(); // 获取得到一个消息列表
  QJsonObject textObj;
  QJsonArray textArray;
  int txt_size = 0;

  for(const auto & i : msgList)
  {
    //消息内容长度不合规就跳过
    if(i.content.length() > 1024){
      continue;
    }

    QString type = i.msgFlag;
    auto *pChatItem = new ChatItemBase(role);
    pChatItem->setUserName(userName);
    pChatItem->setUserIcon(QPixmap(userIcon));
    QWidget *pBubble = nullptr;

    if(type == "text")
    {
      //生成唯一id
      QUuid uuid = QUuid::createUuid();
      //转为字符串
      QString uuidString = uuid.toString();

      pBubble = new TextBubble(role, i.content);
      if(txt_size + i.content.length()> 1024){
        textObj["fromuid"] = user_info->_uid;
        textObj["touid"] = _user_info->_uid;
        textObj["text_array"] = textArray;
        QJsonDocument doc(textObj);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
        //发送并清空之前累计的文本列表
        txt_size = 0;
        textArray = QJsonArray();
        textObj = QJsonObject();
        //发送tcp请求给chat server
        emit TcpMgr::getInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ, jsonData);
      }

      //将bubble和uid绑定，以后可以等网络返回消息后设置是否送达
      //_bubble_map[uuidString] = pBubble;
      txt_size += i.content.length();
      QJsonObject obj;
      QByteArray utf8Message = i.content.toUtf8();
      obj["content"] = QString::fromUtf8(utf8Message);
      obj["msgid"] = uuidString;
      textArray.append(obj);
      auto txt_msg = std::make_shared<TextChatData>(uuidString, obj["content"].toString(),
                                                    user_info->_uid, _user_info->_uid);
      emit sig_append_send_chat_msg(txt_msg);
    }
    else if(type == "image")
    {
      pBubble = new PictureBubble(QPixmap(i.content) , role);
    }
    else if(type == "file")
    {

    }
    //发送消息
    if(pBubble != nullptr)
    {
      pChatItem->setWidget(pBubble);
      ui->chat_data_list->appendChatItem(pChatItem);
    }

  }

  qDebug() << "textArray is " << textArray ;
  //发送给服务器
  textObj["text_array"] = textArray;
  textObj["fromuid"] = user_info->_uid;
  textObj["touid"] = _user_info->_uid;
  QJsonDocument doc(textObj);
  QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
  //发送并清空之前累计的文本列表
  txt_size = 0;
  textArray = QJsonArray();
  textObj = QJsonObject();
  //发送tcp请求给chat server
  emit TcpMgr::getInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ, jsonData);
}

void ChatPage::on_receive_btn_clicked()
{
  auto pTextEdit = ui->chatEdit;
  ChatRole role = ChatRole::Other;
  QString userName = _user_info->_name;
  QString userIcon = _user_info->_icon;

  const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();

  for(const auto & i : msgList)
  {
    QString type = i.msgFlag;
    auto *pChatItem = new ChatItemBase(role);
    pChatItem->setUserName(userName);
    pChatItem->setUserIcon(QPixmap(userIcon));
    QWidget *pBubble = nullptr;
    if(type == "text")
    {
      pBubble = new TextBubble(role, i.content);
    }
    else if(type == "image")
    {
      pBubble = new PictureBubble(QPixmap(i.content) , role);
    }
    else if(type == "file")
    {

    }
    if(pBubble != nullptr)
    {
      pChatItem->setWidget(pBubble);
      ui->chat_data_list->appendChatItem(pChatItem);
    }
  }
}

void ChatPage::clearItems()
{
  ui->chat_data_list->removeAllItem();
}
