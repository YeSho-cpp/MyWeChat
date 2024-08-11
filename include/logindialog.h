//
// Created by YeSho on 2024/8/11.
//

#ifndef WECHAT_LOGINDIALOG_H
#define WECHAT_LOGINDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui {
  class LoginDialog;
}
QT_END_NAMESPACE

class LoginDialog : public QDialog {
  Q_OBJECT

public:
  explicit LoginDialog(QWidget *parent = nullptr);
  ~LoginDialog() override;

private:
  Ui::LoginDialog *ui;
signals:
  void switchRegister();
};


#endif//WECHAT_LOGINDIALOG_H
