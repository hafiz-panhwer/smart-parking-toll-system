# Smart Parking Lot & Toll Management System

A beginner-level C++ console project that manages a parking lot with a toll gate.
It assigns parking slots by vehicle type, prints tickets and receipts, and keeps a daily revenue record.
The project is built to show core Object-Oriented Programming (OOP) concepts.

## Features

- **Vehicle entry**: assigns a free slot and prints a parking ticket
- **Exit billing**: calculates the parking fee plus the toll tax and prints a receipt
- **Live parking map**: shows every slot as free or occupied, with a count of free slots per type
- **Vehicle / ticket search**: find a parked vehicle by ticket ID or vehicle number
- **Daily revenue report**: total vehicles served and total revenue
- **File handling (fstream)**: tickets and revenue are saved to text files and loaded again on the next run

## OOP Concepts Used

| Concept | Where |
|---|---|
| Abstraction | `Vehicle` is an abstract class with pure virtual functions |
| Inheritance | `Bike`, `Car` and `Truck` inherit from `Vehicle` |
| Polymorphism | Fees are calculated through a `Vehicle*`, giving a different result for each type |
| Encapsulation | `ParkingSpot` keeps its data private, with getters and setters |
| Composition | A `Ticket` has a `Vehicle` |
| Operator Overloading | `RevenueReport` overloads `+=` to add a payment |

## Fee Table

| Vehicle | Slot Type | Toll Tax | Parking Fee |
|---|---|---|---|
| Bike | Compact | Rs. 30 | Rs. 20 / hour |
| Car | Standard | Rs. 80 | Rs. 50 / hour |
| Truck | Oversized | Rs. 150 | Rs. 100 / hour |

## How to Compile and Run

```bash
g++ main.cpp -o parking
./parking
```
