//
// Created by YeSho on 2024/9/26.
//

#include "invaliditem.h"

invaliditem::invaliditem(QWidget *parent) : ListItemBase(parent)
{
  SetItemType(ListItemType::INVALID_ITEM);
}

