//
// Created by YeSho on 2024/8/11.
//

#include "../include/global.h"

// 用于更新控件的外观，例如当控件的状态改变时，需要刷新其视觉表现。
std::function<void(QWidget *)> repolish =[](QWidget *w){
  w->style()->unpolish(w);
  w->style()->polish(w);
};