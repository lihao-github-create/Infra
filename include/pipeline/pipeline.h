#ifndef PIPELINE_H
#define PIPELINE_H

/**
 * @file pipeline.h
 *
 * @brief This is a brief description of the contents of this header file.
 *
 * 流水线模板，用于加速可阶段化的任务
 *
 * note: DataItem中data对象内存申请和释放由使用者负责
 */

// Include any necessary headers here
#include <atomic>
#include <chrono>  // For std::chrono::milliseconds
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "syncqueue/fixed_syncqueue.h"
// Declare your classes, functions, and variables here

// Define a structure to represent items in the data stream
struct DataItem {
  void* data;
  // Add any additional data fields here if needed
};

class PipelineStage {
 public:
  PipelineStage(std::function<void(DataItem&)> process_func,
                size_t max_queue_size, std::chrono::milliseconds timeout)
      : process_func_(process_func), data_queue_(max_queue_size) {}

  void SetNextStage(PipelineStage* next_stage) { next_stage_ = next_stage; }

  bool PushData(const DataItem& item) { return data_queue_.Push(item); }

  void Process() {
    if (!process_func_) {
      std::cout << "未设置 process_func_" << std::endl;
    }
    while (true) {
      if (is_finished_) {
        break;
      }

      DataItem item;
      // Simulate some processing in the stage
      if (data_queue_.Pop(item)) {
        process_func_(item);

        // Notify the next stage that data is available
        if (next_stage_) {
          next_stage_->PushData(item);
        }
      }
    }
  }

  void Finish() { is_finished_ = true; }

 private:
  std::function<void(DataItem&)> process_func_;
  PipelineStage* next_stage_ = nullptr;
  FixedSyncQueue<DataItem> data_queue_;
  std::atomic<bool> is_finished_{false};
};

class Pipeline {
 public:
  Pipeline(int num_stages) : num_stages_(num_stages) {
    stages_.resize(num_stages_);
    threads_.reserve(num_stages_);
  }

  void AddStage(int stage_index, std::function<void(DataItem&)> process_func,
                size_t max_queue_size, std::chrono::milliseconds timeout) {
    if (stage_index < 0 || stage_index >= num_stages_) {
      return;
    }
    stages_[stage_index] =
        std::make_unique<PipelineStage>(process_func, max_queue_size, timeout);
    if (stage_index > 0) {
      stages_[stage_index - 1]->SetNextStage(stages_[stage_index].get());
    }
  }

  void Run() {
    for (int stage_index = 0; stage_index < num_stages_; ++stage_index) {
      std::cout << "pipeline stage:" << stage_index << std::endl;
      threads_.push_back(
          std::thread([&, stage_index]() { stages_[stage_index]->Process(); }));
    }
  }

  bool PushData(const DataItem& item) { return stages_[0]->PushData(item); }

  void Finish() {
    // Finish all stages
    for (int stage_index = 0; stage_index < num_stages_; ++stage_index) {
      stages_[stage_index]->Finish();
    }

    // Join the threads and wait for them to finish
    for (int stage_index = 0; stage_index < num_stages_; ++stage_index) {
      threads_[stage_index].join();
    }
  }

 private:
  int num_stages_;
  std::vector<std::unique_ptr<PipelineStage>> stages_;
  std::vector<std::thread> threads_;
};

#endif  // PIPE_LINE_H
