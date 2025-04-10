//
// Created by YeSho on 2024/8/11.
//

#ifndef WECHAT_LOGINDIALOG_H
#define WECHAT_LOGINDIALOG_H

#include "global.h"
#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui {
  class LoginDialog;
}
QT_END_NAMESPACE

class LoginDialog : public QDialog {
  Q_OBJECT

public:
  void initHead();
  explicit LoginDialog(QWidget *parent = nullptr);
  ~LoginDialog() override;

private slots:
  void slot_forget_pwd();
  void on_login_btn_clicked();
  void slot_login_mod_finish(ReqId id, const QString& res, ErrorCode err);
  void slot_tcp_con_finish(bool bsuccess);
  void slot_login_failed(int);

private:
  Ui::LoginDialog *ui;
  void initHttpHandlers();
  void showTip(const QString& str, bool b_ok);
  void AddTipErr(TipErr te, const QString& tips);
  void DelTipErr(TipErr te);
  bool enableBtn(bool enabled);
  bool checkPwdValid();
  bool checkUserValid();
  QMap<TipErr, QString> _tip_errs;
  QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
  int _uid;
  QString _token;
signals:
  void switchRegister();
  void switchReset();
  void sig_connect_tcp(ServerInfo);
};


#endif//WECHAT_LOGINDIALOG_H
