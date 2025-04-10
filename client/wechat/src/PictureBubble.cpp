//
// Created by YeSho on 2024/9/26.
//

#include "PictureBubble.h"
#include <QLabel>


#define PIC_MAX_WIDTH 160
#define PIC_MAX_HEIGHT 90

PictureBubble::PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent)
    :BubbleFrame(role, parent)
{
  auto *lb = new QLabel();
  lb->setScaledContents(true); // 允许图片内容缩放以适应标签大小。

  // 将输入的图片缩放到预定义的最大尺寸（PIC_MAX_WIDTH 和 PIC_MAX_HEIGHT），保持宽高比。
  QPixmap pix = picture.scaled(QSize(PIC_MAX_WIDTH, PIC_MAX_HEIGHT), Qt::KeepAspectRatio);

  lb->setPixmap(pix); // 将缩放后的图片设置到标签中。

  this->setWidget(lb);

  int left_margin = this->layout()->contentsMargins().left();
  int right_margin = this->layout()->contentsMargins().right();
  int v_margin = this->layout()->contentsMargins().bottom();
  setFixedSize(pix.width()+left_margin + right_margin, pix.height() + v_margin *2);
}

