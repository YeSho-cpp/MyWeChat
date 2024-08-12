//
// Created by YeSho on 2024/8/11.
//

#include "../include/httpmgr.h"

HttpMgr::HttpMgr() {
  // 信号与槽函数绑定
  connect(this,&HttpMgr::sig_http_finish,this,&HttpMgr::slot_http_finish);
}
HttpMgr::~HttpMgr() {

}
void HttpMgr::PostHttpReq(const QUrl& url, const QJsonObject& json, ReqId reqId, Modules mod) {
  // 我们要发送http请求，这个数据要序列化为一个字符串
  QByteArray data = QJsonDocument(json).toJson();
  QNetworkRequest request(url);
  // 设置一些请求头的参数
  request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
  request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(data.length())); // 这里有大端和小端之说
  auto self = shared_from_this();
  QNetworkReply *reply = _manager.post(request,data); // 把request 以及data post出去
  QObject::connect(reply,&QNetworkReply::finished,[self,reply,reqId,mod](){
    // 处理错误情况
    if(reply->error()!=QNetworkReply::NoError){
      qDebug()<<reply->errorString();
      // 发送信号通知完成
      emit self->sig_http_finish(reqId,"",ErrorCode::ERR_NETWORK,mod);
    }
    else{
      // 无错误
      QString res=reply->readAll();
      // 发送信号通知完成
      emit self->sig_http_finish(reqId,res,ErrorCode::SUCCESS,mod);
    }
    reply->deleteLater(); //  不会立即删除对象，而是安排对象在稍后的某个时刻被删除。
    return;
  });



}
void HttpMgr::slot_http_finish(ReqId id, const QString& res, ErrorCode err, Modules mod) {
    // 这里有注册模块
    if(mod==Modules::REGISTERMOD){
      // 发送信号通知指定模块http的响应结束了
      emit sig_reg_mod_finish(id,res,err);
    }

}
void HttpMgr::sig_reg_mod_finish(ReqId id, const QString& res, ErrorCode err) {

}
