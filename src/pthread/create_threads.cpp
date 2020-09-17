#include <iostream>
#include <pthread.h>

#define THREADS_NUMBER 3

void *thread_function(void *args) {
    int pthread_id = (intptr_t)args;
    std::cout << pthread_id << std::endl;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREADS_NUMBER];
    for (int i = 0; i < THREADS_NUMBER; i++) {
        std::cout << "Creating thread " << i << "..." << std::endl;

        // https://docs.microsoft.com/en-us/cpp/c-runtime-library/errno-constants
        // https://stackoverflow.com/a/39950673
        int err = pthread_create(&threads[i], NULL, &thread_function,
                                 (void *)(intptr_t)i);
    }

    for (int i = 0; i < THREADS_NUMBER; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
