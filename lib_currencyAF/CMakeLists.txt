cmake_minimum_required(VERSION 3.14)
project(lib_currencyAF VERSION 0.0.1)

set (currencyAF_h
    include/currencyByDate.h
    include/currencyType.h
    include/currencyUpdater.h
)

set (currencyAF_cpp
    currencyByDate.cpp
    currencyType.cpp
    currencyUpdater.cpp
)

set (CMAKE_AUTORCC)
find_package(Qt5Core)
find_package(Qt5Network)

add_library(lib_currencyAF SHARED ${currencyAF_h} ${currencyAF_cpp})

set_target_properties(lib_currencyAF PROPERTIES OUTPUT_NAME "currencyAF")
target_include_directories(lib_currencyAF PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(lib_currencyAF lib_baseAF Qt5::Core Qt5::Network)
