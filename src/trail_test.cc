#define BOOST_TEST_MODULE trail_test

#include "utils.h"

#include <iostream>

namespace {
    template<typename T>
    struct AccumulationTraits;

    template<>
    struct AccumulationTraits<char> {
        using AccT = int;
        static AccT const zero = 0;
    };

    template<>
    struct AccumulationTraits<short> {
        using AccT = int;
        static AccT const zero = 0;
    };

    template<>
    struct AccumulationTraits<int> {
        using AccT = long;
        static AccT const zero = 0;
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