//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_LINEITEM_H
#define WECHAT_LINEITEM_H

#include <QWidget>
#include "listitembase.h"

namespace Ui {
  class LineItem;
}

class LineItem : public ListItemBase
{
  Q_OBJECT

public:
  explicit LineItem(QWidget *parent = nullptr);
  ~LineItem();
  QSize sizeHint() const override;
private:
  Ui::LineItem *ui;
};

#endif//WECHAT_LINEITEM_H
