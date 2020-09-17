#include <iostream>

int main() {
    std::cout << "Enter your name: ";
    std::string name;
    getline(std::cin, name);
    std::cout << "Your name is: " << name << std::endl;
    int age;
    std::cout << "Enter your age: ";
    std::cin >> age;
    std::cout << "Your age is: " << age << std::endl;

    return 0;
}
