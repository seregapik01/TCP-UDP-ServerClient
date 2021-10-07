cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDART 17)

project(testovoe_Pikulev)

#target_compile_features(myapp cxx_std_17)

add_executable( server server/server.cpp server/main_server.cpp server/utils.cpp)
add_executable( client client/client.cpp )
