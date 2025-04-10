//
// Created by YeSho on 2024/9/26.
//

#include "searchlist.h"
#include "adduseritem.h"
#include "customizeedit.h"
#include "findfaildlg.h"
#include "findsuccessdlg.h"
#include "invaliditem.h"
#include "loadingdlg.h"
#include "tcpmgr.h"
#include "userdata.h"
#include "usermgr.h"
#include <QJsonDocument>
#include <QScrollBar>

SearchList::SearchList(QWidget *parent):QListWidget(parent),_find_dlg(nullptr), _search_edit(nullptr), _send_pending(false)
{
  Q_UNUSED(parent);
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  // 安装事件过滤器
  this->viewport()->installEventFilter(this);
  //连接点击的信号和槽
  connect(this, &QListWidget::itemClicked, this, &SearchList::slot_item_clicked);
  //添加条目
  addTipItem();
  //连接搜索条目
  connect(TcpMgr::getInstance().get(), &TcpMgr::sig_user_search, this, &SearchList::slot_user_search);
}

void SearchList::CloseFindDlg()
{
  if(_find_dlg){
    _find_dlg->hide();
    _find_dlg = nullptr;
  }
}

void SearchList::SetSearchEdit(QWidget* edit) {
  _search_edit = edit;
}

void SearchList::waitPending(bool pending)
{
  if(pending){
    _loadingDialog = new LoadingDlg(this);
    _loadingDialog->setModal(true); // 将对话框设置为模态（modal），这意味着它会阻止用户与主窗口的其他部分交互。
    _loadingDialog->show();
    _send_pending = pending;
  }else{
    _loadingDialog->hide();
    _loadingDialog->deleteLater(); // 安全的方式来删除 Qt
    _send_pending = pending;
  }
}


void SearchList::addTipItem()
{
  auto *invalid_item = new QWidget();
  auto *item_tmp = new QListWidgetItem;
  //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
  item_tmp->setSizeHint(QSize(250,10));
  this->addItem(item_tmp);
  invalid_item->setObjectName("invalid_item");
  this->setItemWidget(item_tmp, invalid_item);
  item_tmp->setFlags(item_tmp->flags() & ~Qt::ItemIsSelectable);


  auto *add_user_item = new AddUserItem();
  auto *item = new QListWidgetItem;
  //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
  item->setSizeHint(add_user_item->sizeHint());
  this->addItem(item);
  this->setItemWidget(item, add_user_item);
}

void SearchList::slot_item_clicked(QListWidgetItem *item)
{
  QWidget *widget = this->itemWidget(item); // 获取与被点击的列表项关联的自定义部件。
  if(!widget){
    qDebug()<< "slot item clicked widget is nullptr";
    return;
  }

  // 对自定义widget进行操作， 将item 转化为基类ListItemBase
  auto *customItem = qobject_cast<ListItemBase*>(widget);
  if(!customItem){
    qDebug()<< "slot item clicked widget is nullptr";
    return;
  }

  auto itemType = customItem->GetItemType();
  if(itemType == ListItemType::INVALID_ITEM){
    qDebug()<< "slot invalid item clicked ";
    return;
  }

  if(itemType == ListItemType::ADD_USER_TIP_ITEM){

    // 如果正在发送或搜索编辑框不存在，直接返回。
    if (_send_pending) {
      return;
    }

    if (!_search_edit) {
      return;
    }
    waitPending(true);
    auto search_edit = dynamic_cast<CustomizeEdit*>(_search_edit);
    auto uid_str = search_edit->text();

    //此处发送请求给server
    QJsonObject jsonObj;
    jsonObj["uid"] = uid_str;

    QJsonDocument doc(jsonObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    //发送tcp请求给chat server
    emit TcpMgr::getInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ, jsonData);
    return;
  }

  //清除弹出框
  CloseFindDlg();
}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
  waitPending(false);
  if (si == nullptr) {
    _find_dlg = std::make_shared<FindFailDlg>(this);
  }else{
    //如果是自己，暂且先直接返回，以后看逻辑扩充
    auto self_uid = UserMgr::getInstance()->GetUid();
    if (si->_uid == self_uid) {
      return;
    }
    //此处分两种情况，一种是搜多到已经是自己的朋友了，一种是未添加好友
    //查找是否已经是好友
    bool bExist = UserMgr::getInstance()->CheckFriendById(si->_uid);
    if(bExist){
      //此处处理已经添加的好友，实现页面跳转
      //跳转到聊天界面指定的item中
      emit sig_jump_chat_item(si);
      return;
    }
    //此处先处理为添加的好友
    _find_dlg = std::make_shared<FindSuccessDlg>(this);
    std::dynamic_pointer_cast<FindSuccessDlg>(_find_dlg)->SetSearchInfo(si);

  }
  _find_dlg->show();
}

