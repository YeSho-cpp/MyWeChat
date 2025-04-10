//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_CUSTOMIZEEDIT_H
#define WECHAT_CUSTOMIZEEDIT_H

#include <QLineEdit>
#include <QDebug>

class CustomizeEdit: public QLineEdit
{
  Q_OBJECT
public:
  explicit CustomizeEdit(QWidget *parent = nullptr);
  void SetMaxLength(int maxLen);
protected:
  void focusOutEvent(QFocusEvent *event) override
  {
    // 执行失去焦点时的处理逻辑
    //qDebug() << "CustomizeEdit focusout";
    // 调用基类的focusOutEvent()方法，保证基类的行为得到执行
    QLineEdit::focusOutEvent(event);
    //发送失去焦点得信号
    emit sig_foucus_out();
  }
private:
  void limitTextLength(const QString& text) {
    if(_max_len <= 0){
      return;
    }

    QByteArray byteArray = text.toUtf8();

    if (byteArray.size() > _max_len) {
      byteArray = byteArray.left(_max_len);
      this->setText(QString::fromUtf8(byteArray));
    }
  }

  int _max_len;
signals:
  void sig_foucus_out();
};


#endif//WECHAT_CUSTOMIZEEDIT_H
