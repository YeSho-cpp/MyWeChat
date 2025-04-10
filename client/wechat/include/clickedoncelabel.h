//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_CLICKEDONCELABEL_H
#define WECHAT_CLICKEDONCELABEL_H


#include <QLabel>
#include <QMouseEvent>

class ClickedOnceLabel:public QLabel
{
  Q_OBJECT
public:
  ClickedOnceLabel(QWidget *parent=nullptr);
  virtual void mouseReleaseEvent(QMouseEvent *ev) override;

signals:
  void clicked(QString);
};


#endif//WECHAT_CLICKEDONCELABEL_H
