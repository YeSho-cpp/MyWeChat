//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_INVALIDITEM_H
#define WECHAT_INVALIDITEM_H


#include <QWidget>
#include "listitembase.h"

class invaliditem : public ListItemBase
{
  Q_OBJECT
public:
  explicit invaliditem(QWidget *parent = nullptr);
  QSize sizeHint() const override {
    return QSize(250,10); // 返回自定义的尺寸
  }
signals:

public slots:
};


#endif//WECHAT_INVALIDITEM_H
