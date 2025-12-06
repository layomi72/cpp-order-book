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

# Add benchmark executable
add_executable(benchmark
    src/benchmark.cpp
    src/OrderBook.cpp
)

# Include directories
target_include_directories(orderbook PRIVATE src)
target_include_directories(benchmark PRIVATE src)

# Compiler flags (optional, for better warnings)
target_compile_options(orderbook PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_compile_options(benchmark PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O3  # Optimize for performance benchmarks
)

