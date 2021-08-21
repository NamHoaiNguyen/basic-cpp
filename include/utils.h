#ifndef INCLUDE_UTILS_H
#define INCLUDE_UTILS_H

/*Boost lib*/
#include <boost/test/included/unit_test.hpp>
#include <boost/test/framework.hpp>

/*others*/
#include <iostream>

struct TestLog {
	TestLog() {
		std::cout << "\033[1m\033[32m----------- START TEST CASE: " << boost::unit_test::framework::current_test_case().p_name
                  << "\033[1m\033[32m----------" << std::endl;
	}
	~TestLog() {
		std::cout << "\033[1m\033[31m---------- FINISH TEST CASE: " << boost::unit_test::framework::current_test_case().p_name
                  << "\033[1m\033[31m----------" << std::endl;
		std::cout << std::endl;
	}
};

#define TEST_LOG() TestLog testLog

#endif