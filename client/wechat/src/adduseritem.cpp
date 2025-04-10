//
// Created by YeSho on 2024/9/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddUserItem.h" resolved

#include "adduseritem.h"
#include "ui_adduseritem.h"

AddUserItem::AddUserItem(QWidget *parent) :
                                            ListItemBase(parent),
                                            ui(new Ui::AddUserItem)
{
  ui->setupUi(this);
  SetItemType(ListItemType::ADD_USER_TIP_ITEM);
}

AddUserItem::~AddUserItem()
{
  delete ui;
}
