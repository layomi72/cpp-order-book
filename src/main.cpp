#include "OrderBook.h"
#include <iostream>

// This is a test file to help you verify your implementation
// Add your own test cases here as you develop

int main() {
    OrderBook book;
    
    std::cout << "=== Testing Order Book ===\n" << std::endl;
    
    // TODO: Test Case 1 - Add a single buy order
    std::cout << "Test 1: Add buy order" << std::endl;
    // book.addOrder(1, Side::BUY, 100.0, 10);
    // book.printBook();
    
    // TODO: Test Case 2 - Add a matching sell order
    std::cout << "\nTest 2: Add matching sell order" << std::endl;
    // book.addOrder(2, Side::SELL, 100.0, 5);
    // book.printBook();
    
    // TODO: Test Case 3 - Add non-matching orders
    std::cout << "\nTest 3: Add non-matching orders" << std::endl;
    // book.addOrder(3, Side::BUY, 99.0, 20);
    // book.addOrder(4, Side::SELL, 101.0, 15);
    // book.printBook();
    
    // TODO: Test Case 4 - Test partial fills
    std::cout << "\nTest 4: Partial fill" << std::endl;
    // book.addOrder(5, Side::BUY, 100.0, 100);
    // book.addOrder(6, Side::SELL, 100.0, 50);  // Should partially fill buy order
    // book.printBook();
    
    // TODO: Test Case 5 - Test cancellation
    std::cout << "\nTest 5: Cancel order" << std::endl;
    // book.cancelOrder(3);
    // book.printBook();
    
    // TODO: Test Case 6 - Test price-time priority
    std::cout << "\nTest 6: Price-time priority" << std::endl;
    // Add multiple orders at same price, verify FIFO
    
    // TODO: Test Case 7 - Test edge cases
    std::cout << "\nTest 7: Edge cases" << std::endl;
    // - Cancel non-existent order
    // - Add order with zero quantity
    // - Add order with negative price
    
    return 0;
}

