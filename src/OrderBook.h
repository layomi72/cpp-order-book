#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <map>
#include <deque>
#include <unordered_map>
#include <cstdint>
#include <chrono>

enum class Side {
    BUY,
    SELL
};

struct Order{
    uint64_t id;
    Side side;
    double price;
    uint64_t quantity;
    uint64_t remaining_qty;
    uint64_t timestamp;

    Order(uint64_t id, Side side, double price, uint64_t quantity, uint64_t timestamp) : id(id), side(side), price(price), quantity(quantity), remaining_qty(quantity), timestamp(timestamp) {}
};

class OrderBook {
public:
    // Constructor
    OrderBook();
    
    // Destructor - clean up all orders
    ~OrderBook();
    
    // Add an order to the book
    // Returns true if order was added (or partially added after matching)
    bool addOrder(uint64_t id, Side side, double price, uint64_t quantity);
    
    // Cancel an order by ID
    // Returns true if order was found and cancelled
    bool cancelOrder(uint64_t id);
    
    // Print the current state of the order book
    void printBook() const;
    
    // Get best bid price (highest buy price)
    double getBestBid() const;
    
    // Get best ask price (lowest sell price)
    double getBestAsk() const;
    
    // Get current spread (best ask - best bid)
    double getSpread() const;
    
    // Check if order book is empty
    bool isEmpty() const;

private:
    // Price level storage: price -> deque of orders at that price
    // Bids: sorted descending (highest price first)
    // Asks: sorted ascending (lowest price first)
    std::map<double, std::deque<Order*>> bids_;
    std::map<double, std::deque<Order*>> asks_;
    
    // Fast lookup: order ID -> order pointer
    // Used for cancellation
    std::unordered_map<uint64_t, Order*> order_lookup_;
    
    // Timestamp counter for order sequencing
    uint64_t current_timestamp_;
    
    // Helper methods
    
    // Match a new order against the book
    // Returns remaining quantity after matching
    uint64_t matchOrder(Order* order);
    
    // Match a buy order against asks
    uint64_t matchBuyOrder(Order* order);
    
    // Match a sell order against bids
    uint64_t matchSellOrder(Order* order);
    
    // Add order to appropriate side (bids or asks)
    void addOrderToBook(Order* order);
    
    // Remove order from book (used by cancellation)
    bool removeOrderFromBook(Order* order);
    
    // Get best bid price (highest buy price)
    double getBestBidPrice() const;
    
    // Get best ask price (lowest sell price)
    double getBestAskPrice() const;
    
    // Clean up empty price levels
    void cleanupEmptyLevels();
};

#endif // ORDERBOOK_H

