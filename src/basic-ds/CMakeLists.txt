set(Boost_INCLUDE_DIR "/usr/local/include/boost")

include_directories(${Boost_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}/include")

link_directories(${Boost_LIBRARY_DIR})

add_executable(bst bst/bst.cpp)

add_executable(btree btree/main.cpp)

add_executable(topo topological_sorting/topological_sorting.cc)
