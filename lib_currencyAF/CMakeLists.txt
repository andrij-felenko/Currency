cmake_minimum_required(VERSION 3.14)
project(lib_currencyAF VERSION 0.0.1)

set (currencyAF_h
    include/lib_currencyAF/currencyByDate.h
    include/lib_currencyAF/currencyData.h
    include/lib_currencyAF/currencyType.h
)

set (currencyAF_cpp
    currencyData.cpp
    
    currencyByDate.cpp
    currencyType.cpp
)

set (CMAKE_AUTORCC)
find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5Network)

add_library(lib_currencyAF SHARED ${currencyAF_h} ${currencyAF_cpp})

set_target_properties(lib_currencyAF PROPERTIES OUTPUT_NAME "currencyAF")
target_link_libraries(lib_currencyAF lib_baseAF Qt5::Core Qt5::Network)
target_include_directories(lib_currencyAF
    PUBLIC include
    PRIVATE include/lib_currencyAF
)
