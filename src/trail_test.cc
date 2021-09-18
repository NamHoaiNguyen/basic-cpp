#define BOOST_TEST_MODULE trail_test

#include "utils.h"

#include <iostream>
#include <list>
#include <vector>

namespace accumulation {
    template<typename T>
    struct AccumulationTraits;

    template<>
    struct AccumulationTraits<char> {
        using AccT = int;
        /*drawback: only allow to initilaize a* static constant 
        data member inside its class only if it has integral or enumeration type
        Method 1: using constexpr -> drawback: cant use for nonliteral types
        Method 2: using inline in C++17
        Method 3: prior to c++17, using inline function that return nonliteral types */
        static AccT const zero = 0;
    };

    template<>
    struct AccumulationTraits<short> {
        using AccT = int;
        /*static AccT const zero = 0;*/
        static constexpr AccT zero() {
            return 0;
        }
    };

    template<>
    struct AccumulationTraits<int> {
        using AccT = long;
        static AccT const zero = 0;
    };

    template<>
    struct AccumulationTraits<unsigned int> {
        using AccT = unsigned long;
        static constexpr AccT zero() {
            return 0;
        }
    };

    template<>
    struct AccumulationTraits<float> {
        using AccT = float;
        static constexpr float zero = 0.0f;
    };

    class BigInt {
        public:
            explicit BigInt(long long value) : value_(value) {

            } 
        private:    
            long long value_;
    };  

    template<>
    struct AccumulationTraits<BigInt> {
        using AccT = BigInt;
        static BigInt zero() {
            return BigInt{0};
        }
    };

    template<typename T>
    auto accum(T const* beg, T const* end) {
        using AccT = typename AccumulationTraits<T>::AccT;
        AccT total = AccumulationTraits<T>::zero;
        while (beg != end) {
            total += *beg;
            ++beg;
        }
        return total;
    }
}

BOOST_AUTO_TEST_CASE(test_accumulation) {
    TEST_LOG();

    using namespace accumulation;

    int num[] = {1, 2, 3, 4, 5};
    std::cout << "the average value of the integer values is "
              << accum(num, num+5) / 5 << std::endl;

    char name[] = "templates";
    int length = sizeof(name)-1;
    // (try to) print average character value
    std::cout << "the average value of the characters in \""
          << name << "\" is "
          << accum(name, name+length) / length << std::endl;
}

namespace ElementType {
    template<typename T>
    struct ElementT;

    template<typename T>
    struct ElementT<std::vector<T>> {
        using Type = T;
    };

    template<typename T>
    struct ElementT<std::list<T>> {
        using Type = T;
    };

    template<typename T, std::size_t N>
    struct ElementT<T[N]> {
        using Type = T;
    };

    template<typename T>
    struct ElementT<T[]> {
        using Type = T;
    };

    template<typename T>
    void printElementType(T const& c) {
        std::cout << "Container of " 
                  << typeid(typename ElementT<T>::Type).name()
                  << " elements" << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(test_element_type) {
    TEST_LOG();

    using namespace ElementType;

    std::vector<bool> s;
    printElementType(s);
    int arr[42];
    printElementType(arr);
    int arr_[10];
    printElementType(arr_);
    std::vector<float> v;
    printElementType(v);
}