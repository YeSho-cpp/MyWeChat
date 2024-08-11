//
// Created by YeSho on 2024/8/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RegisterDialog.h" resolved

#include "../include/registerdialog.h"
#include "ui_registerdialog.h"


RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RegisterDialog) {
  ui->setupUi(this);
  // 将输入的字符隐蔽起来
  ui->pass_edit->setEchoMode(QLineEdit::Password);
  ui->confirm_edit->setEchoMode(QLineEdit::Password);
  ui->err_tip->setProperty("state","normal");
  repolish(ui->err_tip);

}

RegisterDialog::~RegisterDialog() {
  delete ui;
}

void RegisterDialog::on_get_code_clicked() {

  // 我们在点击获取验证码时，用正则检验一下
  auto email  = ui->emai_label->text();
  QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
  bool match =emailRegex.match(email).hasMatch();

  // 符合显示绿色的提示
  if(match){
      // 发送验证码
  }
  // 不符合显示红色的错误提示
  else{
    showTip(tr("邮箱地址不正确"), false);
  }
}
void RegisterDialog::showTip(const QString& str,bool b_ok) {
  if(b_ok){
    ui->err_tip->setProperty("state","normal");
  }else {
    ui->err_tip->setProperty("state","err");
  }
  ui->err_tip->setText(str);
  ui->err_tip->setProperty("state","err");
  repolish(ui->err_tip);
}
