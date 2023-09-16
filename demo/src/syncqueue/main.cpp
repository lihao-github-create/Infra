#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "syncqueue/fixed_syncqueue.h"

int main() {
  FixedSyncQueue<int> queue(100u);  // 设置最大容量为100

  // 创建生产者线程
  std::thread producer([&]() {
    for (int i = 1; i <= 1000; ++i) {
      queue.Push(i);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  });

  // 创建消费者线程
  std::thread consumer([&]() {
    for (int i = 1; i <= 1000; ++i) {
      int value;
      if (queue.Pop(value, std::chrono::milliseconds(500))) {
        std::cout << "Consumed: " << value << std::endl;
      } else {
        std::cout << "Timeout!" << std::endl;
      }
    }
  });

  producer.join();
  consumer.join();

  return 0;
}
