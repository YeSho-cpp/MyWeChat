//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_CHATUSERLIST_H
#define WECHAT_CHATUSERLIST_H


#include <QListWidget>
#include <QWheelEvent>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>

class ChatUserList: public QListWidget
{
  Q_OBJECT
public:
  explicit ChatUserList(QWidget *parent = nullptr);

protected:

  bool eventFilter(QObject *watched, QEvent *event) override;
private:
  bool _load_pending;
signals:
  void sig_loading_chat_user();
};



#endif//WECHAT_CHATUSERLIST_H
