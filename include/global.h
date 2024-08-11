//
// Created by YeSho on 2024/8/11.
//

#ifndef WECHAT_GLOBAL_H
#define WECHAT_GLOBAL_H

/*
 * 一些常用头文件放在这里
 */

#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include <iostream>
#include <QByteArray>
#include "QStyle"


// 我们想让qss属性切换生效
extern std::function<void(QWidget*)> repolish;

// 功能id
enum ReqId{
  ID_GET_VARIFY_CODE =1001, // 获取验证码
  ID_REG_USER = 1002, // 注册用户
};

enum Modules{
  REGISTERMOD = 0,
};

enum ErrorCode{
  SUCCESS =0,
  ERR_JSON = 1, // json 解析失败
  ERR_NETWORK =2, // 网络错误
};

#endif//WECHAT_GLOBAL_H
