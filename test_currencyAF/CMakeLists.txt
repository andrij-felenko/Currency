cmake_minimum_required(VERSION 3.19)
project(test_currencyAF)

find_package(Qt6Core)

add_executable(test_currencyAF main.cpp)
target_include_directories(test_currencyAF
    PUBLIC $<TARGET_PROPERTY:lib_currencyAF,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(test_currencyAF lib_currencyAF Qt6::Core)
