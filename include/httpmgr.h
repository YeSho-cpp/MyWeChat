//
// Created by YeSho on 2024/8/11.
//

#ifndef WECHAT_HTTPMGR_H
#define WECHAT_HTTPMGR_H

#include "singleton.h"
#include <QString>
#include <QUrl>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

class HttpMgr : public QObject,public Singleton<HttpMgr>{ // 这里继承了QObject才可以使用信号和槽机制
  Q_OBJECT
public:
  ~HttpMgr();
private:
  friend class Singleton<HttpMgr>; // 因为在基类构造所以要给访问的权限，便要定义友元
  HttpMgr();
  QNetworkAccessManager _manager; // qt原生的网络的一个网络管理者
  void PostHttpReq(QUrl url,const QJsonObject& json, ReqId reqId,Modules mod);
signals:
  void sig_http_finish(ReqId id,QString res,ErrorCode err, Modules mod); // 一个http发送完之后，会发送这样的一个信号，通知其他的模块
};



#endif//WECHAT_HTTPMGR_H
