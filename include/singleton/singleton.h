#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>

template <typename T>
class Singleton {
 public:
  static T& getInstance() {
    static T instance;  // 使用局部静态变量
    return instance;
  }

  // 防止复制构造函数和赋值运算符被调用
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

 private:
  // 私有构造函数，防止外部实例化
  Singleton() {}
};

#endif