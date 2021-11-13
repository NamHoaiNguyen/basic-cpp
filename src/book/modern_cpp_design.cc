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
    namespace test_partial_template_specialization_ns {
        template<typename T, typename U>
        struct foo {
            void fun() {
                std::cout << "primary template" << std::endl;
            }
        };

        struct test1 { };
        struct test2 { };
        struct test3 { };

        template<>
        struct foo<test1, test3> {
            void fun() {
                std::cout << "full specilization test1,test3" << std::endl;
            }
        };

        template<typename T> 
        struct foo<T, test3> {
            void fun() {
                std::cout << "partial template specialization T, test3" << std::endl;
            }
        };

    }

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

        struct Derived : public Base {
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

    namespace test_compatability_inhertiance_ns {
        template<typename T, typename U>
        class Conversion {
                using Small = char;
                class Big {char dummy[2];};
                static Small Test(U);
                static Big Test(...);
                static T MakeT();
            public:
                enum {
                    exists = sizeof(Test(MakeT())) == sizeof(Small)
                };
                enum { sameType = false }; 
        };

        template<typename T>
        class Conversion<T, T> {
            public:
                enum {exists = 1, sameType = 1};
        };

        #define SUPERSUBCLASS(T, U) \
            (Conversion<const U*, const T*>::exists && \
            !Conversion<const T*, const void*>::sameType)

        #define SUPERSUBCLASS_STRICT(T, U) \
            (SUPERSUBCLASS(T, U) && \
            !Conversion<const T, const U>::sameType)

        struct base { };

        struct derived : public base { };

        struct base2 { };

        template<typename T,bool = SUPERSUBCLASS_STRICT(base, T)>
        struct foo {
            void fun() {
                std::cout << "Derived from base class" << std::endl;
            }
        };

        template<typename T>
        struct foo<T, false> {
            void fun() {
                std::cout << "Not derived from base class" << std::endl;
            }
        };
    }

    namespace test_type_traits_ns {
        class NullType { };

        struct EmptyType { };
    
        template<typename T>
        class TypeTraits {
            private:
                template<typename U> 
                struct PointerTraits {
                    static constexpr bool result = false;
                    using PointeeType = NullType;
                };

                template<typename U>
                struct PointerTraits<U*> {
                    static constexpr bool result = true;
                    using PointeeType = U;
                };
            
            public:
                static constexpr bool isPointer = PointerTraits<T>::result;
                using PointeeType = typename PointerTraits<T>::PointeeType;
        };
    }
}

BOOST_AUTO_TEST_CASE(test_partial_template_specialization) {
    TEST_LOG();

    using namespace techniques::test_partial_template_specialization_ns;

    foo<test1, test2> f12;
    f12.fun();

    foo<test1, test3> f13;
    f13.fun();

    foo<test2, test3> f23;
    f23.fun();
}

BOOST_AUTO_TEST_CASE(test_integral_constants_to_types) {
    TEST_LOG();

    using namespace techniques::test_integral_constants_to_types_ns;

    std::unique_ptr<Base> b = std::make_unique<Derived>();
    /*b->foo(); ???*/
    Container<std::unique_ptr<Base>, std::is_polymorphic_v<Derived>> c;
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

BOOST_AUTO_TEST_CASE(test_convertibility_inhertance) {
    TEST_LOG();

    using namespace techniques::test_compatability_inhertiance_ns;
    
    std::cout << Conversion<double, int>::exists << std::endl;
    std::cout << Conversion<char, char*>::exists << std::endl;
    std::cout << Conversion<std::size_t, std::vector<int>>::exists << std::endl;

    foo<derived> f1;
    f1.fun();
    
    foo<base2> f2;
    f2.fun();
}

BOOST_AUTO_TEST_CASE(test_type_traits) {
    TEST_LOG();

    using namespace techniques::test_type_traits_ns;

    struct foo {
        private:
            int val{1};
        
        public:
            void fun() {
                std::cout << "Value of val " << val << std::endl;
            }
    };

    std::cout << "Is pointer " << std::boolalpha << TypeTraits<foo>::isPointer << std::endl;
    std::cout << "Is pointer " << std::boolalpha << TypeTraits<foo*>::isPointer << std::endl;

    using pointee_t = typename TypeTraits<foo*>::PointeeType;
    pointee_t test;
    test.fun();
}
