cmake_minimum_required(VERSION 3.10)
project(OrderBook)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(orderbook
    src/main.cpp
    src/OrderBook.cpp
)

# Include directories
target_include_directories(orderbook PRIVATE src)

# Compiler flags (optional, for better warnings)
target_compile_options(orderbook PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

