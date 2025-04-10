//
// Created by YeSho on 2024/8/11.
//

#ifndef WECHAT_SINGLETON_H
#define WECHAT_SINGLETON_H

#include "global.h"

/*
 * 这是一个单例的类 它一定是不允许被拷贝和赋值构造的
 * 然后我们要有一个这样的实例，那么我们一定不希望手动回收，因此用智能指针类型‘
 * 我们暴露一个对外的静态方法得到这个类
 */
template<typename T>
class Singleton{
protected:
  Singleton() =default;
  Singleton(const Singleton<T>&)=delete;
  Singleton& operator=(const Singleton<T>& st)=delete;
public:
  //todo 这里采用了c++11 的简化静态初始化方法
  static std::shared_ptr<T>& getInstance(){
    static std::shared_ptr<T> _instance(new T());
    return _instance;
  }

  void PrintAddress(){
    std::cout<<this->getInstance().get()<<std::endl;
  }

  ~Singleton(){
    std::cout<<"this is singleton destruct"<<std::endl;
  }

};


#endif//WECHAT_SINGLETON_H
