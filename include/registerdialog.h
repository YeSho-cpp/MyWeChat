//
// Created by YeSho on 2024/8/11.
//

#ifndef WECHAT_REGISTERDIALOG_H
#define WECHAT_REGISTERDIALOG_H

#include "global.h"
#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui {
  class RegisterDialog;
}
QT_END_NAMESPACE

class RegisterDialog : public QDialog {
  Q_OBJECT

public:
  explicit RegisterDialog(QWidget *parent = nullptr);
  ~RegisterDialog() override;

  // 一些槽函数
private slots:
  void on_get_code_clicked();
  void slot_reg_mod_finish(ReqId id, const QString& res, ErrorCode err);
private:
  Ui::RegisterDialog *ui;
  void showTip(const QString &str, bool b_ok);
  void initHttpHandlers();
  QMap<ReqId,std::function<void(const QJsonObject&)>>_handlers;
};


#endif//WECHAT_REGISTERDIALOG_H
