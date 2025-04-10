//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_APPLYFRIENDLIST_H
#define WECHAT_APPLYFRIENDLIST_H

#include <QListWidget>
#include <QEvent>

class ApplyFriendList: public QListWidget
{
  Q_OBJECT
public:
  ApplyFriendList(QWidget *parent = nullptr);
protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

private slots:

signals:
  void sig_show_search(bool);
};


#endif//WECHAT_APPLYFRIENDLIST_H
