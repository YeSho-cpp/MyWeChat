//
// Created by YeSho on 2024/9/13.
//

#ifndef GATESERVER_MYSQLMGR_H
#define GATESERVER_MYSQLMGR_H

#include "const.h"
#include "MysqlDao.h"

class MysqlMgr : public Singleton<MysqlMgr>{
  friend class Singleton<MysqlMgr>;
public:
  ~MysqlMgr();
  int RegUser(const std::string& name, const std::string& email,  const std::string& pwd);
  bool CheckEmail(const std::string& name, const std::string& email);
  bool UpdatePwd(const std::string& name, const std::string& pwd);
  bool CheckPwd(const std::string& email,const std::string& pwd,UserInfo& userInfo);
private:
  MysqlMgr();
  MysqlDao _dao;
};


#endif//GATESERVER_MYSQLMGR_H
