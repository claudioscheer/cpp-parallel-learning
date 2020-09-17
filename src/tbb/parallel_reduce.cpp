#include <algorithm>
#include <chrono>
#include <iostream>
#include <tbb/global_control.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <vector>

int generate_value() { return 1; }

int main() {
    /*
        Limit total number of worker threads that can be active in the task
       scheduler. With max_allowed_parallelism set to 1, global_control enforces
       serial execution of all tasks by the application thread(s), i.e. the task
       scheduler does not allow worker threads to run.

        https://software.intel.com/en-us/node/589744
    */
    tbb::global_control tc(tbb::global_control::max_allowed_parallelism, 12);

    std::vector<int> values(10000000);
    std::generate(values.begin(), values.end(), generate_value);

#pragma region sequential
    auto start = std::chrono::steady_clock::now();
    int sequential_result = 0;
    for (int i = 0; i < values.size(); i++) {
        sequential_result += values[i];
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::milliseconds elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Sequential result: " << sequential_result << " ("
              << elapsed_ms.count() << "ms)" << std::endl;
#pragma endregion

#pragma region parallel_reduce
    start = std::chrono::steady_clock::now();
    int parallel_result = tbb::parallel_reduce(
        tbb::blocked_range<int>(0, values.size()),
        0, // For sum, the initial value is zero. For multiplication, the
           // initial value is one.
        [&](tbb::blocked_range<int> range, int running_total) -> int {
            for (int i = range.begin(); i != range.end(); i++) {
                running_total += values[i];
            }
            return running_total;
        },
        std::plus<int>());
    end = std::chrono::steady_clock::now();
    elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Parallel reduce result: " << parallel_result << " ("
              << elapsed_ms.count() << "ms)" << std::endl;
#pragma endregion

    return 0;
}
