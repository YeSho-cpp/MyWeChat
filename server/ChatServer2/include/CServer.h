//
// Created by YeSho on 2024/9/24.
//

#ifndef CHATSERVER_CSERVER_H
#define CHATSERVER_CSERVER_H


#include <boost/asio.hpp>
#include "CSession.h"
#include <memory.h>
#include <map>
#include <mutex>
using namespace std;
using boost::asio::ip::tcp;
class CServer
{
public:
  CServer(boost::asio::io_context& io_context, short port);
  ~CServer();
  void ClearSession(const std::string&);
private:
  void HandleAccept(const shared_ptr<CSession>&, const boost::system::error_code & error);
  void StartAccept();
  boost::asio::io_context &_io_context;
  short _port;
  tcp::acceptor _acceptor;
  std::map<std::string, shared_ptr<CSession>> _sessions;
  std::mutex _mutex;
};

#endif//CHATSERVER_CSERVER_H
