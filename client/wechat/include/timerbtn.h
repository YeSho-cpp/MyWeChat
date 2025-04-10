//
// Created by YeSho on 2024/9/14.
//

#ifndef WECHAT_TIMERBTN_H
#define WECHAT_TIMERBTN_H

#include <QPushButton>
#include <QTimer>

class TimerBtn : public QPushButton
{
public:
  explicit TimerBtn(QWidget *parent = nullptr);
  ~ TimerBtn() override;
  // 重写mouseReleaseEvent
  void mouseReleaseEvent(QMouseEvent *e) override;
private:
  QTimer  *_timer;
  int _counter;
};

#endif//WECHAT_TIMERBTN_H
