// Smart Parking Lot & Toll Management System
// Beginner level OOP project in C++
// OOP used: Abstraction, Inheritance, Polymorphism, Encapsulation,
//           Composition, Operator Overloading
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// =====================================================
// 1. Vehicle - ABSTRACT class (has pure virtual functions)
// =====================================================
class Vehicle {
protected:
    string regNo;
public:
    Vehicle(string r) { regNo = r; }
    string getRegNo() { return regNo; }
    virtual ~Vehicle() {}      // needed when deleting through Vehicle*

    // pure virtual = child classes MUST write these
    virtual double calculateParkingFee(int hours) = 0;
    virtual double calculateTollFee() = 0;
    virtual string getTypeName() = 0;
    virtual int getTypeCode() = 0;
};

// =====================================================
// 2. Bike, Car, Truck - INHERITANCE + OVERRIDING
// =====================================================
class Bike : public Vehicle {
public:
    Bike(string r) : Vehicle(r) {}
    double calculateParkingFee(int hours) { return hours * 20; }
    double calculateTollFee() { return 30; }
    string getTypeName() { return "Bike"; }
    int getTypeCode() { return 1; }
};

class Car : public Vehicle {
public:
    Car(string r) : Vehicle(r) {}
    double calculateParkingFee(int hours) { return hours * 50; }
    double calculateTollFee() { return 80; }
    string getTypeName() { return "Car"; }
    int getTypeCode() { return 2; }
};

class Truck : public Vehicle {
public:
    Truck(string r) : Vehicle(r) {}
    double calculateParkingFee(int hours) { return hours * 100; }
    double calculateTollFee() { return 150; }
    string getTypeName() { return "Truck"; }
    int getTypeCode() { return 3; }
};

// makes the correct vehicle object from the type number
Vehicle* makeVehicle(int type, string reg) {
    if (type == 1) return new Bike(reg);
    if (type == 2) return new Car(reg);
    return new Truck(reg);
}

// =====================================================
// 3. ParkingSpot - ENCAPSULATION (private data + getters/setters)
// =====================================================
class ParkingSpot {
private:
    int id;
    int type;          // 1 = Compact, 2 = Standard, 3 = Oversized
    bool occupied;
public:
    ParkingSpot() { id = 0; type = 0; occupied = false; }
    void setSpot(int i, int t) { id = i; type = t; occupied = false; }
    int getId() { return id; }
    int getType() { return type; }
    bool isOccupied() { return occupied; }
    void setOccupied(bool o) { occupied = o; }
};

// =====================================================
// 4. Ticket - COMPOSITION (Ticket HAS-A Vehicle)
// =====================================================
class Ticket {
public:
    int ticketId;
    Vehicle* vehicle;      // the vehicle inside this ticket
    int slotId;
    int entryHour;
    int exitHour;
    double total;
    bool active;

    Ticket() {
        ticketId = 0; vehicle = NULL; slotId = 0;
        entryHour = 0; exitHour = 0; total = 0; active = false;
    }
};

// =====================================================
// 5. RevenueReport - OPERATOR OVERLOADING (+=)
// =====================================================
class RevenueReport {
public:
    double totalRevenue;
    int totalVehicles;
    RevenueReport() { totalRevenue = 0; totalVehicles = 0; }

    void operator+=(double amount) {
        totalRevenue = totalRevenue + amount;
        totalVehicles++;
    }
};

// =====================================================
// 6. ParkingTollSystem - main controller class
// =====================================================
class ParkingTollSystem {
private:
    ParkingSpot spots[18];      // 5 Compact + 10 Standard + 3 Oversized
    Ticket tickets[100];
    int ticketCount;
    int nextTicketId;
    RevenueReport report;

    string slotName(int t) {
        if (t == 1) return "Compact";
        if (t == 2) return "Standard";
        return "Oversized";
    }

    int findFreeSpot(int type) {
        for (int i = 0; i < 18; i++)
            if (spots[i].getType() == type && spots[i].isOccupied() == false)
                return i;
        return -1;
    }

    int findByReg(string reg) {
        for (int i = 0; i < ticketCount; i++)
            if (tickets[i].active && tickets[i].vehicle->getRegNo() == reg)
                return i;
        return -1;
    }

    int findById(int id) {
        for (int i = 0; i < ticketCount; i++)
            if (tickets[i].active && tickets[i].ticketId == id)
                return i;
        return -1;
    }

    int askAndFind() {
        int c;
        cout << "1. By Ticket ID\n2. By Vehicle Number\nChoice: ";
        cin >> c;
        if (c == 1) {
            int id;
            cout << "Ticket ID: ";
            cin >> id;
            return findById(id);
        }
        string reg;
        cout << "Vehicle Number: ";
        cin >> reg;
        return findByReg(reg);
    }

public:
    ParkingTollSystem() {
        ticketCount = 0;
        nextTicketId = 1001;
        for (int i = 0; i < 18; i++) {
            if (i < 5) spots[i].setSpot(i + 1, 1);
            else if (i < 15) spots[i].setSpot(i + 1, 2);
            else spots[i].setSpot(i + 1, 3);
        }
        loadData();
    }

    ~ParkingTollSystem() {
        for (int i = 0; i < ticketCount; i++)
            delete tickets[i].vehicle;
    }

    // ---------- File handling ----------
    void loadData() {
        ifstream rf("revenue.txt");
        if (rf) {
            rf >> report.totalRevenue >> report.totalVehicles >> nextTicketId;
            rf.close();
        }
        ifstream tf("tickets.txt");
        if (tf) {
            int id, type, slot, entry, exitH, act;
            double total;
            string reg;
            while (tf >> id >> reg >> type >> slot >> entry >> exitH >> total >> act) {
                tickets[ticketCount].ticketId = id;
                tickets[ticketCount].vehicle = makeVehicle(type, reg);
                tickets[ticketCount].slotId = slot;
                tickets[ticketCount].entryHour = entry;
                tickets[ticketCount].exitHour = exitH;
                tickets[ticketCount].total = total;
                tickets[ticketCount].active = (act == 1);
                if (act == 1) {
                    for (int i = 0; i < 18; i++)
                        if (spots[i].getId() == slot)
                            spots[i].setOccupied(true);
                }
                ticketCount++;
            }
            tf.close();
        }
    }

    void saveData() {
        ofstream tf("tickets.txt");
        for (int i = 0; i < ticketCount; i++) {
            tf << tickets[i].ticketId << " "
               << tickets[i].vehicle->getRegNo() << " "
               << tickets[i].vehicle->getTypeCode() << " "
               << tickets[i].slotId << " "
               << tickets[i].entryHour << " "
               << tickets[i].exitHour << " "
               << tickets[i].total << " "
               << tickets[i].active << endl;
        }
        tf.close();

        ofstream rf("revenue.txt");
        rf << report.totalRevenue << " " << report.totalVehicles << " " << nextTicketId << endl;
        rf.close();
    }

    // ---------- Vehicle entry ----------
    void vehicleEntry() {
        string reg;
        int type, hour;

        cout << "Vehicle Number: ";
        cin >> reg;
        if (findByReg(reg) != -1) {
            cout << "Error: This vehicle is already parked!\n";
            return;
        }
        cout << "Type (1 = Bike, 2 = Car, 3 = Truck): ";
        cin >> type;
        if (type < 1 || type > 3) {
            cout << "Error: Wrong vehicle type!\n";
            return;
        }
        cout << "Entry hour (0-23): ";
        cin >> hour;
        if (hour < 0 || hour > 23) {
            cout << "Error: Wrong hour!\n";
            return;
        }
        int s = findFreeSpot(type);
        if (s == -1) {
            cout << "Alert: Parking FULL for this vehicle type!\n";
            return;
        }
        if (ticketCount >= 100) {
            cout << "Ticket record is full!\n";
            return;
        }

        spots[s].setOccupied(true);
        tickets[ticketCount].ticketId = nextTicketId;
        tickets[ticketCount].vehicle = makeVehicle(type, reg);
        tickets[ticketCount].slotId = spots[s].getId();
        tickets[ticketCount].entryHour = hour;
        tickets[ticketCount].active = true;

        cout << "\n--- PARKING TICKET ---\n";
        cout << "Ticket ID : " << nextTicketId << endl;
        cout << "Vehicle   : " << reg << " (" << tickets[ticketCount].vehicle->getTypeName() << ")\n";
        cout << "Slot No   : " << spots[s].getId() << " (" << slotName(type) << ")\n";
        cout << "Entry hour: " << hour << endl;

        ticketCount++;
        nextTicketId++;
        saveData();
    }

    // ---------- Vehicle exit + billing ----------
    void vehicleExit() {
        int t = askAndFind();
        if (t == -1) {
            cout << "Error: Ticket not found!\n";
            return;
        }
        int exitHour;
        cout << "Exit hour (0-23): ";
        cin >> exitHour;
        if (exitHour < tickets[t].entryHour || exitHour > 23) {
            cout << "Error: Exit time cannot be before entry time!\n";
            return;
        }
        int hours = exitHour - tickets[t].entryHour;
        if (hours == 0) hours = 1;      // minimum 1 hour

        // POLYMORPHISM: same function call, different result for Bike/Car/Truck
        double parking = tickets[t].vehicle->calculateParkingFee(hours);
        double toll = tickets[t].vehicle->calculateTollFee();
        double total = parking + toll;

        tickets[t].exitHour = exitHour;
        tickets[t].total = total;
        tickets[t].active = false;
        report += total;                // uses overloaded +=

        for (int i = 0; i < 18; i++)
            if (spots[i].getId() == tickets[t].slotId)
                spots[i].setOccupied(false);

        cout << "\n====== RECEIPT ======\n";
        cout << "Ticket ID   : " << tickets[t].ticketId << endl;
        cout << "Vehicle     : " << tickets[t].vehicle->getRegNo()
             << " (" << tickets[t].vehicle->getTypeName() << ")\n";
        cout << "Hours       : " << hours << endl;
        cout << "Parking Fee : Rs. " << parking << endl;
        cout << "Toll Tax    : Rs. " << toll << endl;
        cout << "TOTAL       : Rs. " << total << endl;
        saveData();
    }

    // ---------- Parking map ----------
    void showMap() {
        int freeCount[4] = {0, 0, 0, 0};
        int totalCount[4] = {0, 0, 0, 0};
        cout << "\n--- PARKING MAP ---\n";
        for (int i = 0; i < 18; i++) {
            int ty = spots[i].getType();
            totalCount[ty]++;
            cout << "Slot " << spots[i].getId() << " (" << slotName(ty) << "): ";
            if (spots[i].isOccupied()) {
                for (int j = 0; j < ticketCount; j++)
                    if (tickets[j].active && tickets[j].slotId == spots[i].getId())
                        cout << "Occupied by " << tickets[j].vehicle->getRegNo();
                cout << endl;
            } else {
                cout << "Free\n";
                freeCount[ty]++;
            }
        }
        for (int t = 1; t <= 3; t++)
            cout << slotName(t) << ": " << freeCount[t] << " free out of " << totalCount[t] << endl;
    }

    // ---------- Search ----------
    void searchVehicle() {
        int t = askAndFind();
        if (t == -1) {
            cout << "No active entry found.\n";
            return;
        }
        cout << "Ticket ID : " << tickets[t].ticketId << endl;
        cout << "Vehicle   : " << tickets[t].vehicle->getRegNo()
             << " (" << tickets[t].vehicle->getTypeName() << ")\n";
        cout << "Slot No   : " << tickets[t].slotId << endl;
        cout << "Entry hour: " << tickets[t].entryHour << endl;
    }

    // ---------- Revenue report ----------
    void showReport() {
        cout << "\n--- DAILY REVENUE ---\n";
        cout << "Vehicles served: " << report.totalVehicles << endl;
        cout << "Total revenue  : Rs. " << report.totalRevenue << endl;
    }

    // ---------- Menu ----------
    void run() {
        int choice = -1;
        while (choice != 0) {
            cout << "\n===== SMART PARKING & TOLL SYSTEM =====\n";
            cout << "1. Vehicle Entry\n";
            cout << "2. Vehicle Exit\n";
            cout << "3. Parking Map\n";
            cout << "4. Search Vehicle\n";
            cout << "5. Revenue Report\n";
            cout << "0. Exit\n";
            cout << "Choice: ";
            cin >> choice;
            if (choice == 1) vehicleEntry();
            else if (choice == 2) vehicleExit();
            else if (choice == 3) showMap();
            else if (choice == 4) searchVehicle();
            else if (choice == 5) showReport();
            else if (choice != 0) cout << "Wrong choice!\n";
        }
        saveData();
        cout << "Data saved. Bye!\n";
    }
};

int main() {
    ParkingTollSystem system;
    system.run();
    return 0;
}
