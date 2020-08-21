// Credits: https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h.
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <future>
#include <functional>
#include "queue.h"

class ThreadPool
{
public:
    ThreadPool(int);
    template <class F, class... Args>
    auto enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();

private:
    // Need to keep track of threads so we can join them.
    std::vector<std::thread> workers;
    // Task queue.
    utils::queue<std::function<void()>> tasks;
    // Synchronization.
    bool stop;
};

inline ThreadPool::ThreadPool(int threads)
{
    this->stop = false;
    for (int i = 0; i < threads; ++i)
        workers.emplace_back(
            [this] {
                for (;;)
                {
                    if (this->stop && this->tasks.empty())
                    {
                        return;
                    }
                    std::function<void()> task = this->tasks.pop();
                    task();
                }
            });
}

template <class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        // Don't allow enqueueing after stopping the pool.
        if (this->stop)
        {
            throw std::runtime_error("enqueue on stopped ThreadPool.");
        }
        this->tasks.push([task]() { (*task)(); });
    }
    return res;
}

inline ThreadPool::~ThreadPool()
{
    this->stop = true;
    for (std::thread &worker : this->workers)
    {
        worker.join();
    }
}

#endif
