//
// Created by YeSho on 2024/9/24.
//

#ifndef CHATSERVER_MSGNODE_H
#define CHATSERVER_MSGNODE_H

#include <string>
#include "const.h"
#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;
class LogicSystem;
class MsgNode
{
public:
  explicit MsgNode(short max_len) :_total_len(max_len), _cur_len(0) {
    _data = new char[_total_len + 1]();
    _data[_total_len] = '\0';
  }

  ~MsgNode() {
    std::cout << "destruct MsgNode" << endl;
    delete[] _data;
  }

  void Clear() {
    ::memset(_data, 0, _total_len);
    _cur_len = 0;
  }

  short _cur_len; // 当前已接收/发送的长度。
  short _total_len; //  消息的总长度。
  char* _data; // 指向存储消息内容的字符数组。
};

class RecvNode :public MsgNode {
  friend class LogicSystem;
public:
  RecvNode(short max_len, short msg_id);
private:
  short _msg_id;
};

class SendNode:public MsgNode {
  friend class LogicSystem;
public:
  SendNode(const char* msg,short max_len, short msg_id);
private:
  short _msg_id;
};


#endif//CHATSERVER_MSGNODE_H
