#ifndef SRC_ALGORITHMS_TASK_MANAGER_TASK_MANAGER_H_
#define SRC_ALGORITHMS_TASK_MANAGER_TASK_MANAGER_H_

#include <atomic>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "taks_queue.h"

using std::atomic;
using std::bind;
using std::condition_variable;
using std::defer_lock;
using std::function;
using std::mutex;
using std::thread;
using std::unique_lock;
using std::vector;

namespace hhullen {

class TaskManager {
 public:
  TaskManager(size_t threads = thread::hardware_concurrency() / 2)
      : threads_amount_(threads), thread_index_(0), works_(0) {
    if (threads_amount_ < 1) {
      threads_amount_ = 1;
    }
    InitThreads();
  }
  TaskManager(const TaskManager &src) { *this = src; };
  TaskManager(TaskManager &&src) { *this = move(src); };
  ~TaskManager() { Terminate(); }

  void SetThreads(size_t number) {
    Terminate();
    if (number == 0) {
      number = 1;
    }
    threads_amount_ = number;
    threads_.clear();
    tasks_queues_.clear();
    terminate_ = false;
    thread_index_ = 0;
    works_ = 0;
    InitThreads();
  }

  template <typename Func, typename... Args>
  void AddTask(Func &&function, Args &&...args) {
    bool is_pushed = false;
    size_t i = 0;

    for (; !is_pushed && i != threads_amount_ * 2; ++i) {
      size_t index = (thread_index_ + i) % threads_amount_;
      is_pushed = tasks_queues_[index].TryPush(bind(function, args...));
    }

    if (!is_pushed) {
      size_t index = thread_index_ % threads_amount_;
      tasks_queues_[index].ForcedPush(bind(function, args...));
    }
    ++thread_index_;
    notifier_.notify_all();
  }

  void AwaitAll() {
    unique_lock<mutex> locker(mutex_);
    notifier_.wait(locker, [this]() { return works_ == 0; });
  }

  void operator=(const TaskManager &src) {
    this->SetThreads(src.threads_amount_);
  }

 private:
  size_t threads_amount_, thread_index_;
  vector<TaskQueue> tasks_queues_;
  vector<thread> threads_;

  condition_variable notifier_;
  mutex mutex_;

  atomic<int> works_;
  atomic<bool> terminate_{false};

  void Run() {
    while (true) {
      bool is_poped = false;
      function<void()> task;
      for (size_t i = 0; !is_poped && i != threads_amount_; ++i) {
        is_poped = tasks_queues_[i].TryPop(task);
      }

      if (is_poped) {
        ++works_;
        task();
        --works_;
      } else if (terminate_) {
        break;
      }
      notifier_.notify_all();
    }
  }

  void InitThreads() {
    tasks_queues_.resize(threads_amount_);
    for (size_t i = 0; i != threads_amount_; ++i) {
      threads_.emplace_back(thread(&TaskManager::Run, this));
    }
  }

  void Terminate() {
    AwaitAll();
    terminate_ = true;
    for (thread &trd : threads_) {
      if (trd.joinable()) {
        trd.join();
      }
    }
  }
};

}  // namespace hhullen

#endif  // SRC_ALGORITHMS_TASK_MANAGER_TASK_MANAGER_H_
