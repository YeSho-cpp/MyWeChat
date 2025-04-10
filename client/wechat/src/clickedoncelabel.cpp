//
// Created by YeSho on 2024/9/26.
//

#include "clickedoncelabel.h"

ClickedOnceLabel::ClickedOnceLabel(QWidget *parent):QLabel(parent)
{
  setCursor(Qt::PointingHandCursor); // 设置鼠标悬停在标签上时的光标样式为指向手型
}


void ClickedOnceLabel::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    emit clicked(this->text());
    return;
  }
  // 调用基类的mousePressEvent以保证正常的事件处理
  QLabel::mousePressEvent(event);
}


