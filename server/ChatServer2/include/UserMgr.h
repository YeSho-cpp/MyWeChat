//
// Created by YeSho on 2024/9/24.
//

#ifndef CHATSERVER_USERMGR_H
#define CHATSERVER_USERMGR_H


#include "Singleton.h"
#include <unordered_map>
#include <memory>
#include <mutex>

class CSession;
class UserMgr: public Singleton<UserMgr>
{
  friend class Singleton<UserMgr>;
public:
  ~UserMgr();
  std::shared_ptr<CSession> GetSession(int uid);
  void SetUserSession(int uid, std::shared_ptr<CSession> session);
  void RmvUserSession(int uid);
private:
  UserMgr();
  std::mutex _session_mtx;
  std::unordered_map<int, std::shared_ptr<CSession>> _uid_to_session;
};


#endif//CHATSERVER_USERMGR_H
