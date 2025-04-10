//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_FINDSUCCESSDLG_H
#define WECHAT_FINDSUCCESSDLG_H

#include <QDialog>
#include <memory>
#include "userdata.h"
namespace Ui {
  class FindSuccessDlg;
}

class FindSuccessDlg : public QDialog
{
  Q_OBJECT

public:
  explicit FindSuccessDlg(QWidget *parent = nullptr);
  ~FindSuccessDlg() override;
  void SetSearchInfo(const std::shared_ptr<SearchInfo>& si);
private slots:
  void on_add_friend_btn_clicked();

private:
  Ui::FindSuccessDlg *ui; // 访问在 Qt Designer 中设计的所有 UI 元素
  QWidget * _parent;
  std::shared_ptr<SearchInfo> _si; // 存储搜索信息的智能指针
};


#endif//WECHAT_FINDSUCCESSDLG_H
