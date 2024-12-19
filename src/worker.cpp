#include "worker.hpp"

namespace threads
{

threads::Worker worker{};

void Worker::start()
{
    thread_ = std::thread{&Worker::process_tasks, this};
}

void Worker::stop()
{
    {
        std::lock_guard<std::mutex> lock{mutex_};
        stop_running_.store(true);
    }

    if (thread_.joinable())
        thread_.join();
}

void Worker::add_task(TaskFunc&& task)
{
    {
        std::lock_guard<std::mutex> lock{mutex_};
        task_queue_.push(std::move(task));
    }

    task_condition_.notify_one();
}

void Worker::process_tasks()
{
    while (!stop_running_.load())
    {
        TaskFunc task;

        {
            std::unique_lock<std::mutex> lock(mutex_);
            task_condition_.wait(lock, [this] { return !task_queue_.empty() || stop_running_.load(); });

            if (stop_running_.load() && task_queue_.empty())
                break;

            if (!task_queue_.empty())
            {
                task = std::move(task_queue_.front());
                task_queue_.pop();
            }
        }

        if (task) task();
    }
}

} // namespace threads
