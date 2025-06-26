#pragma once // Prevents the file from being included multiple times

#include <iostream>

// Using an enum class for type safety and readability.
// This is better than using a simple bool or int.
enum class Side { BUY, SELL };

struct Order {
    int orderID;    ///< Unique identifier for the order
    Side side;      ///< The side of the order (BUY or SELL)
    int shares;     ///< Number of shares for the order
    double price;   ///< Price per share

    // A simple constructor for easy initialization
    Order(int id, Side s, int sh, double p) 
        : orderID(id), side(s), shares(sh), price(p) {}
};

// A simple structure to represent an executed trade
struct Trade {
    int buyOrderID;
    int sellOrderID;
    int quantity;
    double price;

    Trade(int bID, int sID, int qty, double p)
        : buyOrderID(bID), sellOrderID(sID), quantity(qty), price(p) {}
};