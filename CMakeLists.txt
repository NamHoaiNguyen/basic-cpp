cmake_minimum_required(VERSION 3.19)

project(basic_cpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost)
if(Boost_FOUND)
    # set(Boost_INCLUDE_DIR "/usr/local/include/boost")    
  
    include_directories(${Boost_INCLUDE_DIR})

    include_directories("${CMAKE_SOURCE_DIR}/include")

    # link_directories(${Boost_LIBRARY_DIR})

    add_executable(bst src/basic-ds/bst/bst.cc)

    add_executable(btree src/basic-ds/btree/main.cc)

    # add_subdirectory(src/stl)
    add_executable(stl src/stl/stl_test.cc)

    add_executable(thread src/stl/stl_thread.cc)

endif()

# add_executable(cpp17_test src/cpp17_test.cpp)

# add_executable(bst src/bst-shared_ptr.cpp)

# add_executable(constexpr src/constexpr_practise.cpp)