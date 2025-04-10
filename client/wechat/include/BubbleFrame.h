//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_BUBBLEFRAME_H
#define WECHAT_BUBBLEFRAME_H


#include <QFrame>
#include "global.h"
#include <QHBoxLayout>
class BubbleFrame : public QFrame
{
  Q_OBJECT
public:
  explicit BubbleFrame(ChatRole role, QWidget *parent = nullptr);
  void setMargin(int margin);
  //inline int margin(){return margin;}
  void setWidget(QWidget *w);
protected:
  void paintEvent(QPaintEvent *e);
private:
  QHBoxLayout *m_pHLayout;
  ChatRole m_role;
  int      m_margin;
};

#endif//WECHAT_BUBBLEFRAME_H
