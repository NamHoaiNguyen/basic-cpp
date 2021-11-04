#define BOOST_TEST_MODULE modern_cpp_design

#include "utils.h"

#include <boost/test/included/unit_test.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <string_view>
#include <vector>

namespace techniques {
    namespace test_type_selection_ns {
        template<bool flag, typename T, typename U>
        class Select {
            public: 
                using type = T;
        };

        template<typename T, typename U>
        class Select<false, T, U> {
            public:
                using type = U;
        };

        class First {
            public: 
                void foo() {
                    std::cout << "First class" << std::endl;
                }
        };

        class Second {
            public:
                void foo() {
                    std::cout << "Second class" << std::endl;
                }
        };
    }
}

BOOST_AUTO_TEST_CASE(test_type_selection) {
    TEST_LOG();

    using namespace techniques::test_type_selection_ns;

    using type1 = typename Select<true, First, Second>::type;
    using type2 = typename Select<false, First, Second>::type;
    type1 test1;
    type2 test2;

    test1.foo();   
    test2.foo();
}