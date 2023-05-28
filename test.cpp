#include <iostream>
#include <typeinfo>

template<typename... Ts>
void PrintTypeNames()
{
    ((std::cout << typeid(Ts).name() << std::endl), ...);
}

int main()
{
    // Example usage: print type names
    PrintTypeNames<int, double, char, std::string>();

    return 0;
}
