//
// Created by YeSho on 2024/8/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RegisterDialog.h" resolved

#include "../include/registerdialog.h"
#include "ui_registerdialog.h"
#include "../include/global.h"
#include "../include/httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RegisterDialog) {
  ui->setupUi(this);
  // 将输入的字符隐蔽起来
  ui->pass_edit->setEchoMode(QLineEdit::Password);
  ui->confirm_edit->setEchoMode(QLineEdit::Password);
  ui->err_tip->setProperty("state","normal");
  repolish(ui->err_tip);
  // 注册连接http发来的一个通知
  connect(HttpMgr::getInstance().get(),&HttpMgr::sig_reg_mod_finish,this,
          &RegisterDialog::slot_reg_mod_finish);

  initHttpHandlers();
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
void RegisterDialog::slot_reg_mod_finish(ReqId id, const QString &res, ErrorCode err) {\

  if(err!=ErrorCode::SUCCESS){
    showTip(tr("网络请求错误"), false);
    return;
  }

  // 解析JSON 字符串，res 转换为QByterArray
  QJsonDocument jsonDoc =QJsonDocument::fromJson((res.toUtf8()));
  if(jsonDoc.isNull()){
    showTip(tr("json解析失败"), false);
    return;
  }
  //json 解析失败
  if(!jsonDoc.isObject()){
    showTip(tr("json解析失败"), false);
    return;
  }

  //
  _handlers[id](jsonDoc.object());
  return;
}
void RegisterDialog::initHttpHandlers() {
  // 注册获取验证码回包的逻辑
  _handlers.insert(ReqId::ID_GET_VARIFY_CODE,[this](const QJsonObject& jsonObject){
      int error = jsonObject["error"].toInt();
      if(error!=ErrorCode::SUCCESS){
        showTip(tr("参数错误"), false);
        return;
      }

      auto email =jsonObject["email"].toString();
      showTip(tr("验证码已经发送到邮箱，注意查收"), true);
      qDebug() <<"email is "<<email;
  });
}
