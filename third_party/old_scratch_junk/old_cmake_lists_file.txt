# TO RUN THIS: 
# mkdir -p build
# cd build
# cmake ..
# cmake --build .
# ./reals

cmake_minimum_required(VERSION 3.16)

project(hott_reals_construction LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(reals
    reals_construction.cpp
)

# target_include_directories(reals PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(reals PRIVATE
    -Wall
    -Wextra
    -O3
)
