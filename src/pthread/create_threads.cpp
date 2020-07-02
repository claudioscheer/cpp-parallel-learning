#include <iostream>
#include <pthread.h>

#define THREADS_NUMBER 3

int main()
{
    pthread_t threads[THREADS_NUMBER];
    for (int i = 0; i < THREADS_NUMBER; i++)
    {
        std::cout << "Creating thread " << i << "..." << std::endl;
    }
    return 0;
}