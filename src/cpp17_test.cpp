#define BOOST_TEST_MODULE cpp17_test
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <random>
#include <vector>
#include <type_traits>
#include <functional>
#include <ratio>

/*
**	Strong type constructor: 
*/

BOOST_AUTO_TEST_CASE(test_strongly_typed_constructors)
{
	class strong_type_constructor_1 {
		private:
			int value_;
		public:
			explicit strong_type_constructor_1(int value) :value_(value)
			{

			}
			auto get() const 
			{
				return value_;
			}
	};

	class strong_type_constructor_2 {
		private:
			int value_;
		public:
			explicit strong_type_constructor_2(int value) : value_(value)
			{

			}
			auto get() const 
			{
				return value_;
			}
	};

	class test {
		private:
			int value_;
		public:
			explicit test(strong_type_constructor_1 c1) : value_(c1.get())
			{
				std::cout << "Constructor 1 called " << std::endl;
			}
			explicit test(strong_type_constructor_2 c2) : value_(c2.get())
			{
				std::cout << "Constructor 2 called " << std::endl;
			}
	};

	test t_1(strong_type_constructor_1{1});
	test t_2(strong_type_constructor_2{2});
}