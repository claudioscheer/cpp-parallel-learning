#include <iostream>

using namespace std;

int main()
{
    cout << "Enter your name: ";
    string name;
    getline(cin, name);
    cout << "Your name is: " << name << endl;
    int age;
    cout << "Enter your age: ";
    cin >> age;
    cout << "Your age is: " << age << endl;

    return 0;
}
