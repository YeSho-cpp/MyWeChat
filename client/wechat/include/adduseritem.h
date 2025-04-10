//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_ADDUSERITEM_H
#define WECHAT_ADDUSERITEM_H

#include <QWidget>
#include "listitembase.h"
namespace Ui {
  class AddUserItem;
}

class AddUserItem : public ListItemBase
{
  Q_OBJECT

public:
  explicit AddUserItem(QWidget *parent = nullptr);
  ~AddUserItem() override;
  [[nodiscard]] QSize sizeHint() const override {
    return {250, 70}; // 返回自定义的尺寸
  }
protected:

private:
  Ui::AddUserItem *ui;
};


#endif//WECHAT_ADDUSERITEM_H
