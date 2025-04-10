//
// Created by YeSho on 2024/8/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoginDialog.h" resolved

#include "../include/logindialog.h"
#include "clickedlabel.h"
#include "httpmgr.h"
#include "tcpmgr.h"
#include "ui_logindialog.h"
#include <QPainter>
#include <QPainterPath>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog) {
  ui->setupUi(this);
  connect(ui->reg_btn,&QPushButton::clicked,this,&LoginDialog::switchRegister);
  ui->forget_label->SetState("normal","hover","","selected","selected_hover","");

  connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);
  initHead();
  initHttpHandlers();
  // 连接登录回包信号
  connect(HttpMgr::getInstance().get(),&HttpMgr::sig_login_mod_finish,this,&LoginDialog::slot_login_mod_finish);

  //连接tcp连接请求的信号和槽函数
  connect(this, &LoginDialog::sig_connect_tcp, TcpMgr::getInstance().get(), &TcpMgr::slot_tcp_connect);
  //连接tcp管理者发出的连接成功信号
  connect(TcpMgr::getInstance().get(), &TcpMgr::sig_con_success, this, &LoginDialog::slot_tcp_con_finish);
  //连接tcp管理者发出的登陆失败的信号
  connect(TcpMgr::getInstance().get(),&TcpMgr::sig_login_failed,this,&LoginDialog::slot_login_failed);
}
void LoginDialog::initHead() {
  // 加载图片
  QPixmap originalPixmap(":/ice.png");
  // 设置图片自动缩放
  qDebug()<< originalPixmap.size() << ui->head_label->size();
  originalPixmap = originalPixmap.scaled(ui->head_label->size(),
                                         Qt::KeepAspectRatio, Qt::SmoothTransformation);

  // 创建一个和原始图片相同大小的QPixmap，用于绘制圆角图片
  QPixmap roundedPixmap(originalPixmap.size());
  roundedPixmap.fill(Qt::transparent); // 用透明色填充

  QPainter painter(&roundedPixmap);
  painter.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿，
  painter.setRenderHint(QPainter::SmoothPixmapTransform); // 使圆角更平滑

  // 使用QPainterPath设置圆角
  QPainterPath path;
  path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10); // 最后两个参数分别是x和y方向的圆角半径
  painter.setClipPath(path);

  // 将原始图片绘制到roundedPixmap上
  painter.drawPixmap(0, 0, originalPixmap);

  // 设置绘制好的圆角图片到QLabel上
  ui->head_label->setPixmap(roundedPixmap);
}

LoginDialog::~LoginDialog() {
  delete ui;
}

void LoginDialog::showTip(const QString& str, bool b_ok) {
  if(b_ok){
    ui->err_tip->setProperty("state","normal");
  }else{
    ui->err_tip->setProperty("state","err");
  }

  ui->err_tip->setText(str);

  repolish(ui->err_tip);
}


void LoginDialog::slot_forget_pwd() {
  qDebug()<<"slot forget pwd";
  emit switchReset();
}
void LoginDialog::on_login_btn_clicked() {
  qDebug()<<"login btn clicked";
  if(!checkUserValid()){
    return;
  }
  if(!checkPwdValid()){
    return ;
  }
  enableBtn(false);
  auto email =ui->email_edit->text();
  auto pwd = ui->pass_edit->text();

  //发送http请求登录
  QJsonObject json_obj;
  json_obj["email"] = email;
  json_obj["passwd"] = xorString(pwd);
  HttpMgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),
                                      json_obj, ReqId::ID_LOGIN_USER,Modules::LOGINMOD);
}
bool LoginDialog::checkUserValid() {
  auto email = ui->email_edit->text();
  if(email.isEmpty()){
    qDebug() << "email empty " ;
    AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱不能为空"));
    return false;
  }
  DelTipErr(TipErr::TIP_EMAIL_ERR);
  return true;
}
bool LoginDialog::checkPwdValid() {
  auto pwd = ui->pass_edit->text();
  if(pwd.length() < 6 || pwd.length() > 15){
    qDebug() << "Pass length invalid";
    //提示长度不准确
    AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
    return false;
  }

  // 创建一个正则表达式对象，按照上述密码要求
  // 这个正则表达式解释：
  // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
  QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
  bool match = regExp.match(pwd).hasMatch();
  if(!match){
    //提示字符非法
    AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符且长度为(6~15)"));
    return false;;
  }

  DelTipErr(TipErr::TIP_PWD_ERR);

  return true;
}

bool LoginDialog::enableBtn(bool enabled) {
  ui->login_btn->setEnabled(enabled);
  ui->reg_btn->setEnabled(enabled);
  return true;
}


void LoginDialog::AddTipErr(TipErr te, const QString& tips) {
  _tip_errs[te]=tips;
  showTip(tips, false);
}

void LoginDialog::DelTipErr(TipErr te) {
  _tip_errs.remove(te);
  if(_tip_errs.empty()){
    ui->err_tip->clear();
    return;
  }
  showTip(_tip_errs.first(),false);
}

// 初始化http回调逻辑, 并添加_handlers成员
void LoginDialog::initHttpHandlers() {
  //注册获取登录回包逻辑
  _handlers.insert(ReqId::ID_LOGIN_USER, [this](QJsonObject jsonObj){
    int error = jsonObj["error"].toInt();
    if(error != ErrorCode::SUCCESS){
      showTip(tr("参数错误"),false);
      enableBtn(true);
      return;
    }
    auto email = jsonObj["email"].toString();

    //发送信号通知tcpMgr发送长链接
    ServerInfo si; // 这是存储服务器给我们回来的信息
    si.Uid = jsonObj["uid"].toInt();
    si.Host = jsonObj["host"].toString();
    si.Port = jsonObj["port"].toString();
    si.Token = jsonObj["token"].toString();

    _uid = si.Uid;
    _token = si.Token;
    qDebug()<< "email is " << email << " uid is " << si.Uid <<" host is "
             << si.Host << " Port is " << si.Port << " Token is " << si.Token;
    emit sig_connect_tcp(si);
  });
}
void LoginDialog::slot_login_mod_finish(ReqId id, const QString& res, ErrorCode err) {

  if(err != ErrorCode::SUCCESS){
    showTip(tr("网络请求错误"),false);
    return;
  }

  // 解析 JSON 字符串,res需转化为QByteArray
  QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
  //json解析错误
  if(jsonDoc.isNull()){
    showTip(tr("json解析错误"),false);
    return;
  }

  //json解析错误
  if(!jsonDoc.isObject()){
    showTip(tr("json解析错误"),false);
    return;
  }


  //调用对应的逻辑,根据id回调。
  _handlers[id](jsonDoc.object());
}
void LoginDialog::slot_tcp_con_finish(bool bsuccess) {

  if(bsuccess){
    showTip(tr("聊天服务连接成功，正在登录..."),true);
    QJsonObject jsonObj;
    jsonObj["uid"] = _uid;
    jsonObj["token"] = _token;

    // 这是连接建立后，发送登陆聊天服务器请求

    QJsonDocument doc(jsonObj);
    QByteArray jsonString = doc.toJson(QJsonDocument::Indented);
    //发送tcp请求给chat server
    TcpMgr::getInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN, jsonString);

  }else{
    showTip(tr("网络异常"),false);
    enableBtn(true);
  }

}
void LoginDialog::slot_login_failed(int err) {

  QString result=QString("登录失败，err is %1").arg(err);

  showTip(result, false);

  enableBtn(true);
}
