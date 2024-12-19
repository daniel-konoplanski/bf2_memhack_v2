#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace threads
{

using Task = void();
using TaskFunc = std::function<Task>;

class Worker
{
public:
    void start();
    void stop();
    void add_task(TaskFunc&& task);

private:
    void process_tasks();

private:
    mutable std::mutex mutex_{};
    std::atomic<bool> stop_running_{false};
    std::queue<TaskFunc> task_queue_;
    std::thread thread_;
    std::condition_variable task_condition_;
};

extern threads::Worker worker;

} // namespace threads
