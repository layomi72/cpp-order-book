#include "OrderBook.h"
#include <iostream>
#include <algorithm>

// Constructor
OrderBook::OrderBook() : current_timestamp_(0) {
    // Initialize timestamp counter
}

// Destructor - clean up all orders
OrderBook::~OrderBook() {
    // Clean up all Order objects to prevent memory leaks
    for (auto& pair : order_lookup_) {
        delete pair.second;
    }
}

bool OrderBook::addOrder(uint64_t id, Side side, double price, uint64_t quantity) {
    // Validate inputs
    if (price <= 0.0 || quantity == 0) {
        return false;
    }
    
    // Check if order already exists
    if (order_lookup_.find(id) != order_lookup_.end()) {
        return false;
    }
    
    // Create new Order object with current timestamp
    uint64_t timestamp = current_timestamp_++;
    Order* order = new Order(id, side, price, quantity, timestamp);
    
    // Try to match the order first
    uint64_t remaining = matchOrder(order);
    order->remaining_qty = remaining;
    
    // If order still has remaining quantity, add to book
    if (remaining > 0) {
        addOrderToBook(order);
        order_lookup_[id] = order;
        return true;
    } else {
        // Order fully filled, delete it
        delete order;
        return true;
    }
}

bool OrderBook::cancelOrder(uint64_t id) {
    // Look up order in order_lookup_
    auto it = order_lookup_.find(id);
    if (it == order_lookup_.end()) {
        return false;
    }
    
    // Remove order from the book
    Order* order = it->second;
    bool removed = removeOrderFromBook(order);
    
    if (!removed) {
        return false;
    }
    
    // Remove from order_lookup_
    order_lookup_.erase(it);
    
    // Delete the order object
    delete order;
    
    // Clean up empty price levels
    cleanupEmptyLevels();
    
    return true;
}

void OrderBook::printBook() const {
    std::cout << "=== ORDER BOOK ===" << std::endl;
    
    // Print BIDS (buy orders) - highest price first
    std::cout << "\nBIDS (Buy Orders):" << std::endl;
    std::cout << "Price\tQuantity" << std::endl;
    
    if (bids_.empty()) {
        std::cout << "(empty)" << std::endl;
    } else {
        // Iterate in reverse (highest price first)
        for (auto it = bids_.rbegin(); it != bids_.rend(); ++it) {
            double price = it->first;
            uint64_t total_qty = 0;
            
            // Sum up all order quantities at this price level
            for (Order* order : it->second) {
                total_qty += order->remaining_qty;
            }
            
            std::cout << price << "\t" << total_qty << std::endl;
        }
    }
    
    // Print ASKS (sell orders) - lowest price first
    std::cout << "\nASKS (Sell Orders):" << std::endl;
    std::cout << "Price\tQuantity" << std::endl;
    
    if (asks_.empty()) {
        std::cout << "(empty)" << std::endl;
    } else {
        // Iterate forward (lowest price first)
        for (const auto& pair : asks_) {
            double price = pair.first;
            uint64_t total_qty = 0;
            
            // Sum up all order quantities at this price level
            for (Order* order : pair.second) {
                total_qty += order->remaining_qty;
            }
            
            std::cout << price << "\t" << total_qty << std::endl;
        }
    }
    
    std::cout << "===================" << std::endl;
}

double OrderBook::getBestBid() const {
    return getBestBidPrice();
}

double OrderBook::getBestAsk() const {
    return getBestAskPrice();
}

double OrderBook::getSpread() const {
    double best_bid = getBestBidPrice();
    double best_ask = getBestAskPrice();
    
    if (best_bid == 0.0 || best_ask == 0.0) {
        return 0.0;
    }
    
    return best_ask - best_bid;
}

bool OrderBook::isEmpty() const {
    return bids_.empty() && asks_.empty();
}

// Match a new order against the book
uint64_t OrderBook::matchOrder(Order* order) {
    if (order->side == Side::BUY) {
        return matchBuyOrder(order);
    } else {
        return matchSellOrder(order);
    }
}

// Match a buy order against asks
uint64_t OrderBook::matchBuyOrder(Order* order) {
    uint64_t remaining = order->remaining_qty;
    
    while (remaining > 0) {
        // Get best ask price (lowest sell price)
        if (asks_.empty()) {
            break;
        }
        
        double best_ask = getBestAskPrice();
        
        // If best ask > buy price, no match
        if (best_ask > order->price) {
            break;
        }
        
        // Get queue at best ask price
        auto& ask_queue = asks_[best_ask];
        
        // Process orders at this price level
        while (!ask_queue.empty() && remaining > 0) {
            Order* ask_order = ask_queue.front();
            
            // Calculate fill quantity (min of both)
            uint64_t fill_qty = std::min(remaining, ask_order->remaining_qty);
            
            // Update both orders' remaining_qty
            remaining -= fill_qty;
            ask_order->remaining_qty -= fill_qty;
            
            // If ask order fully filled: remove from queue and delete
            if (ask_order->remaining_qty == 0) {
                ask_queue.pop_front();
                order_lookup_.erase(ask_order->id);
                delete ask_order;
            } else {
                // If ask order partially filled: break (FIFO, can't skip)
                break;
            }
        }
        
        // If price level empty: remove from map
        if (ask_queue.empty()) {
            asks_.erase(best_ask);
        }
    }
    
    return remaining;
}

// Match a sell order against bids
uint64_t OrderBook::matchSellOrder(Order* order) {
    uint64_t remaining = order->remaining_qty;
    
    while (remaining > 0) {
        // Get best bid price (highest buy price)
        if (bids_.empty()) {
            break;
        }
        
        double best_bid = getBestBidPrice();
        
        // If best bid < sell price, no match
        if (best_bid < order->price) {
            break;
        }
        
        // Get queue at best bid price
        auto& bid_queue = bids_[best_bid];
        
        // Process orders at this price level
        while (!bid_queue.empty() && remaining > 0) {
            Order* bid_order = bid_queue.front();
            
            // Calculate fill quantity (min of both)
            uint64_t fill_qty = std::min(remaining, bid_order->remaining_qty);
            
            // Update both orders' remaining_qty
            remaining -= fill_qty;
            bid_order->remaining_qty -= fill_qty;
            
            // If bid order fully filled: remove from queue and delete
            if (bid_order->remaining_qty == 0) {
                bid_queue.pop_front();
                order_lookup_.erase(bid_order->id);
                delete bid_order;
            } else {
                // If bid order partially filled: break (FIFO, can't skip)
                break;
            }
        }
        
        // If price level empty: remove from map
        if (bid_queue.empty()) {
            bids_.erase(best_bid);
        }
    }
    
    return remaining;
}

// Add order to appropriate side (bids or asks)
void OrderBook::addOrderToBook(Order* order) {
    if (order->side == Side::BUY) {
        // Add to bids map
        bids_[order->price].push_back(order);
    } else {
        // Add to asks map
        asks_[order->price].push_back(order);
    }
}

// Remove order from book (used by cancellation)
bool OrderBook::removeOrderFromBook(Order* order) {
    if (order->side == Side::BUY) {
        auto it = bids_.find(order->price);
        if (it != bids_.end()) {
            auto& deque = it->second;
            // Find and remove the order from deque
            auto deque_it = std::find(deque.begin(), deque.end(), order);
            if (deque_it != deque.end()) {
                deque.erase(deque_it);
                return true;
            }
        }
    } else {
        auto it = asks_.find(order->price);
        if (it != asks_.end()) {
            auto& deque = it->second;
            // Find and remove the order from deque
            auto deque_it = std::find(deque.begin(), deque.end(), order);
            if (deque_it != deque.end()) {
                deque.erase(deque_it);
                return true;
            }
        }
    }
    
    return false;
}

double OrderBook::getBestBidPrice() const {
    if (bids_.empty()) {
        return 0.0;
    }
    // Reverse iterator gives highest price
    return bids_.rbegin()->first;
}

double OrderBook::getBestAskPrice() const {
    if (asks_.empty()) {
        return 0.0;
    }
    // Forward iterator gives lowest price
    return asks_.begin()->first;
}

void OrderBook::cleanupEmptyLevels() {
    // Remove any price levels that have empty deques
    auto bid_it = bids_.begin();
    while (bid_it != bids_.end()) {
        if (bid_it->second.empty()) {
            bid_it = bids_.erase(bid_it);
        } else {
            ++bid_it;
        }
    }
    
    auto ask_it = asks_.begin();
    while (ask_it != asks_.end()) {
        if (ask_it->second.empty()) {
            ask_it = asks_.erase(ask_it);
        } else {
            ++ask_it;
        }
    }
}
