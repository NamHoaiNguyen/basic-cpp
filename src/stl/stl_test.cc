#define BOOST_TEST_MODULE stl_algorithms_test

#include "utils.h"

#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <string_view>
#include <vector>

void log(std::string_view txt, int64_t val, bool tag) {
	if (tag) {
		std::cout << txt << std::boolalpha << (bool)val << "\n";
	} else {
		std::cout << txt << std::noboolalpha << val << "\n";
	}
}

/*test any_of, all_of, none_of*/
BOOST_AUTO_TEST_CASE(test_algorithm)
{
   TEST_LOG();

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto all = std::all_of(vec.begin(), vec.end(), [](const auto& elem) {
        return elem > 0;
    });

    std::cout << "All elems are positive " << all << std::endl;

    auto any = std::any_of(vec.begin(), vec.end(), [](const auto& elem) {
        return elem < 0;
    });

    std::cout << "Has elem smaller than 0 " << any << std::endl;

    auto none = std::none_of(vec.begin(), vec.end(), [](const auto& elem) {
		return elem % 2 == 0;
	});
	log("Has no even elements: ", none, true);

    std::cout << "Has no even elements " << none << std::endl;
}