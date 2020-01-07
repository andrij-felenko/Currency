cmake_minimum_required(VERSION 3.14)
project(server_currencyAF)

set (server_currencyAF_src
    currencyHttpServer.cpp
    currencyHttpServer.h
    main.cpp
    serverHost.cpp
    serverHost.h
    )

find_package(Qt5Core)
find_package(Qt5Network)

add_executable(server_currencyAF ${server_currencyAF_src})
target_link_libraries(server_currencyAF lib_currencyAF qt_http_server Qt5::Core Qt5::Network)
target_include_directories(server_currencyAF
    PUBLIC $<TARGET_PROPERTY:lib_currencyAF,INTERFACE_INCLUDE_DIRECTORIES>
    PUBLIC $<TARGET_PROPERTY:qt_http_server,INTERFACE_INCLUDE_DIRECTORIES>
)
