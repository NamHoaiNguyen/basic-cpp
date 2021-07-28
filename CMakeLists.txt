cmake_minimum_required(VERSION 3.19)

project(basic_cpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(Boost_INCLUDE_DIR "/usr/local/include/boost")

include_directories(${Boost_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}/include")

link_directories(${Boost_LIBRARY_DIR})

add_executable(cpp17_test src/cpp17_test.cpp)

add_executable(bst src/bst-shared_ptr.cpp)

add_executable(constexpr src/constexpr_practise.cpp)