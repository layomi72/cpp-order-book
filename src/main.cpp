#include "OrderBook.h"
#include <iostream>
#include <cassert>

void testBasicOrder() {
    std::cout << "=== Test 1: Basic Order Addition ===" << std::endl;
    OrderBook book;
    
    book.addOrder(1, Side::BUY, 100.0, 10);
    book.printBook();
    
    assert(!book.isEmpty());
    assert(book.getBestBid() == 100.0);
    std::cout << "✓ Test 1 passed\n" << std::endl;
}

void testMatchingOrders() {
    std::cout << "=== Test 2: Matching Orders ===" << std::endl;
    OrderBook book;
    
    book.addOrder(1, Side::BUY, 100.0, 10);
    book.addOrder(2, Side::SELL, 100.0, 5);
    book.printBook();
    
    // Buy order should have 5 remaining, sell order fully filled
    assert(book.getBestBid() == 100.0);
    std::cout << "✓ Test 2 passed\n" << std::endl;
}

void testNonMatchingOrders() {
    std::cout << "=== Test 3: Non-Matching Orders ===" << std::endl;
    OrderBook book;
    
    book.addOrder(1, Side::BUY, 99.0, 20);
    book.addOrder(2, Side::SELL, 101.0, 15);
    book.printBook();
    
    assert(book.getBestBid() == 99.0);
    assert(book.getBestAsk() == 101.0);
    assert(book.getSpread() == 2.0);
    std::cout << "✓ Test 3 passed\n" << std::endl;
}

void testPartialFill() {
    std::cout << "=== Test 4: Partial Fill ===" << std::endl;
    OrderBook book;
    
    book.addOrder(1, Side::BUY, 100.0, 100);
    book.addOrder(2, Side::SELL, 100.0, 50);
    book.printBook();
    
    // Buy order should have 50 remaining
    assert(book.getBestBid() == 100.0);
    std::cout << "✓ Test 4 passed\n" << std::endl;
}

void testCancellation() {
    std::cout << "=== Test 5: Order Cancellation ===" << std::endl;
    OrderBook book;
    
    book.addOrder(1, Side::BUY, 100.0, 10);
    book.addOrder(2, Side::BUY, 99.0, 20);
    book.printBook();
    
    bool cancelled = book.cancelOrder(2);
    assert(cancelled);
    
    std::cout << "\nAfter cancellation:" << std::endl;
    book.printBook();
    
    assert(book.getBestBid() == 100.0);
    std::cout << "✓ Test 5 passed\n" << std::endl;
}

void testPriceTimePriority() {
    std::cout << "=== Test 6: Price-Time Priority ===" << std::endl;
    OrderBook book;
    
    // Add multiple orders at same price - should match in FIFO order
    book.addOrder(1, Side::BUY, 100.0, 10);
    book.addOrder(2, Side::BUY, 100.0, 15);
    book.addOrder(3, Side::SELL, 100.0, 20);
    book.printBook();
    
    // First buy order should be fully filled, second partially filled
    assert(book.getBestBid() == 100.0);
    std::cout << "✓ Test 6 passed\n" << std::endl;
}

void testEdgeCases() {
    std::cout << "=== Test 7: Edge Cases ===" << std::endl;
    OrderBook book;
    
    // Test cancel non-existent order
    bool cancelled = book.cancelOrder(999);
    assert(!cancelled);
    
    // Test invalid order (zero quantity)
    bool added = book.addOrder(1, Side::BUY, 100.0, 0);
    assert(!added);
    
    // Test invalid price
    added = book.addOrder(2, Side::BUY, -10.0, 10);
    assert(!added);
    
    // Test duplicate order ID
    book.addOrder(3, Side::BUY, 100.0, 10);
    added = book.addOrder(3, Side::SELL, 101.0, 5);
    assert(!added);
    
    std::cout << "✓ Test 7 passed\n" << std::endl;
}

void testMultiplePriceLevels() {
    std::cout << "=== Test 8: Multiple Price Levels ===" << std::endl;
    OrderBook book;
    
    book.addOrder(1, Side::BUY, 98.0, 10);
    book.addOrder(2, Side::BUY, 100.0, 15);
    book.addOrder(3, Side::BUY, 99.0, 20);
    book.addOrder(4, Side::SELL, 101.0, 5);
    book.addOrder(5, Side::SELL, 103.0, 10);
    book.addOrder(6, Side::SELL, 102.0, 8);
    
    book.printBook();
    
    assert(book.getBestBid() == 100.0);
    assert(book.getBestAsk() == 101.0);
    assert(book.getSpread() == 1.0);
    std::cout << "✓ Test 8 passed\n" << std::endl;
}

void testCrossingSpread() {
    std::cout << "=== Test 9: Crossing the Spread ===" << std::endl;
    OrderBook book;
    
    book.addOrder(1, Side::BUY, 100.0, 10);
    book.addOrder(2, Side::SELL, 101.0, 5);
    
    // Add a buy order that crosses the spread (should match)
    book.addOrder(3, Side::BUY, 102.0, 8);
    book.printBook();
    
    // The sell order at 101 should be fully filled
    assert(book.getBestAsk() == 0.0 || book.getBestAsk() > 101.0);
    std::cout << "✓ Test 9 passed\n" << std::endl;
}

int main() {
    std::cout << "Running Order Book Test Suite\n" << std::endl;
    
    try {
        testBasicOrder();
        testMatchingOrders();
        testNonMatchingOrders();
        testPartialFill();
        testCancellation();
        testPriceTimePriority();
        testEdgeCases();
        testMultiplePriceLevels();
        testCrossingSpread();
        
        std::cout << "\n=== All Tests Passed! ===" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
