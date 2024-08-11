//
// Created by YeSho on 2024/8/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoginDialog.h" resolved

#include "../include/logindialog.h"
#include "ui_logindialog.h"


LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog) {
  ui->setupUi(this);
  connect(ui->regis_button,&QPushButton::clicked,this,&LoginDialog::switchRegister);
}

LoginDialog::~LoginDialog() {
  delete ui;
}
