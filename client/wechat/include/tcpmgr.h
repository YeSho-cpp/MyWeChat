//
// Created by YeSho on 2024/9/24.
//

#ifndef WECHAT_TCPMGR_H
#define WECHAT_TCPMGR_H

#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
#include <QObject>
#include "usermgr.h"

class TcpMgr : public QObject,public Singleton<TcpMgr>,public std::enable_shared_from_this<TcpMgr>{
  Q_OBJECT  // 因为我们要发送信号和接受信号
public:
  ~TcpMgr();
private:
  friend class Singleton<TcpMgr>;
  TcpMgr();
  void initHandlers();
  void handleMsg(ReqId id,int len,QByteArray data);
  QTcpSocket _socket;
  QString _host; // 地址
  uint16_t _port; // 端口
  QByteArray _buffer; // 用于存储接收的数据。
  bool _b_recv_pending; // 标记是否有待处理的接收数据。 因为tcp是面向字节流的
  quint16 _message_id;
  quint16 _message_len;
  QMap<ReqId,std::function<void(ReqId id,int len,QByteArray data)>> _handlers;
public slots:
  void slot_tcp_connect(const ServerInfo&);
  void slot_send_data(ReqId reqId, const QByteArray& dataBytes);
signals:
  void sig_con_success(bool bsuccess);
  void sig_send_data(ReqId reqId, QByteArray data);
  void sig_swich_chatdlg();
  void sig_load_apply_list(QJsonArray json_array);
  void sig_login_failed(int);
  void sig_user_search(std::shared_ptr<SearchInfo>);
  void sig_friend_apply(std::shared_ptr<AddFriendApply>);
  void sig_add_auth_friend(std::shared_ptr<AuthInfo>);
  void sig_auth_rsp(std::shared_ptr<AuthRsp>);
  void sig_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
};


#endif//WECHAT_TCPMGR_H
