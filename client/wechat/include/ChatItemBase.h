//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_CHATITEMBASE_H
#define WECHAT_CHATITEMBASE_H


#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "global.h"
class BubbleFrame;

class ChatItemBase : public QWidget
{
  Q_OBJECT
public:
  explicit ChatItemBase(ChatRole role, QWidget *parent = nullptr);
  void setUserName(const QString &name);
  void setUserIcon(const QPixmap &icon);
  void setWidget(QWidget *w);

private:
  ChatRole m_role;
  QLabel *m_pNameLabel;
  QLabel *m_pIconLabel;
  QWidget *m_pBubble;
};


#endif//WECHAT_CHATITEMBASE_H
