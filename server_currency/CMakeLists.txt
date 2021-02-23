cmake_minimum_required(VERSION 3.19)
project(server_currencyAF)

set (server_currencyAF_src
    currencyHttpServer.cpp
    currencyHttpServer.h
    main.cpp
    serverHost.cpp
    serverHost.h
    )

find_package(Qt6 COMPONENTS Core Network)

add_executable(server_currencyAF ${server_currencyAF_src} rc.qrc)
target_link_libraries(server_currencyAF lib_currencyAF qt_http_server Qt6::Core Qt6::Network)
target_include_directories(server_currencyAF
    PUBLIC $<TARGET_PROPERTY:lib_currencyAF,INTERFACE_INCLUDE_DIRECTORIES>
    PUBLIC $<TARGET_PROPERTY:qt_http_server,INTERFACE_INCLUDE_DIRECTORIES>
)
