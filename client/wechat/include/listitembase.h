//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_LISTITEMBASE_H
#define WECHAT_LISTITEMBASE_H


#include <QWidget>
#include "global.h"

class ListItemBase : public QWidget
{
  Q_OBJECT
public:
  explicit ListItemBase(QWidget *parent = nullptr);
  void SetItemType(ListItemType itemType);

  ListItemType GetItemType();
protected:
  void paintEvent(QPaintEvent* event) override;
private:
  ListItemType _itemType;

public slots:

signals:


};

#endif//WECHAT_LISTITEMBASE_H
