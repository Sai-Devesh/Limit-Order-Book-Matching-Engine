# Low-Latency Limit Order Book Matching Engine

This project is a high-performance, C++ implementation of a limit order book (LOB) matching engine, the core component of any modern financial exchange. It is designed from the ground up with a focus on low-latency, efficiency, and robust C++ design patterns, simulating the performance-critical environment of high-frequency trading (HFT) systems.

The engine processes market orders (BUY/SELL) and maintains the book according to strict **Price-Time Priority**, executing trades when the spread is crossed.


*(Suggestion: Use a tool like [ScreenToGif](https://www.screentogif.com/) or Kap to record a short demo of you using the command line, and upload it to a site like Imgur to embed here.)*

## Key Features

- **Price-Time Priority Matching:** Orders are correctly prioritized first by price, then by time of arrival.
- **High-Performance Order Management:** Optimized for fast addition and cancellation of orders.
- **Efficient Trade Execution:** Aggressive orders can sweep multiple price levels in the book in a single operation.
- **Interactive CLI:** A simple command-line interface to add/cancel orders and view the live state of the order book.

## Core Design & Data Structures

The performance of this system is centered on a synergistic combination of three C++ STL data structures, each chosen for a specific task:

1.  **`std::map<double, Limit>` for Price Levels:**
    - **Purpose:** To store price levels and enforce **Price Priority**.
    - **Why:** `std::map` is a balanced binary search tree, providing **O(log P)** time complexity for lookups, insertions, and deletions, where `P` is the number of unique price levels. The bids map uses `std::greater` to sort from highest to lowest price, while the asks map uses the default `std::less`.

2.  **`std::list<Order>` for Time Priority:**
    - **Purpose:** To queue orders at the same price level, enforcing **Time Priority (FIFO)**.
    - **Why:** `std::list` provides O(1) insertion/deletion at either end. Crucially, it offers **iterator stability**, meaning iterators to other elements are not invalidated when an element is removed. This is essential for our fast cancellation mechanism.

3.  **`std::unordered_map<int, ...>` for Fast Cancellation:**
    - **Purpose:** To provide **O(1)** average-time lookup for any order by its unique ID.
    - **Why:** This is a classic **space-for-time trade-off**. By maintaining this index, we avoid a slow O(N) search through the entire book for cancellations. This is a critical optimization for any low-latency system.

## Performance Analysis

- **`N`**: Total number of orders on the book.
- **`P`**: Number of unique price levels.

| Operation | Time Complexity | Notes |
| :--- | :--- | :--- |
| **Add Order (No Match)** | **O(log P)** | Dominated by the `std::map` lookup for the price level. |
| **Cancel Order** | **O(1) average** | The `unordered_map` provides an instant lookup of the order's iterator. |
| **Match Order** | O(M log P + L) | `M` is matched price levels, `L` is matched orders. The engine efficiently sweeps the book. |
| **View Book** | O(P) | Linear scan of all price levels. |

| Complexity | Value | Notes |
| :--- | :--- | :--- |
| **Space Complexity** | **O(N)** | Memory usage scales linearly with the number of resting orders. |

## How to Build and Run

### Prerequisites
- A C++ compiler supporting C++17 (e.g., g++, Clang, MSVC).

### Compilation
Navigate to the project directory and run the following command:
```bash
g++ main.cpp -std=c++17 -o matching_engine
```

### Running the Application
```bash
./matching_engine
```

You will be greeted with an interactive prompt.

### Available Commands
- `BUY <shares> <price>` (e.g., `BUY 100 99.50`)
- `SELL <shares> <price>` (e.g., `SELL 50 100.50`)
- `CANCEL <order_id>` (e.g., `CANCEL 1`)
- `VIEW` (Displays the current order book)
- `EXIT`

---
*This project was developed as a deep dive into low-latency systems, data structure optimization, and modern C++ design patterns relevant to the quantitative finance industry.*
