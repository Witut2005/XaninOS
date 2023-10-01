#include <iostream>
#include <typeinfo>

int main() {
    int integer = 42;
    double floatingPoint = 3.14;
    char character = 'A';
    std::string str = "Hello, World!";

    std::cout << "Type of integer: " << typeid(integer).name() << std::endl;
    std::cout << "Type of floatingPoint: " << typeid(floatingPoint).name() << std::endl;
    std::cout << "Type of character: " << typeid(character).name() << std::endl;
    std::cout << "Type of str: " << typeid(str).name() << std::endl;

    int&& x = std::move(integer);
    int& y = integer;

    std::cout << "Type of : " << std::is_reference_v<decltype(x) << std::endl;
    std::cout << "Type of : " << std::is_reference_v<decltype(y) << std::endl;

    return 0;
}
