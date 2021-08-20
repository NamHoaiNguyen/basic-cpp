# set(Boost_INCLUDE_DIR "/usr/local/include/boost")

# include_directories(${Boost_INCLUDE_DIR})
# include_directories("${CMAKE_SOURCE_DIR}/include")

# link_directories(${Boost_LIBRARY_DIR})

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_executable(stl stl_test.cc)

target_link_libraries(stl ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}) 

