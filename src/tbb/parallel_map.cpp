#include <algorithm>
#include <chrono>
#include <iostream>
#include <tbb/global_control.h>
#include <tbb/parallel_for.h>
#include <vector>

bool validate_matrix(std::vector<std::vector<int>> *matrix, int *matrix_size) {
    bool values_correct = 1;
    for (int i = 0; i < *matrix_size; i++) {
        for (int j = 0; j < *matrix_size; j++) {
            values_correct = (*matrix)[i][j] == *matrix_size;
        }
    }
    return values_correct;
}

std::vector<std::vector<int>> get_matrix(int *matrix_size, int initial_value) {
    std::vector<std::vector<int>> matrix(
        *matrix_size, std::vector<int>(*matrix_size, initial_value));
    return matrix;
}

int main() {
    /*
        Limit total number of worker threads that can be active in the task
       scheduler. With max_allowed_parallelism set to 1, global_control enforces
       serial execution of all tasks by the application thread(s), i.e. the task
       scheduler does not allow worker threads to run.

        https://software.intel.com/en-us/node/589744
    */
    tbb::global_control tc(tbb::global_control::max_allowed_parallelism, 12);

    int matrix_size = 500;

    std::vector<std::vector<int>> a = get_matrix(&matrix_size, 1);
    std::vector<std::vector<int>> b = get_matrix(&matrix_size, 1);

#pragma region sequential
    std::vector<std::vector<int>> c = get_matrix(&matrix_size, 0);
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            for (int k = 0; k < matrix_size; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    auto end = std::chrono::steady_clock::now();

    std::chrono::milliseconds elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Sequential result: " << validate_matrix(&c, &matrix_size)
              << " (" << elapsed_ms.count() << "ms)" << std::endl;
#pragma endregion

#pragma region parallel_map 1
    c = get_matrix(&matrix_size, 0);
    start = std::chrono::steady_clock::now();
    tbb::parallel_for(
        tbb::blocked_range<int>(0, matrix_size),
        [&c, &matrix_size, &a, &b](tbb::blocked_range<int> range) {
            for (int i = range.begin(); i != range.end(); i++) {
                for (int j = 0; j < matrix_size; j++) {
                    for (int k = 0; k < matrix_size; k++) {
                        c[i][j] += a[i][k] * b[k][j];
                    }
                }
            }
        });
    end = std::chrono::steady_clock::now();
    elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Parallel map 1 result: " << validate_matrix(&c, &matrix_size)
              << " (" << elapsed_ms.count() << "ms)" << std::endl;
#pragma endregion

#pragma region parallel_map 2
    c = get_matrix(&matrix_size, 0);
    start = std::chrono::steady_clock::now();
    tbb::parallel_for(
        tbb::blocked_range<int>(0, matrix_size),
        [&c, &matrix_size, &a, &b](tbb::blocked_range<int> range) {
            for (int i = range.begin(); i != range.end(); i++) {
                tbb::parallel_for(
                    tbb::blocked_range<int>(0, matrix_size),
                    [&c, &matrix_size, &a, &b,
                     &i](tbb::blocked_range<int> range) {
                        for (int j = range.begin(); j != range.end(); j++) {
                            for (int k = 0; k < matrix_size; k++) {
                                c[i][j] += a[i][k] * b[k][j];
                            }
                        }
                    });
            }
        });
    end = std::chrono::steady_clock::now();
    elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Parallel map 2 result: " << validate_matrix(&c, &matrix_size)
              << " (" << elapsed_ms.count() << "ms)" << std::endl;
#pragma endregion

    return 0;
}
