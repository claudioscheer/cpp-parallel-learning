#include <iostream>

int main() {
    std::string text = "Dhis is a simple test.";
    text[0] = 'T';
    std::cout << text[0] << std::endl;
    int length = text.length();
    std::cout << length << std::endl;
    std::cout << text.find(" a", 0) << std::endl;
    std::string substring = text.substr(4, 8);
    std::cout << substring << std::endl;
    return 0;
}
