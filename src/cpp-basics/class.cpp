#include "class.h"
#include <iostream>

double Operations::operate() { return this->a * this->b; }

int main() {
    Operations *operations = new Operations();
    operations->set_values(2.5, 1.0);

    double result = operations->operate();

    std::cout << "Result: " << result << std::endl;
    return 0;
}
