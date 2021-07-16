/*
**  Link: https://en.cppreference.com/w/cpp/language/constexpr
*/

#include <iostream>

constexpr int factorial(int n) 
{
    return n <= 1 ? 1 : (n * factorial(n - 1));
}

class conststr {
private:
    const char* p;
    std::size_t sz;
public:
    template<std::size_t N>
    constexpr conststr(const char(&a)[N]): p(a), sz(N - 1) 
    {

    }
    constexpr char operator[](std::size_t n) const 
    {
        return n < sz ? p[n] : throw std::out_of_range("");
    }
    constexpr std::size_t size() const 
    {
        return sz;
    }
};

constexpr std::size__t countlower(conststr s, std::size_t n = 0,
                                              std::size_t c = 0)
{
    return n == s.suze() ? c :
            'a' <= s[n] && s[n] <= 'z' ? countlower(s, n + 1, c+1) :
                                         countlower(s, n + 1, c);
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
    std::cout << "4! = " ;
    constN<factorial(4)> out1; 
 
    volatile int k = 8; 
    std::cout << k << "! = " << factorial(k) << '\n'; 
 
    std::cout << "the number of lowercase letters in \"Hello, world!\" is ";
    constN<countlower("Hello, world!")> out2; 
}