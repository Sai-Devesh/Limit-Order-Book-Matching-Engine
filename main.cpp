#include "MatchingEngine.h"
#include <bits/stdc++.h>

// Helper function to print a vector of trades to the console in a readable format.
void printTrades(const std::vector<Trade>& trades) {
    if (trades.empty()) {
        std::cout << ">> No trades executed." << std::endl;
    } else {
        std::cout << ">> " << trades.size() << " Trade(s) Executed:" << std::endl;
        for (const auto& trade : trades) {
            std::cout << "   - Trade: Buy Order " << trade.buyOrderID
                      << " & Sell Order " << trade.sellOrderID
                      << " | Quantity: " << trade.quantity
                      << " @ Price: " << trade.price << std::endl;
        }
    }
}

// Prints the welcome message and lists available commands.
void printHelp() {
    std::cout << "\n--- Limit Order Book Matching Engine ---\n";
    std::cout << "Commands:\n";
    std::cout << "  BUY <shares> <price>   (e.g., BUY 100 99.50)\n";
    std::cout << "  SELL <shares> <price>  (e.g., SELL 50 100.50)\n";
    std::cout << "  CANCEL <order_id>      (e.g., CANCEL 1)\n";
    std::cout << "  VIEW                   (to display the current order book)\n";
    std::cout << "  EXIT                   (to close the application)\n";
    std::cout << "----------------------------------------\n" << std::endl;
}

int main() {
    MatchingEngine engine;
    std::string line;

    printHelp();

    // Main application loop to read and process user commands.
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        
        if (line.empty()) {
            continue;
        }
        std::stringstream ss(line);
        std::string command;
        ss >> command;

        for (char &c : command) {
            c = toupper(c);
        }

        if (command == "EXIT") {
            break;
        }

        if (command == "BUY" || command == "SELL") {
            int shares;
            double price;
            // Input validation.
            if (!(ss >> shares >> price) || shares <= 0 || price <= 0) {
                std::cerr << "Error: Invalid format or non-positive values. Use: " << command << " <shares> <price>" << std::endl;
                continue;
            }
            
            Side side = (command == "BUY") ? Side::BUY : Side::SELL;
            Order order(0, side, shares, price);
            
            std::cout << ">> Processing " << command << " order for " << shares << " shares at " << price << " (ID will be assigned)" << std::endl;
            auto trades = engine.processOrder(order);
            printTrades(trades);

        } else if (command == "CANCEL") {
            int orderID;
            if (!(ss >> orderID) || orderID <= 0) {
                std::cerr << "Error: Invalid format or non-positive Order ID. Use: CANCEL <order_id>" << std::endl;
                continue;
            }
            engine.cancelOrder(orderID);

        } else if (command == "VIEW") {
            engine.viewBook();
        
        } else if (command == "HELP") {
            printHelp();
        } else {
            std::cerr << "Error: Unknown command '" << line << "'" << std::endl;
        }
    }

    std::cout << "\nExiting application." << std::endl;
    return 0;
}