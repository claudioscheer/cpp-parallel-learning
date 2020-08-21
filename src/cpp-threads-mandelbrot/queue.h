// Credits: https://stackoverflow.com/a/12805690.
#ifndef QUEUE_H
#define QUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>

namespace utils
{
    template <typename T>
    class queue
    {
    private:
        std::mutex mutex;
        std::condition_variable condition;
        std::deque<T> queue;

    public:
        void push(T const &value)
        {
            {
                std::unique_lock<std::mutex> lock(this->mutex);
                this->queue.push_front(value);
            }
            this->condition.notify_one();
        }

        T pop()
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            this->condition.wait(lock, [=] { return !this->queue.empty(); });
            T value(std::move(this->queue.back()));
            this->queue.pop_back();
            return value;
        }

        bool empty()
        {
            return this->queue.empty();
        }
    };
} // namespace utils

#endif
