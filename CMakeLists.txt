cmake_minimum_required(VERSION 3.18)

project(basic_cpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})

    add_executable(boost_timer_sync src/boost/asio/timer_sync.cc)
    
    add_executable(boost_timer_async src/boost/asio/timer_async.cc)

    add_executable(boost_bind_arg src/boost/asio/bind_arg.cc)

    # target_link_libraries(boost_test LINK_PUBLIC ${Boost_LIBRARIES})

endif(Boost_FOUND)
    # set(Boost_INCLUDE_DIR "/usr/local/include/boost")    
  
    # include_directories(${Boost_INCLUDE_DIR})

include_directories("${CMAKE_SOURCE_DIR}/include")

add_executable(bst src/basic-ds/bst/bst.cc)

add_executable(btree src/basic-ds/btree/main.cc)

add_executable(stl src/stl/stl_test.cc)

add_executable(thread src/stl/stl_thread.cc)

add_executable(constexpr src/other/constexpr_practise.cc)

add_executable(cpp17 src/other/cpp17_test.cc)

add_executable(trail src/other/trail_test.cc)

add_executable(topo src/basic-ds/topological-sorting/topological-sorting.cc)

add_executable(rbtree src/basic-ds/rbtree/rbtree.cc
                        src/basic-ds/rbtree/main.cc)

add_executable(bplustree src/basic-ds/bplustree/bplustree.cc
                            src/basic-ds/bplustree/main.cc)

add_executable(hashjoin src/basic-ds/hashjoin/main.cc)

add_executable(modern src/book/modern_cpp_design.cc)
