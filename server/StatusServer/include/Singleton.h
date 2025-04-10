//
// Created by YeSho on 2024/9/15.
//

#ifndef STATUSSERVER_SINGLETON_H
#define STATUSSERVER_SINGLETON_H

#include <memory>
#include <mutex>
#include <iostream>
using namespace std;

template <typename T>
class Singleton {
protected:
  Singleton() = default;
  Singleton(const Singleton<T>&) = delete;
  Singleton& operator=(const Singleton<T>& st) = delete;

  static std::shared_ptr<T> _instance;
public:
  static std::shared_ptr<T> GetInstance() {
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() {
      _instance = shared_ptr<T>(new T);
    });

    return _instance;
  }
  void PrintAddress() {
    std::cout << _instance.get() << endl;
  }
  ~Singleton() {
    std::cout << "this is singleton destruct" << std::endl;
  }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;
#endif // STATUSSERVER_SINGLETON_H
