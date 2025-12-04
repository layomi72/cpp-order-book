#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <map>
#include <queue>
#include <unordered_map>
#include <cstdint>

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
    // TODO: Decide on return type - bool, or return fill information?
    bool addOrder(uint64_t id, Side side, double price, uint64_t quantity);
    
    // Cancel an order by ID
    // Returns true if order was found and cancelled
    bool cancelOrder(uint64_t id);
    
    // Print the current state of the order book
    void printBook() const;
    
    // TODO: Consider adding helper methods:
    //   - getBestBid() const
    //   - getBestAsk() const
    //   - getSpread() const
    //   - isEmpty() const

private:
    // Price level storage: price -> queue of orders at that price
    // Bids: sorted descending (highest price first)
    // Asks: sorted ascending (lowest price first)
    std::map<double, std::queue<Order*>> bids_;
    std::map<double, std::queue<Order*>> asks_;
    
    // Fast lookup: order ID -> order pointer
    // Used for cancellation
    std::unordered_map<uint64_t, Order*> order_lookup_;
    
    // TODO: Consider adding:
    //   - uint64_t next_order_id_ (if auto-generating IDs)
    //   - uint64_t current_timestamp_ (if auto-generating timestamps)
    
    // Helper methods (implement these as you need them)
    
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
    // Challenge: std::queue doesn't support removal from middle!
    // Hint: You might need to change to std::deque
    bool removeOrderFromBook(Order* order);
    
    // Get best bid price (highest buy price)
    // Returns nullptr if no bids
    double getBestBidPrice() const;
    
    // Get best ask price (lowest sell price)
    // Returns nullptr if no asks
    double getBestAskPrice() const;
    
    // Clean up empty price levels
    void cleanupEmptyLevels();
};

#endif // ORDERBOOK_H

