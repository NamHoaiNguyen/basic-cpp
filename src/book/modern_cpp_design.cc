#define BOOST_TEST_MODULE modern_cpp_design

#include "utils.h"

#include <boost/test/included/unit_test.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <string_view>
#include <type_traits>
#include <vector>

namespace techniques {
    /*Technique to decide which functon will be called at compile-time(Instead of
    using if-else statement)*/
    namespace test_integral_constants_to_types_ns {
        template<int v>
        struct Int2Type {
            enum { value = v };
        };

        struct First {
            private:
                int var_1 = 0;
            
            public:
                void foo() {
                    std::cout << "First class" << std::endl;
                }
        };

        struct Second {
            private:
                void foo() {
                    std::cout << "Second class" << std::endl;
                }
            
            public:
                int var_1 = 0;
                int var_2 = 0;
        };

        struct Base {
            public:
                virtual void foo() = 0;
        };

        struct Derived1 : public Base {
            public:
                void foo() override {
                    std::cout << "Derived class 1" << std::endl;          
                }
        };

        struct Base2 {
            void foo() {
                std::cout << "Not polymorphic class" << std::endl;
            }
        };

        // struct Derived2 : public Base {
        //     public:
        //         void foo() override {
        //             std::cout << "Derived class 2" << std::endl;          
        //         }
        // };

        template<typename T, bool IsPolymorphic>
        struct Container {
            private:
                void foo(T &obj, Int2Type<true>) {
                    obj->foo();
                }

                void foo(T &obj, Int2Type<false>) {
                    obj->foo();
                }

            public:
                void foo(T &obj) {
                    foo(obj, Int2Type<IsPolymorphic>());
                }
        };
    }

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

BOOST_AUTO_TEST_CASE(test_integral_constants_to_types) {
    TEST_LOG();

    using namespace techniques::test_integral_constants_to_types_ns;

    std::unique_ptr<Base> b = std::make_unique<Derived1>();
    /*b->foo(); ???*/
    Container<std::unique_ptr<Base>, std::is_polymorphic_v<Derived1>> c;
    c.foo(b);

    std::unique_ptr<Base2> b2;
    Container<std::unique_ptr<Base2>, std::is_polymorphic_v<Base2>> c2;
    c2.foo(b2);
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