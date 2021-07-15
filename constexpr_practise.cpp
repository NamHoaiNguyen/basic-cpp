#include <iostream>

constexpr int factorial(int n) 
{
    return n <= 1 ? 1 : (n * factorial(n - 1));
}

template <int n>
struct constN
{
    constN() 
    {
        std::cout << n << std::endl;
    }
};

int main()
{
    constN<factorial(4)> out1;
    return 0;
}