//
// Created by YeSho on 2024/8/11.
//

#ifndef WECHAT_MAINWINDOW_H
#define WECHAT_MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"
#include "chatdialog.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
public slots:
  void SlotSwitchReg();
  void SlotSwitchLogin();
  void SlotSwitchReset();
  void SlotSwitchLogin2();
  void SlotSwitchChat();
private:
  Ui::MainWindow *ui;
  LoginDialog* _login_dlg;
  RegisterDialog* _reg_dlg;
  ResetDialog* _reset_dlg;
  ChatDialog* _chat_dlg;
};


#endif//WECHAT_MAINWINDOW_H
