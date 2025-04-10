//
// Created by YeSho on 2024/9/26.
//

#include "clickedbtn.h"

#include <utility>
#include "global.h"

ClickedBtn::ClickedBtn(QWidget *parent):QPushButton (parent)
{
  setCursor(Qt::PointingHandCursor); // 设置光标为小手
}

ClickedBtn::~ClickedBtn()= default;

void ClickedBtn::SetState(const QString& normal, QString hover, QString press)
{
  _hover = std::move(hover);
  _normal = normal;
  _press = std::move(press);
  setProperty("state",normal);
  repolish(this);
  update();
}

void ClickedBtn::enterEvent(QEnterEvent *event)
{
  setProperty("state",_hover);
  repolish(this);
  update();
  QPushButton::enterEvent(event);
}

void ClickedBtn::mousePressEvent(QMouseEvent *event)
{
  setProperty("state",_press);
  repolish(this);
  update();
  QPushButton::mousePressEvent(event);
}

void ClickedBtn::mouseReleaseEvent(QMouseEvent *event)
{
  setProperty("state",_hover);
  repolish(this); // 确保新的样式被应用到按钮上。
  update(); // 然后触发重绘，以便新的样式变化可以在视觉上显示出来
  QPushButton::mouseReleaseEvent(event);
}

void ClickedBtn::leaveEvent(QEvent *event)
{
  setProperty("state",_normal);
  repolish(this);
  update();
  QPushButton::leaveEvent(event);
}

