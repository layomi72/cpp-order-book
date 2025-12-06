# C++ Limit Order Book

A high-performance limit order book implementation with price-time priority matching engine, built from scratch in C++17.

## Overview

This project implements a complete limit order book system that handles order matching, partial fills, and order cancellation. The matching engine follows standard exchange rules: orders are matched based on price-time priority, ensuring fair execution for all market participants.

## Features

- **Price-Time Priority Matching**: Orders are matched first by price, then by time of arrival
- **Partial Fill Support**: Orders can be partially filled across multiple price levels
- **Order Cancellation**: Fast O(1) order lookup and cancellation
- **Bid/Ask Management**: Separate handling of buy and sell orders with efficient price level organization
- **Spread Calculation**: Real-time bid-ask spread tracking

## Project Structure

```
cpp-order-book/
├── src/
│   ├── OrderBook.h          # OrderBook class definition
│   ├── OrderBook.cpp        # Core implementation
│   └── main.cpp             # Test suite and examples
├── CMakeLists.txt           # Build configuration
└── README.md                # This file
```

## Building

### Prerequisites
- C++17 compatible compiler (g++, clang++, or MSVC)
- CMake 3.10 or higher

### Build Instructions

**Using CMake:**
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

**On Windows (Visual Studio):**
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

**Using g++ directly:**
```bash
g++ -std=c++17 -Wall -Wextra -O2 src/*.cpp -o orderbook
```

## Running

After building, run the executable:
```bash
./orderbook        # Linux/Mac
orderbook.exe      # Windows
```

The program runs a comprehensive test suite demonstrating various order book operations.

## Implementation Details

### Data Structures

- **Price Levels**: `std::map<double, std::deque<Order*>>` for both bids and asks
  - Bids sorted in descending order (highest first)
  - Asks sorted in ascending order (lowest first)
- **Order Lookup**: `std::unordered_map<uint64_t, Order*>` for O(1) cancellation

### Matching Algorithm

The matching engine processes incoming orders as follows:

1. **Validation**: Checks for valid price and quantity
2. **Matching**: Attempts to match against the opposite side of the book
3. **Price-Time Priority**: Matches at best available price, then by FIFO within price level
4. **Partial Fills**: Handles orders that span multiple price levels
5. **Book Update**: Adds remaining quantity to the book if not fully filled

### Time Complexity

- **Add Order**: O(log n) for price level lookup + O(m) for matching (where m is number of price levels matched)
- **Cancel Order**: O(1) average case for lookup, O(k) for removal from deque (where k is orders at price level)
- **Best Bid/Ask**: O(1) using map iterators

## Example Usage

```cpp
#include "OrderBook.h"

OrderBook book;

// Add a buy order
book.addOrder(1, Side::BUY, 100.0, 10);

// Add a matching sell order
book.addOrder(2, Side::SELL, 100.0, 5);

// Check best bid and spread
double best_bid = book.getBestBid();
double spread = book.getSpread();

// Cancel an order
book.cancelOrder(1);

// Print current state
book.printBook();
```

## Testing

The included test suite covers:
- Basic order addition and matching
- Partial fills
- Order cancellation
- Price-time priority enforcement
- Edge cases (invalid inputs, duplicate IDs)
- Multiple price levels
- Spread crossing scenarios

## Future Enhancements

Potential improvements for production use:
- Market order support
- Multi-symbol order books
- Order modification (replace/cancel-replace)
- Performance profiling and optimization
- Thread-safe operations for concurrent access
- Advanced order types (iceberg, stop orders)
- Trade execution callbacks/events

## License

This project is open source and available for educational and portfolio purposes.
