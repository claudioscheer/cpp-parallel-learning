#include <iostream>
#include <vector>
#include <algorithm>
#include <tbb/parallel_for.h>

// Parallel for is still not working well.
// The result is different from the sequential algorithm.

class SumVector
{
public:
    SumVector(std::vector<int> *values, int *result)
    {
        _values = values;
        _result = result;
    }

    void operator()(const tbb::blocked_range<int> &range) const
    {
        for (int i = range.begin(); i < range.end(); i++)
        {
            (*_result) += (*_values)[i];
        }
    }

private:
    int *_result;
    std::vector<int> *_values;
};

int generate_values()
{
    static int i = 0;
    return ++i;
}

int main()
{
    std::vector<int> values(1000);
    std::generate(values.begin(), values.end(), generate_values);

#pragma region sequential
    int sequential_result = 0;
    for (int i = 0; i < values.size(); i++)
    {
        sequential_result += values[i];
    }
    std::cout << "Sequential result: " << sequential_result << std::endl;
#pragma endregion

#pragma region parallel_for
    int parallel_result = 0;
    auto func = SumVector(&values, &parallel_result);
    tbb::parallel_for(tbb::blocked_range<int>(0, values.size()), func);
    std::cout << "Parallel for result: " << parallel_result << std::endl;
#pragma endregion

    return 0;
}