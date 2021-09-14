#define BOOST_TEST_MODULE stl_concurency_test

#include "utils.h"

#include <boost/test/included/unit_test.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

BOOST_AUTO_TEST_CASE(test_for_each_transform)
{
    TEST_LOG();

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> vec2;

    std::for_each(vec.begin(), vec.end(), [](auto& elem) {
        elem = elem * 2;
    });

}