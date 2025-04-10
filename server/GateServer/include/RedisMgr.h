//
// Created by YeSho on 2024/9/12.
//

#ifndef GATESERVER_REDISMGR_H
#define GATESERVER_REDISMGR_H
#include "const.h"

class RedisConPool{
public:
  RedisConPool(size_t poolSize, const char* host, int port, const char* pwd);
  ~RedisConPool();
  redisContext* getConnection();
  void returnConnection(redisContext* context);
  void Close();
  void ClearConnections();
private:
  std::atomic<bool>b_stop_;
  size_t poolSize_;
  const char* host_;
  int port_;
  std::queue<redisContext*>connections_;
  std::mutex mutex_;
  std::condition_variable cond_;
};


class RedisMgr: public Singleton<RedisMgr>,public std::enable_shared_from_this<RedisMgr>{
  friend class Singleton<RedisMgr>;
public:
  ~RedisMgr();
  // bool Connect(const std::string& host, int port); 已经有redis连接池，不需要这个了
  bool Get(const std::string &key, std::string& value);
  bool Set(const std::string &key, const std::string &value);
  bool Auth(const std::string &password);
  bool LPush(const std::string &key, const std::string &value);
  bool LPop(const std::string &key, std::string& value);
  bool RPush(const std::string& key, const std::string& value);
  bool RPop(const std::string& key, std::string& value);
  bool HSet(const std::string &key, const std::string  &hkey, const std::string &value);
  bool HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen);
  std::string HGet(const std::string &key, const std::string &hkey);
  bool Del(const std::string &key);
  bool ExistsKey(const std::string &key);
  void Close();
private:
  RedisMgr();
  std::unique_ptr<RedisConPool>  _con_pool;
};




#endif//GATESERVER_REDISMGR_H
