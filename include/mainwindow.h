//
// Created by YeSho on 2024/8/11.
//

#ifndef WECHAT_MAINWINDOW_H
#define WECHAT_MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui {
  class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
public slots:
  void SlotSwitchReg();

private:
  Ui::MainWindow *ui;
  LoginDialog * _login_dlg;
  RegisterDialog * _register_dlg;

};


#endif//WECHAT_MAINWINDOW_H
