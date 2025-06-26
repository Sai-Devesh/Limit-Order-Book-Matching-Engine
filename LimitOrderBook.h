#pragma once

#include "Order.h"
#include <map>
#include <list>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <sstream>

// Forward declaration: Tells the compiler that a class named MatchingEngine exists,
// without needing to include the full header. This is necessary for the friend declaration.
class MatchingEngine;

// Represents a single price level in the order book.
struct Limit {
    double price;
    std::list<Order> orders;
};

class LimitOrderBook {
    // By declaring MatchingEngine as a friend, we give it access to the private
    // members of this class (bids, asks, and orderMap). This is a clean way
    // to solve the tight coupling between the data structure and its logic handler.
    friend class MatchingEngine;

private:
    std::map<double, Limit, std::greater<double>> bids;
    std::map<double, Limit> asks;
    std::unordered_map<int, std::list<Order>::iterator> orderMap;

public:
    // Adds an order to the correct side of the book.
    void addOrder(const Order& order) {
        if (order.side == Side::BUY) {
            addOrderToMap(bids, order);
        } else {
            addOrderToMap(asks, order);
        }
    }

    // Cancels an existing order using the fast lookup map.
    void cancelOrder(int orderID) {
        auto it = orderMap.find(orderID);
        if (it == orderMap.end()) {
            std::cerr << "Error: Cannot cancel. Order ID " << orderID << " not found or already filled." << std::endl;
            return;
        }

        std::list<Order>::iterator orderIt = it->second;
        double price = orderIt->price;

        if (orderIt->side == Side::BUY) {
            auto& limit = bids.at(price);
            limit.orders.erase(orderIt);
            if (limit.orders.empty()) {
                bids.erase(price);
            }
        } else { // SELL side
            auto& limit = asks.at(price);
            limit.orders.erase(orderIt);
            if (limit.orders.empty()) {
                asks.erase(price);
            }
        }
        orderMap.erase(it);
    }

    // Displays a formatted view of the order book.
    void displayBook() const {
        std::cout << "\n=================================================================\n";
        std::cout << std::left << std::setw(35) << "           BIDS (BUY)" << " | " << "ASKS (SELL)\n";
        std::cout << "-----------------------------------------------------------------\n";

        auto bid_it = bids.begin();
        auto ask_it = asks.begin();

        while (bid_it != bids.end() || ask_it != asks.end()) {
            std::stringstream bid_ss;
            if (bid_it != bids.end()) {
                int total_shares = 0;
                for(const auto& order : bid_it->second.orders) {
                    total_shares += order.shares;
                }
                bid_ss << total_shares << " @ " << std::fixed << std::setprecision(2) << bid_it->first;
                ++bid_it;
            }
            std::cout << std::left << std::setw(35) << bid_ss.str();
            
            std::cout << " | ";

            std::stringstream ask_ss;
            if (ask_it != asks.end()) {
                int total_shares = 0;
                for(const auto& order : ask_it->second.orders) {
                    total_shares += order.shares;
                }
                ask_ss << total_shares << " @ " << std::fixed << std::setprecision(2) << ask_it->first;
                ++ask_it;
            }
            std::cout << std::left << std::setw(35) << ask_ss.str() << "\n";
        }
        std::cout << "=================================================================\n";
    }

    bool hasAsks() const { return !asks.empty(); }
    bool hasBids() const { return !bids.empty(); }
    Limit& getBestAsk() { return asks.begin()->second; }
    Limit& getBestBid() { return bids.begin()->second; }
    void removeBestBid() { bids.erase(bids.begin()); }
    void removeBestAsk() { asks.erase(asks.begin()); }

private:
    template <typename T>
    void addOrderToMap(T& map, const Order& order) {
        auto it = map.find(order.price);
        if (it == map.end()) {
            Limit newLimit;
            newLimit.price = order.price;
            newLimit.orders.push_back(order);
            auto mapIt = map.insert({order.price, newLimit});
            orderMap[order.orderID] = std::prev(mapIt.first->second.orders.end());
        } else {
            it->second.orders.push_back(order);
            orderMap[order.orderID] = std::prev(it->second.orders.end());
        }
    }
};