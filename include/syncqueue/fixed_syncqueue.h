#include <chrono>
#include <condition_variable>
#include <limits>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class FixedSyncQueue {
 public:
  FixedSyncQueue(size_t max_capacity = std::numeric_limits<size_t>::max())
      : max_capacity_(max_capacity) {}

  // 向队列中添加元素
  // 向队列中添加元素，如果队列已满，执行超时策略
  bool Push(const T& value, std::chrono::milliseconds timeout =
                                std::chrono::milliseconds(1000u)) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.size() >= max_capacity_) {
      // 如果队列已满，等待一段时间，然后再尝试添加
      if (!condition_push_.wait_for(lock, timeout, [this] {
            return queue_.size() < max_capacity_;
          })) {
        // 超时
        return false;
      }
    }
    queue_.push(value);
    lock.unlock();
    condition_pop_.notify_one();
    return true;
  }

  // 从队列中取出元素，如果超时，返回false
  bool Pop(T& value, std::chrono::milliseconds timeout =
                         std::chrono::milliseconds(100u)) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (condition_pop_.wait_for(lock, timeout,
                                [this] { return !queue_.empty(); })) {
      value = queue_.front();
      queue_.pop();
      lock.unlock();
      condition_push_.notify_one();
      return true;
    }
    return false;
  }

  // 检查队列是否为空
  bool Empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
  }

 private:
  std::queue<T> queue_;
  mutable std::mutex mutex_;
  std::condition_variable condition_push_;
  std::condition_variable condition_pop_;
  size_t max_capacity_;
};