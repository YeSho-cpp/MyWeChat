//
// Created by YeSho on 2024/8/17.
//

#include "CServer.h"
#include "AsioIOServicePool.h"
CServer::CServer(boost::asio::io_context &ioc, unsigned short &port):_ioc(ioc),
 _acceptor(ioc,tcp::endpoint(tcp::v4(),port)){

}
void CServer::start() {
  // 我们首先要做一个监听的函数，对端来连接了，就要接收这个连接

  auto self =shared_from_this(); // 防止回调会被析构掉
  auto& io_context=AsioIOServicePool::getInstance()->GetIOService();
  auto new_con=std::make_shared<HttpConnection>(io_context);
  _acceptor.async_accept(new_con->GetSocket(),[self,new_con](beast::error_code ec){
    try{
        // 出错了将放弃这个连接，去监听其他的连接
        if(ec){
          self->start();
          return;
        }
        // 如果没有出错，我们用来一个连接管理类来处理这个socket上的读事件和写事件
        new_con->Start();


        // 继续监听
        self->start();
    }catch (std::exception& exp){
      std::cout << "exception is " << exp.what() << std::endl;
      self->start();
    }
  });
}
