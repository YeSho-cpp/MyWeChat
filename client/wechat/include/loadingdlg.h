//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_LOADINGDLG_H
#define WECHAT_LOADINGDLG_H

#include <QDialog>

namespace Ui {
  class LoadingDlg;
}

class LoadingDlg : public QDialog
{
  Q_OBJECT

public:
  explicit LoadingDlg(QWidget *parent = nullptr);
  ~LoadingDlg() override;

private:
  Ui::LoadingDlg *ui;
};


#endif//WECHAT_LOADINGDLG_H
