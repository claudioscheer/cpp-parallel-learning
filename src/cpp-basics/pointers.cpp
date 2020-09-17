#include <assert.h>
#include <iostream>

int main() {
    // Store a value.
    int value = 1;
    std::cout << value << std::endl;

    /*
        & gets the pointer to a variable.
        *value_address says that the variable stores a pointer.
    */
    int *value_address = &value;
    std::cout << value_address << std::endl;

    // Retrieve the value of a pointer.
    int retrieved_value = *value_address;
    std::cout << retrieved_value << std::endl;

    assert(value == retrieved_value);

    return 0;
}
