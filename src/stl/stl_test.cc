#define BOOST_TEST_MODULE stl_algorithms_test

#include "utils.h"

#include <boost/test/included/unit_test.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <string_view>
#include <vector>

using namespace std;

void log(std::string_view txt, const std::vector<int>& vec) {
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
BOOST_AUTO_TEST_CASE(test_for_each_transform) {
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

namespace overload_operator {
    template<typename Os, typename V>
    Os& operator<< (Os& os, V const& v) {
        os << "{ ";
        for (auto const& e : v) os << e << ' ';
        return os << "}";
    }
}

/*test any_of, all_of, none_of*/
BOOST_AUTO_TEST_CASE(test_range) {
    TEST_LOG();

    using namespace overload_operator;

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

    static constexpr auto v1 = {1, 2, 3, 4, 5};
    static constexpr auto v2 = {3, 5, 4, 1, 2};
    static constexpr auto v3 = {3, 5, 4, 1, 1};

    std::cout << v2 << " is a permutation of " << v1 << ": " << std::boolalpha
              << std::is_permutation (v1.begin(), v1.end(), v2.begin()) << std::endl
              << v3 << " is a permutation of " << v1 << ": " << std::boolalpha
              << std::is_permutation (v1.begin(), v1.end(), v3.begin(), v3.end()) << std::endl;  

    std::vector<char> v1_char {'a', 'b', 'c', 'd'};
    std::vector<char> v2_char {'a', 'b', 'c', 'd'};
    
    std::mt19937 g{std::random_device{}()};
    while (!std::lexicographical_compare(v1_char.begin(), v1_char.end(),
                                         v2_char.begin(), v2_char.end())) {
        for (auto c : v1_char) std::cout << c << ' ';
        std::cout << ">= ";
        for (auto c : v2_char) std::cout << c << ' ';
        std::cout << std::endl;
 
        std::shuffle(v1_char.begin(), v1_char.end(), g);
        std::shuffle(v2_char.begin(), v2_char.end(), g);
    }
}

/*remove, erase*/
BOOST_AUTO_TEST_CASE(test_remove) {
    TEST_LOG();

    std::vector<int> vec = {1,1,2,4,2,3,3,3,5,5,4,5,2,8,1,1,2,2,3,5,8};

    auto rem = vec.erase(std::remove(vec.begin(), vec.end(), 8), vec.end());
    log("Vector after applying std::remove ", vec);
    BOOST_TEST((vec == std::vector<int>{1, 1, 2, 4, 2, 3, 3, 3, 5, 5, 4, 5, 2, 1, 1, 2, 2, 3, 5}));
   
    auto uni = vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    log("Vector after applying std::unique ", vec);
    BOOST_TEST((vec == std::vector<int>{1, 2, 4, 2, 3, 5, 4, 5, 2, 1, 2, 3, 5}));
}

/*partition, partition_point*/
BOOST_AUTO_TEST_CASE(test_partition) {
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

/*Testing level of abstraction*/
namespace test_level_of_abstraction {
    template<typename Container>
    class Range {
        private:
        typename Container::iterator begin_;
        typename Container::iterator end_; 
        public:
        Range(std::pair<typename Container::iterator, typename Container::iterator> range) : begin_(range.first), end_(range.second) {

        }
        typename Container::iterator begin() {
            return begin_;
        }
        typename Container::iterator end() {
            return end_;
        }
    };
}

/*find, find_if, find_if_not, binary_search, equal_range*/
BOOST_AUTO_TEST_CASE(test_find) {
    TEST_LOG();

    using namespace test_level_of_abstraction;

    std::vector<int> v = {1, 2, 3, 3, 4, 6, 7, 8, 9, 10};
    auto n1 = 3;
    auto n2 = 5;
    auto is_even = [](auto i) { return i % 2 == 0; };

    /*For unsorted elements*/
    auto res1 = std::find(v.begin(), v.end(), n1);
    auto res2 = std::find_if(v.begin(), v.end(), is_even);
    auto res3 = std::find_if_not(v.begin(), v.end(), is_even); /*Return the first iterator for an element which predicate condition return false*/

    (res1 != v.end()) 
        ? std::cout << "v contains n1 value: " << *res1 << std::endl
        : std::cout << "v doesn't contain " << n1 << std::endl;

    (res2 != v.end())
        ? std::cout << "v contains an even number: " << *res2 << std::endl
        : std::cout << "v doesn't contain an even number" << std::endl;

    (res2 != v.end())
        ? std::cout << "v contains an odd number: " << *res3 << std::endl
        : std::cout << "v doesn't contain an odd number" << std::endl;

    /*For sorted elements*/
    auto res4 = std::binary_search(v.begin(), v.end(), n1);
    std::cout << "v contain n1 value(found with binary search): " << std::boolalpha << res4 << std::endl;

    /*res5 =std::pair<std::vector<int>::iterator, std::vector<int>::iterator>
        Second paramter is pass-the-end iterator
    */
    auto res5 = std::equal_range(v.begin(), v.end(), n1);
    if (res5.first != v.end()) {
        std::cout << "Distance first index " << std::distance(v.begin(), res5.first) << std::endl;
        std::cout << "Distance last index " << std::distance(v.begin(), res5.second) << std::endl;
    }

    auto res6 = std::lower_bound(v.begin(), v.end(), n2);
    if (res6 != v.end()) {
        std::cout << "Value and distance " << *res6 << " " << std::distance(v.begin(), res6) << std::endl;
    }
    
    auto res7 = std::upper_bound(v.begin(), v.end(), n1);
    if (res7 != v.end()) {
        std::cout << "Value and distance " << *res7 << " " << std::distance(v.begin(), res7) << std::endl;
    }

    Range<std::vector<int>> test_range = std::equal_range(v.begin(), v.end(), n1);
    
    std::for_each(test_range.begin(), test_range.end(), [](auto& elem) {
        elem = elem * 2;
    });
    
    std::cout << std::endl;

    BOOST_TEST((v == std::vector<int>{1, 2, 6, 6, 4, 6, 7, 8, 9, 10}));
}

/*distance*/
BOOST_AUTO_TEST_CASE(test_distance) {
    TEST_LOG();

    std::vector<int> v = {1, 2, 3, 4};
    std::cout << "distance(first, last) = "
              << std::distance(v.begin(), v.end()) << std::endl
              << "distance(last, first) = "
              << std::distance(v.end(), v.begin()) << std::endl;
}

/*min, max, min_element, max_element*/
BOOST_AUTO_TEST_CASE(test_min_max) {
    TEST_LOG();

    constexpr auto a = 1;
    constexpr auto b = 12;
    std::vector<std::string> v = {"foo", "bar", "hello"};

    std::cout << "smaller of a and b " << std::min(a, b) << std::endl;
    std::cout << "smaller of a and b " << std::max(a, b) << std::endl;

    std::vector<int> vec = {1, 4, 7, 14, 5, 12, 9};
    auto min_elem = std::distance(vec.begin(), std::min_element(vec.begin(), vec.end()));
    auto max_elem = std::distance(vec.begin(), std::max_element(vec.begin(), vec.end()));
    std::cout << "min element at " << min_elem << std::endl;
    std::cout << "max element at " << max_elem << std::endl;
}

/*fill, generate, iota*/
BOOST_AUTO_TEST_CASE(test_fill) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::fill(v.begin(), v.end(), -1);
    BOOST_TEST((v == std::vector<int>{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}));

    // std::generate(v.begin(), v.end(), [n = 1] () mutable { return n++; });
    //     return elem++; 
    // });

    // BOOST_TEST((v == std::vector<int>{0, 2, 4, 6, 8, 10, 6, 7, 8, 9}));
    std::vector<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::iota(v1.begin(), std::find(v1.begin(), v1.end(), 5), -5);
    std::copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, " "));
    BOOST_TEST((v1 == std::vector<int>{-5, -4, -3, -2, 5, 6, 7, 8, 9, 10}));

    std::replace(v1.begin(), (v1.begin() + 4), -2, 2);
    BOOST_TEST((v1 == std::vector<int>{-5, -4, -3, 2, 5, 6, 7, 8, 9, 10}));

    std::replace_if(v1.begin(), v1.end(), [](const auto& elem) {
        return elem % 2 == 0;
    }, 1);
    BOOST_TEST((v1 == std::vector<int>{-5, 1, -3, 1, 5, 1, 7, 1, 9, 1}));
}