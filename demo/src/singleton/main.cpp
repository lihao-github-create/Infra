#include <iostream>

#include "singleton/singleton.h"

// 示例使用：定义一个类并使用Singleton模板创建单例对象
class MySingleton {
 public:
  void doSomething() {
    std::cout << "MySingleton is doing something." << std::endl;
  }

  // 构造函数和析构函数设置为私有，以防止外部实例化
 private:
  MySingleton() { std::cout << "MySingleton constructor called." << std::endl; }

  ~MySingleton() { std::cout << "MySingleton destructor called." << std::endl; }

  // Singleton 模板类作为友元，以便访问私有构造函数
  friend class Singleton<MySingleton>;
};

int main() {
  // 获取MySingleton的单例实例
  MySingleton& singleton = Singleton<MySingleton>::getInstance();

  // 使用单例对象执行操作
  singleton.doSomething();

  return 0;
}