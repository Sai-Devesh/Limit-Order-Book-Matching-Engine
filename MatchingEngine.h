#pragma once

#include "LimitOrderBook.h" // Includes the full definition now
#include <vector>
#include <functional>
#include <algorithm>

class MatchingEngine {
private:
    LimitOrderBook book;
    int nextOrderID = 1;

public:
    std::vector<Trade> processOrder(Order& newOrder) {
        newOrder.orderID = nextOrderID++;
        std::vector<Trade> trades;

        if (newOrder.side == Side::BUY) {
            match(newOrder, 
                  [this]() -> Limit& { return book.getBestAsk(); }, 
                  [this]() -> bool { return book.hasAsks(); }, 
                  [this]() { book.removeBestAsk(); }, 
                  trades);
        } else {
            match(newOrder, 
                  [this]() -> Limit& { return book.getBestBid(); },
                  [this]() -> bool { return book.hasBids(); },
                  [this]() { book.removeBestBid(); },
                  trades);
        }

        if (newOrder.shares > 0) {
            book.addOrder(newOrder);
        }

        return trades;
    }

    void cancelOrder(int orderID) {
        book.cancelOrder(orderID);
    }

    void viewBook() const {
        book.displayBook();
    }

private:
    void match(Order& newOrder, 
               std::function<Limit&()> getBest, 
               std::function<bool()> hasOrders, 
               std::function<void()> removeBest, 
               std::vector<Trade>& trades) 
    {
        while (newOrder.shares > 0 && hasOrders() && isMatch(newOrder, getBest().price)) {
            Limit& bestLevel = getBest();
            std::list<Order>& ordersAtBestLevel = bestLevel.orders;

            while (!ordersAtBestLevel.empty() && newOrder.shares > 0) {
                Order& bookOrder = ordersAtBestLevel.front();
                
                int tradeQuantity = std::min(newOrder.shares, bookOrder.shares);
                double tradePrice = bookOrder.price;

                if (newOrder.side == Side::BUY) {
                    trades.emplace_back(newOrder.orderID, bookOrder.orderID, tradeQuantity, tradePrice);
                } else {
                    trades.emplace_back(bookOrder.orderID, newOrder.orderID, tradeQuantity, tradePrice);
                }

                newOrder.shares -= tradeQuantity;
                bookOrder.shares -= tradeQuantity;

                if (bookOrder.shares == 0) {
                    // *** THIS IS THE FIX ***
                    // We must access orderMap through our 'book' object.
                    // This is now legal because MatchingEngine is a friend of LimitOrderBook.
                    book.orderMap.erase(bookOrder.orderID);
                    ordersAtBestLevel.pop_front();
                }
            }

            if (ordersAtBestLevel.empty()) {
                removeBest();
            }
        }
    }

    bool isMatch(const Order& newOrder, double bookPrice) const {
        if (newOrder.side == Side::BUY) {
            return newOrder.price >= bookPrice;
        } else {
            return newOrder.price <= bookPrice;
        }
    }
};