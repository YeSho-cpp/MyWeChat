//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_FINDFAILDLG_H
#define WECHAT_FINDFAILDLG_H

#include <QDialog>

namespace Ui {
  class FindFailDlg;
}

class FindFailDlg : public QDialog
{
  Q_OBJECT

public:
  explicit FindFailDlg(QWidget *parent = nullptr);
  ~FindFailDlg();

private slots:


  void on_fail_sure_btn_clicked();

private:
  Ui::FindFailDlg *ui;
};


#endif//WECHAT_FINDFAILDLG_H
