# VAAYU — Flight Routing & Reservation Engine

VAAYU is a modern C++ console application designed to simulate intelligent flight routing, fare calculation, and itinerary reservation. By leveraging Object-Oriented Programming (OOP) principles and graph traversal algorithms, VAAYU dynamically evaluates direct and connecting flight options from live CSV datasets while adjusting pricing based on passenger status, seat class, and travel priority.

---

## Key Features

* **Graph-Based Route Engine**: Uses Depth-First Search (DFS) on adjacency matrices to compute optimal paths across domestic and international airports, accounting for layovers and connection times.
* **Dynamic Fare Calculation**: Multiplies base fares according to selected seat tier (*Economy*, *Business*, *First Class*) and applies layover overheads.
* **Passenger Tier Perks**: Polymorphic passenger handling offers special discounts (e.g., 10% discount for `FrequentFlyer` members).
* **Live CSV Integration**: Loads route metadata dynamically from an external dataset (`flights.csv`), with automatic currency normalization (USD, SGD, HKD converted to INR).
* **Multi-Criteria Optimization**: Allows users to sort search results based on either **Time-Saving** (shortest duration) or **Economic** (lowest total cost) preferences.
* **Rich Console UI**: Features ANSI escape codes for vibrant terminal output and configures the Windows console UTF-8 code page (`CP_UTF8`) for clean aesthetic banners.

---

## System Architecture

The codebase adheres strictly to core Object-Oriented Programming (OOP) principles:

```
                  +-------------------+
                  |   NetworkGraph    |  (Base Graph Logic & Matrices)
                  +-------------------+
                            |
                            v
                  +-------------------+
                  |    VaayuSystem    |  (DFS Routing, CSV Parser, UI)
                  +-------------------+

      +---------------------------------------------------+
      |                    Passenger                      | (Abstract Base)
      +---------------------------------------------------+
               /                                 \
              v                                   v
    +-------------------+               +-------------------+
    |   FrequentFlyer   |               |       Guest       |
    +-------------------+               +-------------------+
    (10% Member Discount)               (Standard Pricing)
```

### Core Classes & Design

1. **`Airport`** *(Encapsulation)*
   * Stores airport metadata (IATA Code, City, ID) and flags distinguishing domestic vs. international terminals.
2. **`Passenger` Hierarchy** *(Inheritance & Polymorphism)*
   * **`Passenger`**: Abstract base class enforcing the pure virtual function `getFinalFare()`.
   * **`FrequentFlyer`**: Inherits from `Passenger`, applying a 10% discount on final fare totals.
   * **`Guest`**: Inherits from `Passenger`, implementing standard ticket pricing.
3. **`NetworkGraph`** *(Base Class)*
   * Manages adjacency matrices (`time_matrix`, `airline_matrix`, `fare_matrix`) for up to 100 graph nodes.
4. **`VaayuSystem`** *(Derived Class)*
   * Inherits from `NetworkGraph`. Responsible for reading `flights.csv`, building the network graph, parsing time strings, running the DFS pathfinding algorithm, and rendering output tables.

---

## Fare & Class Pricing Rules

### Seat Multipliers
| Class | Fare Multiplier |
| :--- | :--- |
| **Economy** | `1.00x` |
| **Business** | `2.45x` |
| **First Class** | `4.20x` |

### Passenger Discounts
* **Frequent Flyer**: `(Base Fare Total * Class Multiplier) * 0.90`
* **Guest**: `Base Fare Total * Class Multiplier`

### Layover & Fee Adjustments
* Base itinerary fee: **Rs. 1,500**
* Connection fee per layover: **Rs. 1,200**
* Connection duration overhead: **90 minutes per layover**

---

## `flights.csv` Dataset Format

The program expects a `flights.csv` file located in the working execution directory. The file should be structured as follows:

```csv
Origin_IATA,Destination_IATA,Airline,Routing,Distance,Duration,Fare,Currency
DEL,BOM,Air India,Direct,1148,2h 10m,4500,INR
DEL,SIN,Singapore Airlines,Direct,4150,5h 45m,350,SGD
BOM,JFK,Emirates,Connecting,12500,16h 30m,850,USD
```

### Supported Currencies & Auto-Conversion Rates
* **USD** $\rightarrow$ `83.50 INR`
* **SGD** $\rightarrow$ `61.80 INR`
* **HKD** $\rightarrow$ `10.65 INR`
* **INR** $\rightarrow$ Direct usage (`1.00`)

---

## Build and Execution Instructions

### Prerequisites
* **Operating System**: Windows (required for `<windows.h>` API calls).
* **Compiler**: MinGW / GCC, Clang, or MSVC supporting C++11 or higher.

### Compilation

Open your terminal or developer command prompt and compile `VAAYUnew.cpp`:

```bash
g++ -std=c++11 VAAYUnew.cpp -o vaayu.exe
```

### Running the Application

1. Ensure `flights.csv` is present in the same directory as the executable.
2. Launch the application:

```bash
./vaayu.exe
```

---

## Sample Interaction Workflow

1. **Start System**: Main menu loads and parses route data from `flights.csv`.
2. **Passenger Details**: Select passenger type (`1: Frequent Flyer`, `2: Guest`), enter wallet balance, select seat class, and choose travel domain (`Domestic` or `International`).
3. **Route Selection**: View filtered list of available origin and destination airports with associated display IDs.
4. **Sorting Preference**: Choose prioritization (`1: Time Saving`, `2: Economic`).
5. **Itinerary Render**: The system presents up to 3 recommended options with detailed routes, duration, layovers, total fare, and booking status depending on available wallet funds.

---
