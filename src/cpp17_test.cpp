#define BOOST_TEST_MODULE cpp17_test
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <random>
#include <vector>
#include <chrono>
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
			explicit test(strong_type_constructor_2 c2) : value_(c2.get() / 2)
			{
				std::cout << "Constructor 2 called " << std::endl;
			}
	};

	test t_1(strong_type_constructor_1{1});
	test t_2(strong_type_constructor_2{2});
}

/*
**	crtp: https://www.fluentcpp.com/2017/05/16/what-the-crtp-brings-to-code/
*/

namespace benchmark_test_crtp_ns
{
	template<typename T>
	class Base_crtp{
		public:
			Base_crtp() {}
			auto getValue() const {
				return static_cast<T const&>(*this).getValue();
				// return this->getValue();
			}
	};

	class Derived_constant_crtp : public Base_crtp<Derived_constant_crtp>
	{
		public:
			Derived_constant_crtp() {}
			auto getValue() const {
				return 42;
			}
	};

	class Derived_variable_crtp : public Base_crtp<Derived_variable_crtp>
	{
		private:
			int value_;
		public:
			explicit Derived_variable_crtp(int value) : value_(value)
			{

			}
			auto getValue() const
			{
				return value_;
			}
	};

	template<typename T>
	class Test_crtp {
		public:
			Test_crtp() {}
			auto print(Base_crtp<T> const& base)
			{
				std::cout << base.getValue() << std::endl;
			}
	};

	template<typename T>
	auto print(Base_crtp<T> const& base )
	{
		std::cout << base.getValue() << std::endl;
	}

	template<typename T>
	class Base{
		public:
			Base() {}
			virtual int getValue() const = 0;
	};

	class Derived_constant : public Base<Derived_constant>
	{
		public:
			Derived_constant() {}
			int getValue() const override {
				return 42;
			}
	};

	class Derived_variable : public Base<Derived_variable>
	{
		private:
			int value_;
		public:
			explicit Derived_variable(int value) : value_(value)
			{

			}

			int getValue() const override {
				return value_;
			}
	};

	template<typename T>
	class Test {
		public:
			Test() {}
			auto print(Base<T> const& base)
			{
				std::cout << base.getValue() << std::endl;
			}
	};

	template<typename T>
	auto print(Base<T> const& base )
	{
		std::cout << base.getValue() << std::endl;
	}

}

// namespace benchmark_test_not_using_crtp_ns
// {
// 	template<typename T>
// 	class Base{
// 		public:
// 			Base() {}
// 			virtual int getValue() const = 0;
// 	};

// 	class Derived_constant : public Base<Derived_constant>
// 	{
// 		public:
// 			Derived_constant() {}
// 			int getValue() const override {
// 				return 42;
// 			}
// 	};

// 	class Derived_variable : public Base<Derived_variable>
// 	{
// 		private:
// 			int value_;
// 		public:
// 			explicit Derived_variable(int value) : value_(value)
// 			{

// 			}

// 			int getValue() const override {
// 				return value_;
// 			}
// 	};

// 	template<typename T>
// 	class Test {
// 		public:
// 			Test() {}
// 			auto print(Base<T> const& base)
// 			{
// 				std::cout << base.getValue() << std::endl;
// 			}
// 	};

// 	template<typename T>
// 	auto print(Base<T> const& base )
// 	{
// 		std::cout << base.getValue() << std::endl;
// 	}
// }

BOOST_AUTO_TEST_CASE(test_benchmark_crtp)
{
	using namespace benchmark_test_crtp_ns;
	auto start_time_crtp = std::chrono::high_resolution_clock::now();
	Derived_constant_crtp d42_crtp;
	Derived_variable_crtp v_crtp(43);
	// Test<int> test;
	// test.print(d42);
	// test.print(43);
	print(d42_crtp);
	print(v_crtp);
	auto end_time_crtp = std::chrono::high_resolution_clock::now();
	auto execution_time_crtp = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_crtp - start_time_crtp);
	std::cout << execution_time_crtp.count() << std::endl;
	
	auto start_time = std::chrono::high_resolution_clock::now();
	Derived_constant d42;
	Derived_variable v(43);
	print(d42);
	print(v);
	auto end_time = std::chrono::high_resolution_clock::now();
	auto execution_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
	std::cout << execution_time.count() << std::endl;
}

// BOOST_AUTO_TEST_CASE(test_benchmark_not_using_crtp)
// {
// 	using namespace benchmark_test_not_using_crtp_ns;
// 	auto start_time = std::chrono::high_resolution_clock::now();
// 	Derived_constant d42;
// 	Derived_variable v(43);
// 	print(d42);
// 	print(v);
// 	auto end_time = std::chrono::high_resolution_clock::now();
// 	auto execution_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
// 	std::cout << execution_time.count() << std::endl;

// }

/*
**	Strong type interface
**	Link:  https://www.fluentcpp.com/2016/12/08/strong-types-for-strong-interfaces/
*/

namespace test_strong_type_interface_ns
{
	template<typename type, typename parameter>
	struct Strong_type_interface {
		private:
			type val_;
		public:
			explicit Strong_type_interface(const type &v) : val_{v} {}
			explicit Strong_type_interface(type &&v) : val_{std::move(v)} {}
			type &get() {
				return val_;
			}
			const type& get() const {
				return val_;
			}
	};
}

BOOST_AUTO_TEST_CASE(test_strong_type_interface)
{
	using namespace test_strong_type_interface_ns;
	using strong_type_1 = Strong_type_interface<int, struct strong_type_interface_1>;
	using strong_type_2 = Strong_type_interface<int, struct strong_type_interface_2>;

	class test {
		private:
			int val_1;
			int val_2;
		public:
			test(strong_type_1 v_1, strong_type_2 v_2) : val_1{v_1.get()}, val_2{v_2.get()} 
			{

			} 
	};
	test f(strong_type_1{1}, strong_type_2{1});
}

namespace test_enable_if_ns
{
	template<typename T>
	using EnableIfString = std::enable_if_t<
							std::is_convertible_v<T, std::string>>;

	class Person {
		private:
			std::string name_;
		public:
			template<typename STR, typename = EnableIfString<STR>>
			explicit Person(STR&& n) : name_(std::forward<STR>(n)) {
				std::cout << "TMPL-CONSTR for " << name_ << std::endl;
			}
			Person (Person const& p) : name_(p.name_) {
				std::cout << "COPY-CONSTR Person " << name_ << std::endl;
			}
			Person (Person&& p) : name_(std::move(p.name_)) {
				std::cout << "MOVE-CONSTR Person " << name_ << std::endl;
			}
	};
}

BOOST_AUTO_TEST_CASE(test_enable_if)
{
	using namespace test_enable_if_ns;

	std::string s = "sname";
	Person p1(s);
	Person p2("tmp");
	Person p3(p1);
	Person p4(std::move(p1));
}