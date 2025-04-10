//
// Created by YeSho on 2024/9/24.
//

#include "AsioIOServicePool.h"

#include <iostream>
#include <memory>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size):_ioServices(size),
                                                         _works(size), _nextIOService(0){
  for (std::size_t i = 0; i < size; ++i) {
    _works[i] = std::make_unique<Work>(_ioServices[i]); // 为每个 io_context 创建一个 work 对象。
  }

  //遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
  for (auto & _ioService : _ioServices) {
    _threads.emplace_back([this, &_ioService]() {
      _ioService.run(); // 没有 work 对象，当 io_context 中没有待处理的异步操作时，io_context::run() 方法会立即返回。
    });
  }
}

AsioIOServicePool::~AsioIOServicePool() {
  std::cout << "AsioIOServicePool destruct" << endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
  auto& service = _ioServices[_nextIOService++];
  if (_nextIOService == _ioServices.size()) {
    _nextIOService = 0;
  }
  return service;
}

void AsioIOServicePool::Stop(){
  //因为仅仅执行work.reset并不能让iocontext从run的状态中退出
  //当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
  for (auto& work : _works) {
    //把服务先停止
    work->get_io_context().stop(); // 立即停止 io_context，取消所有未完成的异步操作，是一个"硬"停止，不等待当前正在执行的操作完成。
    work.reset(); // 这是一个"软"停止。
  }

  for (auto& t : _threads) {
    t.join();
  }
}
