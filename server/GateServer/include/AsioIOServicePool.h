//
// Created by YeSho on 2024/9/12.
//

#ifndef GATESERVER_ASIOIOSERVICEPOOL_H
#define GATESERVER_ASIOIOSERVICEPOOL_H

#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"

class AsioIOServicePool : public Singleton<AsioIOServicePool>{
  friend Singleton<AsioIOServicePool>;
public:
  using IOService = boost::asio::io_context;
  using Work = boost::asio::io_context::work; // 用于防止 io_context 在没有待处理的工作时退出。
  using WorkPtr = std::unique_ptr<Work>;
  ~AsioIOServicePool();
  AsioIOServicePool(const AsioIOServicePool&)=delete;
  AsioIOServicePool& operator=(const AsioIOServicePool&)=delete;
  // 使用 round-robin 的方式返回一个 io_service
  boost::asio::io_context& GetIOService();
  void Stop();
private:
  explicit AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency() /*std::thread::hardware_concurrency()*/);
  std::vector<IOService> _ioServices;
  std::vector<WorkPtr> _works;
  std::vector<std::thread> _threads;
  std::size_t  _nextIOService; // 用于跟踪下一个应该被使用的 io_context 的索引。
};


#endif//GATESERVER_ASIOIOSERVICEPOOL_H
