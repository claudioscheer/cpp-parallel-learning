#include <iostream>
#include <vector>

int main() {
    int x[5] = {0};
    for (int i = 0; i < 5; i++) {
        std::cout << x[i] << std::endl;
    }

    std::vector<int> y;
    for (int i = 0; i < 10; i++) {
        y.push_back(i);
    }

    std::vector<int>::iterator it = y.begin() + 3;
    y.insert(it, 50);
    for (int i = 0; i < y.size(); i++) {
        std::cout << y[i] << std::endl;
    }

    return 0;
}
