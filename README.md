# C++ Limit Order Book Implementation

A learning project to build a limit order book and matching engine from scratch.

## Project Structure

```
c++ Order Book/
├── src/
│   ├── OrderBook.h          # Header file with class definition
│   ├── OrderBook.cpp        # Implementation (you fill this in!)
│   └── main.cpp             # Test file
├── IMPLEMENTATION_GUIDE.md  # Detailed step-by-step guide
├── CMakeLists.txt           # Build configuration
└── README.md                 # This file
```

## Getting Started

### Prerequisites
- C++17 compatible compiler (g++, clang++)
- CMake (optional, or use Makefile)

### Building

**Using CMake:**
```bash
mkdir build
cd build
cmake ..
make
./orderbook
```

**Using g++ directly:**
```bash
g++ -std=c++17 -Wall -Wextra src/*.cpp -o orderbook
./orderbook
```

## Learning Path

1. **Read** `IMPLEMENTATION_GUIDE.md` thoroughly
2. **Start** with Phase 1: Foundation (Order struct, basic class setup)
3. **Implement** incrementally, one feature at a time
4. **Test** each feature before moving to the next
5. **Refactor** as you learn better approaches

## Key Concepts to Master

- Price-time priority matching
- FIFO queue behavior
- Partial fills
- Memory management in C++
- STL containers (map, queue, unordered_map)
- Reverse iteration for sorted data

## Tips

- Don't try to implement everything at once
- Test frequently with simple cases
- Use a debugger to step through your code
- Read the STL documentation for each container you use
- Understand the time complexity of each operation

## Next Steps After Completion

- Add market orders
- Implement multi-symbol support
- Add performance profiling
- Explore multi-threading
- Add order types (iceberg, stop orders)

Good luck! 🚀

