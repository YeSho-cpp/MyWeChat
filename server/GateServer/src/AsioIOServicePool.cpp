//
// Created by YeSho on 2024/9/12.
//

#include "AsioIOServicePool.h"

#include <iostream>

AsioIOServicePool::AsioIOServicePool(std::size_t size) : _ioServices(size),_works(size), _nextIOService(0) {
  for (std::size_t i = 0; i < size; ++i) {
    _works[i] = std::make_unique<Work>(_ioServices[i]);
  }

  //遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
  for (auto & _ioService : _ioServices) {
    _threads.emplace_back([this, &_ioService]() {
      _ioService.run(); // 有了前面的work，这里就不会直接返回了
    });
  }
}
AsioIOServicePool::~AsioIOServicePool() {
  Stop();
  std::cout << "AsioIOServicePool destruct" << std::endl;
}
boost::asio::io_context &AsioIOServicePool::GetIOService() {
  _nextIOService=(_nextIOService+1)%_ioServices.size();
  auto& service=_ioServices[_nextIOService];
  return service;
}
void AsioIOServicePool::Stop() {
  //因为仅仅执行work.reset并不能让iocontext从run的状态中退出
  //当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
  for (auto& work:_works) {
    //把服务先停止
    work->get_io_context().stop();
    work.reset();
  }

  for(auto& t:_threads){
    t.join();
  }
}
