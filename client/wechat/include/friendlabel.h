//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_FRIENDLABEL_H
#define WECHAT_FRIENDLABEL_H

#include <QWidget>


#include <QFrame>
#include <QString>

namespace Ui {
  class FriendLabel;
}

class FriendLabel : public QFrame
{
  Q_OBJECT

public:
  explicit FriendLabel(QWidget *parent = nullptr);
  ~FriendLabel();
  void SetText(QString text);
  int Width();
  int Height();
  QString Text();
private:
  Ui::FriendLabel *ui;
  QString _text;
  int _width;
  int _height;
public slots:
  void slot_close();
signals:
  void sig_close(QString);
};


#endif//WECHAT_FRIENDLABEL_H
