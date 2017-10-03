cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(searcher)
include_directories(${PROJECT_SOURCE_DIR}/src)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "-std=c++1z -Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

file(GLOB searcher_SRC "src/*.cpp")
add_executable(searcher ${searcher_SRC})
target_link_libraries(searcher stdc++fs pthread)