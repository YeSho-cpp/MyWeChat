//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_PICTUREBUBBLE_H
#define WECHAT_PICTUREBUBBLE_H


#include "BubbleFrame.h"
#include <QHBoxLayout>
#include <QPixmap>
class PictureBubble : public BubbleFrame
{
  Q_OBJECT
public:
  PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent = nullptr);
};


#endif//WECHAT_PICTUREBUBBLE_H
