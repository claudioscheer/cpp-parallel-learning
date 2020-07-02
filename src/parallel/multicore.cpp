#include <iostream>
#include <thread>

// https://ncona.com/2019/05/using-thread-pools-in-cpp/

int main(int argc, char const *argv[])
{
    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    std::cout << concurentThreadsSupported << std::endl;
    return 0;
}
