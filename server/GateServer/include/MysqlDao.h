//
// Created by YeSho on 2024/9/13.
//

#ifndef GATESERVER_MYSQLDAO_H
#define GATESERVER_MYSQLDAO_H

#include <thread>
#include <utility>
#include "const.h"


#include <mysql-cppconn/jdbc/mysql_driver.h>
#include <mysql-cppconn/jdbc/mysql_connection.h>
#include <mysql-cppconn/jdbc/cppconn/prepared_statement.h>
#include <mysql-cppconn/jdbc/cppconn/resultset.h>
#include <mysql-cppconn/jdbc/cppconn/statement.h>
#include <mysql-cppconn/jdbc/cppconn/exception.h>

// SqlConnection 类表示一个 SQL 连接
class SqlConnection {
public:
  // 构造函数，接受一个 SQL 连接指针和最后操作时间
  SqlConnection(sql::Connection* con, int64_t lasttime);

  // 存储 SQL 连接的智能指针
  std::unique_ptr<sql::Connection> _con;

  // 记录最后一次操作的时间戳
  int64_t _last_oper_time;
};

// MySqlPool 类实现了一个 MySQL 连接池
class MySqlPool {
public:
  // 构造函数，初始化连接池
  // 参数包括数据库 URL，用户名，密码，数据库名，以及池的大小
  MySqlPool(std::string url, std::string user, std::string pass, std::string schema, int poolSize);

  // 从连接池中获取一个连接
  std::unique_ptr<SqlConnection> getConnection();

  // 将使用完的连接归还到连接池
  void returnConnection(std::unique_ptr<SqlConnection> con);

  // 关闭连接池
  void Close();

  void checkConnection();

  // 析构函数，用于清理资源
  ~MySqlPool();



private:
  std::string url_;      // 数据库 URL
  std::string user_;     // 数据库用户名
  std::string pass_;     // 数据库密码
  std::string schema_;   // 数据库名
  int poolSize_;         // 连接池大小

  // 存储空闲连接的队列
  std::queue<std::unique_ptr<SqlConnection>> pool_;

  std::mutex mutex_;              // 用于同步的互斥锁
  std::condition_variable cond_;  // 条件变量，用于线程间通信

  // 原子布尔值，用于指示连接池是否应该停止
  std::atomic<bool> b_stop_;
  std::thread _check_thread; // 检测线程 心跳机制
};

struct UserInfo{
  std::string name;
  std::string pwd;
  int uid;
  std::string email;
};

class MysqlDao
{
public:
  MysqlDao();
  ~MysqlDao();
  int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
  bool CheckEmail(const std::string& name, const std::string & email);
  bool UpdatePwd(const std::string& name, const std::string& newpwd);
  bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo);
private:
  std::unique_ptr<MySqlPool> pool_;
};

#endif//GATESERVER_MYSQLDAO_H
