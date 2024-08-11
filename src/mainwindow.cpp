//
// Created by YeSho on 2024/8/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "../include/mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  this->setWindowIcon(QIcon(":/wechat.ico"));
  setWindowTitle("Wechat");
  ui->setupUi(this);

  _login_dlg =new LoginDialog(this);
  _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
  setCentralWidget(_login_dlg);

  // 创建和注册消息链接
  connect(_login_dlg,&LoginDialog::switchRegister,this,&MainWindow::SlotSwitchReg);
  _register_dlg=new RegisterDialog(this);
  _register_dlg->hide(); // 确保 _register_dlg 不被显示


  _register_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::SlotSwitchReg() {
  setCentralWidget(_register_dlg);
  _login_dlg->hide();
  _register_dlg->show();
}