#include "MatchingEngine.h"
#include <bits/stdc++.h>
using namespace std;
// Helper function to print a vector of trades to the console in a readable format.
void printTrades(const vector<Trade>& trades) {
    if (trades.empty()) {
        cout << ">> No trades executed." << endl;
    } else {
        cout << ">> " << trades.size() << " Trade(s) Executed:" << endl;
        for (const auto& trade : trades) {
            cout << "   - Trade: Buy Order " << trade.buyOrderID
                      << " & Sell Order " << trade.sellOrderID
                      << " | Quantity: " << trade.quantity
                      << " @ Price: " << trade.price << endl;
        }
    }
}

// Prints the welcome message and lists available commands.
void printHelp() {
    cout << "\n--- Limit Order Book Matching Engine ---\n";
    cout << "Commands:\n";
    cout << "  BUY <shares> <price>   (e.g., BUY 100 99.50)\n";
    cout << "  SELL <shares> <price>  (e.g., SELL 50 100.50)\n";
    cout << "  CANCEL <order_id>      (e.g., CANCEL 1)\n";
    cout << "  VIEW                   (to display the current order book)\n";
    cout << "  EXIT                   (to close the application)\n";
    cout << "----------------------------------------\n" << endl;
}

int main() {
    MatchingEngine engine;
    string line;

    printHelp();

    // Main application loop to read and process user commands.
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) {
            break;
        }
        
        if (line.empty()) {
            continue;
        }
        stringstream ss(line);
        string command;
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
                cerr << "Error: Invalid format or non-positive values. Use: " << command << " <shares> <price>" << endl;
                continue;
            }
            
            Side side = (command == "BUY") ? Side::BUY : Side::SELL;
            Order order(0, side, shares, price);
            
            cout << ">> Processing " << command << " order for " << shares << " shares at " << price << " (ID will be assigned)" << endl;
            auto trades = engine.processOrder(order);
            printTrades(trades);

        } else if (command == "CANCEL") {
            int orderID;
            if (!(ss >> orderID) || orderID <= 0) {
                cerr << "Error: Invalid format or non-positive Order ID. Use: CANCEL <order_id>" << endl;
                continue;
            }
            engine.cancelOrder(orderID);

        } else if (command == "VIEW") {
            engine.viewBook();
        
        } else if (command == "HELP") {
            printHelp();
        } else {
            cerr << "Error: Unknown command '" << line << "'" << endl;
        }
    }

    cout << "\nExiting application." << endl;
    return 0;
}