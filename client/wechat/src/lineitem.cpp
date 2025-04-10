//
// Created by YeSho on 2024/9/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LineItem.h" resolved

#include "lineitem.h"
#include "ui_lineitem.h"

LineItem::LineItem(QWidget *parent) :
                                      ListItemBase(parent),
                                      ui(new Ui::LineItem)
{
  ui->setupUi(this);
  SetItemType(ListItemType::LINE_ITEM);
}

LineItem::~LineItem()
{
  delete ui;
}

QSize LineItem::sizeHint() const
{
  return QSize(250,5); // 返回自定义的尺寸
}
