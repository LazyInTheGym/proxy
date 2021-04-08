cmake_minimum_required(VERSION 3.17)
project(proxy)

set(CMAKE_CXX_STANDARD 14)

add_executable(proxy main.cpp)

find_package(Boost 1.75 REQUIRED COMPONENTS
        date_time system thread regex serialization)

target_link_libraries(proxy
        Boost::date_time
        Boost::system
        Boost::thread
        Boost::regex
        Boost::serialization
        )
