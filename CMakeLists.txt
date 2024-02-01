cmake_minimum_required(VERSION 3.5)

project(test)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

set(BOOST_ROOT "D:\\Boost\\boost_1_82_0")               # Specify the path to the Boost root
set(BOOST_INCLUDEDIR "D:\\Boost\\boost_1_82_0\\libs")   # Specify the path to the Boost libs

find_package(Boost 1.82.0 REQUIRED)                     # Specify the boost version

add_executable(main main.cpp)

target_link_libraries(main PUBLIC Boost::boost)