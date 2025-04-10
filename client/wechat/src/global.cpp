//
// Created by YeSho on 2024/8/11.
//

#include "../include/global.h"

QString gate_url_prefix="";

// 用于更新控件的外观，例如当控件的状态改变时，需要刷新其视觉表现。
std::function<void(QWidget *)> repolish =[](QWidget *w){
  w->style()->unpolish(w);
  w->style()->polish(w);
};

// 简单的字符串加密/解密算法
std::function<QString(QString)> xorString = [](QString input){
  QString result = input; // 复制原始字符串，以便进行修改
  int length = input.length(); // 获取字符串的长度
  ushort xor_code = length % 255;
  for (int i = 0; i < length; ++i) {
    // 对每个字符进行异或操作
    // 注意：这里假设字符都是ASCII，因此直接转换为QChar
    result[i] = QChar(static_cast<ushort>(input[i].unicode() ^ xor_code));
  }
  return result;
};
