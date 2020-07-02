#include <iostream>
#include <thread>
#include <string>
#include <sstream>

static const int num_threads = 10;

void thread_method(int thread_id)
{
    std::stringstream ss;
    ss << "Function running in thread " << thread_id << "...";
    std::cout << ss.str() << std::endl;
}

int main()
{
    std::thread threads[num_threads];
    for (int i = 0; i < num_threads; i++)
    {
        threads[i] = std::thread(thread_method, i);
    }
    std::cout << "The main function execution." << std::endl;
    for (int i = 0; i < num_threads; i++)
    {
        threads[i].join();
    }
    return 0;
}