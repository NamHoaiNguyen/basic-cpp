#define BOOST_TEST_MODULE stl_algorithms_test

#include "utils.h"

#include <boost/test/included/unit_test.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>
using namespace std;

void log(std::string_view txt, const std::vector<int>& vec)
{
    std::cout << txt;
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

void log(std::string_view txt, int64_t val, bool tag) {
	if (tag) {
		std::cout << txt << std::boolalpha << bool(val) << std::endl;
	} else {
		std::cout << txt << std::noboolalpha << val << std::endl;
	}
}

/*test std::for_each, std::transform*/
BOOST_AUTO_TEST_CASE(test_for_each_transform)
{
    TEST_LOG();

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> vec2;

    std::for_each(vec.begin(), vec.end(), [](auto& elem) {
        elem = elem * 2;
    });

    log("Vector after for_each: ", vec);

    std::transform(vec.begin(), vec.end(), std::back_inserter(vec2), [](auto &elem) {
        return elem * elem;
    });

    log("- Vector 1: ", vec);
	log("- Vector 2: ", vec2);

    std::vector<int>vec_res = {2, 4, 6, 8, 10, 12, 14, 16,18, 20};
    auto check_equal_vec = std::equal(vec.begin(), vec.end(), vec_res.begin());

    std::vector<int>vec_res2 = {4, 16, 36, 64, 100, 144, 196, 256 ,324, 400};
    auto check_equal_vec2 = std::equal(vec2.begin(), vec2.end(), vec_res2.begin());

    BOOST_REQUIRE(check_equal_vec == true);
    BOOST_REQUIRE(check_equal_vec2 == true);
}

/*test any_of, all_of, none_of*/
BOOST_AUTO_TEST_CASE(test_algorithm)
{
    TEST_LOG();

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto all = std::all_of(vec.begin(), vec.end(), [](const auto& elem) {
        return elem > 0;
    });

    log("All elems are positive: ", all, true);

    auto any = std::any_of(vec.begin(), vec.end(), [](const auto& elem) {
        return elem < 0;
    });

    log("Has elem smaller than zero: ", any, true);

    auto none = std::none_of(vec.begin(), vec.end(), [](const auto& elem) {
		return elem % 2 == 0;
	});
	log("Has no even elements: ", none, true);
}

/*remove, erase*/
BOOST_AUTO_TEST_CASE(test_remove)
{
    TEST_LOG();

    std::vector<int> vec = {1,1,2,4,2,3,3,3,5,5,4,5,2,8,1,1,2,2,3,5,8};

    auto rem = vec.erase(std::remove(vec.begin(), vec.end(), 8), vec.end());
    log("Vector after applying std::remove ", vec);
    BOOST_TEST((vec == std::vector{1, 1, 2, 4, 2, 3, 3, 3, 5, 5, 4, 5, 2, 1, 1, 2, 2, 3, 5}));
   
    auto uni = vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    log("Vector after applying std::unique ", vec);
    BOOST_TEST((vec == std::vector{1, 2, 4, 2, 3, 5, 4, 5, 2, 1, 2, 3, 5}));
}

/*partition, partition_point*/
BOOST_AUTO_TEST_CASE(test_partition)
{
    TEST_LOG();

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::partition(v.begin(), v.end(), [](int i) {
        return i % 2 == 0;
    });
    auto p = std::partition_point(v.begin(), v.end(), [](int i){
        return i % 2 == 0 ;
    });

    std::cout << "Before partition: ";
    std::copy(v.begin(), p, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\nAfter partition: ";
    std::copy(p, v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}