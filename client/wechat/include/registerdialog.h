//
// Created by YeSho on 2024/8/11.
//

#ifndef WECHAT_REGISTERDIALOG_H
#define WECHAT_REGISTERDIALOG_H

#include "global.h"
#include <QDialog>


namespace Ui {
  class RegisterDialog;
}

class RegisterDialog : public QDialog {
  Q_OBJECT

public:
  explicit RegisterDialog(QWidget *parent = nullptr);
  ~RegisterDialog() override;

  // 一些槽函数
private slots:
  void on_get_code_clicked();
  void on_cancel_btn_clicked();
  void on_sure_btn_clicked(); // 确认按钮
  void on_return_btn_clicked();

public slots:
  void slot_reg_mod_finish(ReqId id, const QString& res, ErrorCode err);
private:
  Ui::RegisterDialog *ui;
  void showTip(const QString &str, bool b_ok);
  void initHttpHandlers();
  QMap<ReqId,std::function<void(const QJsonObject&)>>_handlers; // 处理函数的映射
  bool checkUserValid();
  bool checkEmailValid();
  bool checkPassValid();
  bool checkConfirmValid();
  bool checkVarifyValid();
  void AddTipErr(TipErr err, QString qString);
  void DelTipErr(TipErr err);

  QMap<TipErr, QString> _tip_errs;

  QTimer* _countdown_timer; //计时器
  int _countdown; //倒计时

  void ChangeTipPage();

signals:
   void sigSwitchLogin();
};


#endif//WECHAT_REGISTERDIALOG_H
