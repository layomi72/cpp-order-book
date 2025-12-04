#include "OrderBook.h"
#include <iostream>
#include <algorithm>

// TODO: Implement Side enum
// enum class Side { BUY, SELL };

// TODO: Implement Order struct
// struct Order { ... };

// Constructor
OrderBook::OrderBook() {
    // Initialize any member variables if needed
}

// Destructor
OrderBook::~OrderBook() {
    // TODO: Clean up all Order objects
    // Iterate through order_lookup_ and delete all orders
    // This prevents memory leaks
}

bool OrderBook::addOrder(uint64_t id, Side side, double price, uint64_t quantity) {
    // TODO: Step 1 - Validate inputs
    //   - Check price > 0
    //   - Check quantity > 0
    //   - Check order doesn't already exist (id not in order_lookup_)
    
    // TODO: Step 2 - Create new Order object
    //   Order* order = new Order(id, side, price, quantity, ...);
    //   Remember to set remaining_qty = quantity initially
    
    // TODO: Step 3 - Try to match the order first
    //   uint64_t remaining = matchOrder(order);
    //   Update order->remaining_qty = remaining;
    
    // TODO: Step 4 - If order still has remaining quantity, add to book
    //   if (remaining > 0) {
    //       addOrderToBook(order);
    //       order_lookup_[id] = order;
    //   } else {
    //       // Order fully filled, delete it
    //       delete order;
    //   }
    
    // TODO: Step 5 - Return success/failure
    return false; // Placeholder
}

bool OrderBook::cancelOrder(uint64_t id) {
    // TODO: Step 1 - Look up order in order_lookup_
    //   auto it = order_lookup_.find(id);
    //   if (it == order_lookup_.end()) return false;
    
    // TODO: Step 2 - Remove order from the book
    //   Order* order = it->second;
    //   bool removed = removeOrderFromBook(order);
    
    // TODO: Step 3 - Remove from order_lookup_
    //   order_lookup_.erase(it);
    
    // TODO: Step 4 - Delete the order object
    //   delete order;
    
    // TODO: Step 5 - Clean up empty price levels
    //   cleanupEmptyLevels();
    
    return false; // Placeholder
}

void OrderBook::printBook() const {
    std::cout << "=== ORDER BOOK ===" << std::endl;
    
    // TODO: Print BIDS (buy orders)
    //   - Iterate bids_ map in REVERSE (highest price first)
    //   - For each price level, sum up all order quantities
    //   - Print: Price    Total Quantity
    
    std::cout << "\nBIDS (Buy Orders):" << std::endl;
    std::cout << "Price\tQuantity" << std::endl;
    // TODO: Implement iteration
    // Hint: Use rbegin() and rend() for reverse iteration
    
    std::cout << "\nASKS (Sell Orders):" << std::endl;
    std::cout << "Price\tQuantity" << std::endl;
    // TODO: Print ASKS (sell orders)
    //   - Iterate asks_ map in FORWARD (lowest price first)
    //   - For each price level, sum up all order quantities
    //   - Print: Price    Total Quantity
    
    std::cout << "===================" << std::endl;
}

// TODO: Implement matchOrder()
// This should call matchBuyOrder() or matchSellOrder() based on side
uint64_t OrderBook::matchOrder(Order* order) {
    // TODO: Implement
    return 0;
}

// TODO: Implement matchBuyOrder()
// Algorithm:
//   1. While order has remaining_qty > 0:
//      a. Get best ask price (lowest sell price)
//      b. If no asks OR best ask > buy price: break (no match)
//      c. Get queue at best ask price
//      d. While queue not empty AND order has remaining_qty:
//         - Get front order from queue
//         - Calculate fill quantity (min of both)
//         - Update both orders' remaining_qty
//         - If front order fully filled: remove from queue and delete
//         - If front order partially filled: break (FIFO, can't skip)
//      e. If price level empty: remove from map
uint64_t OrderBook::matchBuyOrder(Order* order) {
    // TODO: Implement
    return 0;
}

// TODO: Implement matchSellOrder()
// Similar to matchBuyOrder but:
//   - Check best bid (highest buy price)
//   - Match if best bid >= sell price
uint64_t OrderBook::matchSellOrder(Order* order) {
    // TODO: Implement
    return 0;
}

// TODO: Implement addOrderToBook()
// Algorithm:
//   1. Choose bids_ or asks_ based on order->side
//   2. Check if price level exists
//   3. If exists: push order to existing queue
//   4. If not: create new queue and add to map
void OrderBook::addOrderToBook(Order* order) {
    // TODO: Implement
}

// TODO: Implement removeOrderFromBook()
// Challenge: std::queue doesn't support removal from middle!
// Solutions:
//   Option A: Change to std::deque, iterate to find and erase
//   Option B: Mark as cancelled, skip during matching (lazy deletion)
//   Option C: Rebuild queue without cancelled order
// 
// Recommended: Option A - change queue to deque
bool OrderBook::removeOrderFromBook(Order* order) {
    // TODO: Implement
    return false;
}

double OrderBook::getBestBidPrice() const {
    // TODO: Return highest bid price
    // If bids_ empty, return 0.0 or use optional
    if (bids_.empty()) return 0.0;
    return bids_.rbegin()->first; // Reverse iterator gives highest
}

double OrderBook::getBestAskPrice() const {
    // TODO: Return lowest ask price
    // If asks_ empty, return 0.0 or use optional
    if (asks_.empty()) return 0.0;
    return asks_.begin()->first; // Forward iterator gives lowest
}

void OrderBook::cleanupEmptyLevels() {
    // TODO: Remove any price levels that have empty queues
    // Iterate through bids_ and asks_, remove entries where queue is empty
}

