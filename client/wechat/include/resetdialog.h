//
// Created by YeSho on 2024/9/14.
//

#ifndef WECHAT_RESETDIALOG_H
#define WECHAT_RESETDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
  class ResetDialog;
}

class ResetDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ResetDialog(QWidget *parent = nullptr);
  ~ResetDialog();

private slots:
  void on_return_btn_clicked();

  void on_varify_btn_clicked();

  void slot_reset_mod_finish(ReqId id, QString res, ErrorCode err);
  void on_sure_btn_clicked();


private:
  bool checkUserValid();
  bool checkPassValid();
  void showTip(const QString& str,bool b_ok);
  bool checkEmailValid();
  bool checkVarifyValid();
  void AddTipErr(TipErr te,QString tips);
  void DelTipErr(TipErr te);
  void initHandlers();
  Ui::ResetDialog *ui;
  QMap<TipErr, QString> _tip_errs;
  QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
  QTimer * _countdown_timer;
  int _countdown; // 倒计时计数
signals:
  void switchLogin();
};

#endif//WECHAT_RESETDIALOG_H
