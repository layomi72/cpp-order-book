#include "OrderBook.h"
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>

/**
 * Performance benchmark for OrderBook.
 * 
 * Measures throughput (orders per second) for various operations:
 * - Order addition
 * - Order matching
 * - Order cancellation
 * 
 * This demonstrates production-level performance awareness.
 */

class Benchmark {
private:
    OrderBook book_;
    std::mt19937 rng_;
    std::uniform_real_distribution<double> price_dist_;
    std::uniform_int_distribution<uint64_t> qty_dist_;
    std::uniform_int_distribution<int> side_dist_;
    
public:
    Benchmark() 
        : rng_(std::random_device{}()),
          price_dist_(100.0, 200.0),
          qty_dist_(1, 1000),
          side_dist_(0, 1) {}
    
    double benchmarkAddOrders(size_t num_orders) {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < num_orders; ++i) {
            double price = price_dist_(rng_);
            uint64_t qty = qty_dist_(rng_);
            Side side = (side_dist_(rng_) == 0) ? Side::BUY : Side::SELL;
            
            book_.addOrder(i, side, price, qty);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        return (num_orders * 1000000.0) / duration.count(); // orders per second
    }
    
    double benchmarkMatching(size_t num_orders) {
        // Pre-populate book with orders
        for (size_t i = 0; i < num_orders / 2; ++i) {
            book_.addOrder(i, Side::BUY, 100.0 + (i % 10), 100);
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Add matching sell orders
        for (size_t i = num_orders / 2; i < num_orders; ++i) {
            book_.addOrder(i, Side::SELL, 100.0 + ((i - num_orders/2) % 10), 50);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        return ((num_orders / 2) * 1000000.0) / duration.count();
    }
    
    double benchmarkCancellation(size_t num_orders) {
        // Pre-populate book
        for (size_t i = 0; i < num_orders; ++i) {
            book_.addOrder(i, Side::BUY, 100.0, 100);
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Cancel every other order
        for (size_t i = 0; i < num_orders; i += 2) {
            book_.cancelOrder(i);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        return ((num_orders / 2) * 1000000.0) / duration.count();
    }
    
    void reset() {
        book_ = OrderBook();
    }
};

void printBenchmarkResults(const std::string& operation, double throughput, size_t num_ops) {
    std::cout << std::left << std::setw(25) << operation 
              << std::right << std::setw(12) << std::fixed << std::setprecision(0) 
              << throughput << " ops/sec"
              << " (" << num_ops << " operations)" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  OrderBook Performance Benchmark" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    Benchmark bench;
    
    // Warm-up run (to avoid cold start effects)
    bench.benchmarkAddOrders(1000);
    bench.reset();
    
    std::cout << "Benchmark Results:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    // Benchmark order addition
    const size_t num_orders = 100000;
    double add_throughput = bench.benchmarkAddOrders(num_orders);
    printBenchmarkResults("Order Addition", add_throughput, num_orders);
    bench.reset();
    
    // Benchmark matching
    double match_throughput = bench.benchmarkMatching(num_orders);
    printBenchmarkResults("Order Matching", match_throughput, num_orders / 2);
    bench.reset();
    
    // Benchmark cancellation
    double cancel_throughput = bench.benchmarkCancellation(num_orders);
    printBenchmarkResults("Order Cancellation", cancel_throughput, num_orders / 2);
    
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "\nPerformance Summary:" << std::endl;
    std::cout << "  - Order addition: " << std::fixed << std::setprecision(0) 
              << add_throughput << " orders/sec" << std::endl;
    std::cout << "  - Order matching: " << match_throughput << " matches/sec" << std::endl;
    std::cout << "  - Cancellation: " << cancel_throughput << " cancels/sec" << std::endl;
    
    std::cout << "\nNote: Results may vary based on hardware and compiler optimizations." << std::endl;
    std::cout << "      Compile with -O3 for production performance." << std::endl;
    
    return 0;
}

