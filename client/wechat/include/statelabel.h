//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_STATELABEL_H
#define WECHAT_STATELABEL_H

#include <QLabel>
#include "global.h"
#include <QMouseEvent>

class StateLabel : public QLabel
{
  Q_OBJECT
public:
  explicit StateLabel(QWidget* parent = nullptr);
  virtual void mousePressEvent(QMouseEvent *ev) override;
  virtual void mouseReleaseEvent(QMouseEvent *ev) override;
  virtual void enterEvent(QEnterEvent* event) override;
  virtual void leaveEvent(QEvent* event) override;
  void SetState(const QString& normal="", QString hover="", QString press="",
                QString select="", QString select_hover="", QString select_press="");

  ClickLbState GetCurState();
  void ClearState();

  void SetSelected(bool bselected);
protected:

private:
  QString _normal;
  QString _normal_hover;
  QString _normal_press;

  QString _selected;
  QString _selected_hover;
  QString _selected_press;

  ClickLbState _curstate;

signals:
  void clicked(void);
};

#endif//WECHAT_STATELABEL_H
