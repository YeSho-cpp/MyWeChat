//
// Created by YeSho on 2024/8/11.
//

#include "../include/httpmgr.h"

HttpMgr::HttpMgr() {

}
HttpMgr::~HttpMgr() {

}
void HttpMgr::PostHttpReq(QUrl url, const QJsonObject& json, ReqId reqId, Modules mod) {
  // 我们要发送http请求，这个数据要序列化为一个字符串
  QByteArray data = QJsonDocument(json).toJson();
  QNetworkRequest request(url);
  // 设置一些请求头的参数
  request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
  request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(data.length())); // 这里有大端和小端之说



}
