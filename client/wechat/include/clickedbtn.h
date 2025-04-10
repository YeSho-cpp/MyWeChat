//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_CLICKEDBTN_H
#define WECHAT_CLICKEDBTN_H
#include <QPushButton>

class ClickedBtn:public QPushButton
{
  Q_OBJECT
public:
  explicit ClickedBtn(QWidget * parent = nullptr);
  ~ClickedBtn() override;
  void SetState(const QString& nomal, QString hover, QString press);
protected:
  void enterEvent(QEnterEvent *event) override ; // 鼠标进入
  void leaveEvent(QEvent *event) override;// 鼠标离开
  void mousePressEvent(QMouseEvent *event) override; // 鼠标按下
  void mouseReleaseEvent(QMouseEvent *event) override; // 鼠标释放
private:
  QString _normal; // 正常
  QString _hover; // 悬停
  QString _press; // 按下
};

#endif//WECHAT_CLICKEDBTN_H
