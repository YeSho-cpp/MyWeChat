//
// Created by YeSho on 2024/9/26.
//

#ifndef WECHAT_CHATVIEW_H
#define WECHAT_CHATVIEW_H

#include <QScrollArea>
#include <QVBoxLayout>
#include <QTimer>

class ChatView : public QWidget
{
  Q_OBJECT
public:
  explicit ChatView(QWidget *parent = Q_NULLPTR);
  void appendChatItem(QWidget *item);                 //尾插
  void prependChatItem(QWidget *item);                //头插
  void insertChatItem(QWidget *before, QWidget *item);//中间插
  void removeAllItem();
protected:
  bool eventFilter(QObject *o, QEvent *e) override;
  void paintEvent(QPaintEvent *event) override;
private slots:
  void onVScrollBarMoved(int min, int max);

private:
  void initStyleSheet();
private:
  //QWidget *m_pCenterWidget;
  QVBoxLayout *m_pVl; // 管理聊天项的垂直排列
  QScrollArea *m_pScrollArea; // 滚动区域控件，是整个聊天视图的核心部分
  bool isAppended; // 用于指示是否刚刚添加了新的聊天项。

};


#endif//WECHAT_CHATVIEW_H
