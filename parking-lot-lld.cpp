#include <iostream>
#include <vector>

// Enums
enum VehicleType { TwoWheeler, FourWheeler };

// Vehicle class
class Vehicle {
public:
    int vehicleNo;
    VehicleType vehicleType;

    Vehicle(int vehicleNo, VehicleType vehicleType) 
        : vehicleNo(vehicleNo), vehicleType(vehicleType) {}
};

// ParkingSpot class
class ParkingSpot {
public:
    int id;
    bool isEmpty;
    Vehicle* vehicle;
    int price;

    ParkingSpot(int id, int price) 
        : id(id), isEmpty(true), vehicle(nullptr), price(price) {}

    void parkVehicle(Vehicle* v) {
        vehicle = v;
        isEmpty = false;
    }

    void removeVehicle() {
        vehicle = nullptr;
        isEmpty = true;
    }
};

// ParkingSpotManager class
class ParkingSpotManager {
protected:
    std::vector<ParkingSpot*> spots;

public:
    ParkingSpotManager(const std::vector<ParkingSpot*>& spots) : spots(spots) {}

    virtual ParkingSpot* findParkingSpace() = 0;

    void parkVehicle(Vehicle* v) {
        ParkingSpot* spot = findParkingSpace();
        if (spot) spot->parkVehicle(v);
    }

    void removeVehicle(Vehicle* v) {
        for (ParkingSpot* spot : spots) {
            if (spot->vehicle && spot->vehicle->vehicleNo == v->vehicleNo) {
                spot->removeVehicle();
                break;
            }
        }
    }

    virtual ~ParkingSpotManager() {}
};

// TwoWheelerManager and FourWheelerManager classes
class TwoWheelerManager : public ParkingSpotManager {
public:
    TwoWheelerManager(const std::vector<ParkingSpot*>& spots) : ParkingSpotManager(spots) {}

    ParkingSpot* findParkingSpace() override {
        // Implementation to find nearest parking spot for Two Wheelers
        return nullptr;
    }
};

class FourWheelerManager : public ParkingSpotManager {
public:
    FourWheelerManager(const std::vector<ParkingSpot*>& spots) : ParkingSpotManager(spots) {}

    ParkingSpot* findParkingSpace() override {
        // Implementation to find nearest parking spot for Four Wheelers
        return nullptr;
    }
};

// ParkingSpotManagerFactory class
class ParkingSpotManagerFactory {
public:
    ParkingSpotManager* getParkingSpotManager(VehicleType vehicleType, const std::vector<ParkingSpot*>& spots) {
        if (vehicleType == TwoWheeler)
            return new TwoWheelerManager(spots);
        else if (vehicleType == FourWheeler)
            return new FourWheelerManager(spots);
        else
            return nullptr; // Handle error
    }
};

// Ticket class
class Ticket {
public:
    long entryTime;
    ParkingSpot* parkingSpot;
    Vehicle* vehicle;

    Ticket(long entryTime, ParkingSpot* parkingSpot, Vehicle* vehicle)
        : entryTime(entryTime), parkingSpot(parkingSpot), vehicle(vehicle) {}
};

// EntranceGate class
class EntranceGate {
private:
    ParkingSpotManagerFactory factory;

public:
    EntranceGate(ParkingSpotManagerFactory factory) : factory(factory) {}

    ParkingSpot* findParkingSpace(VehicleType vehicleType, const std::vector<ParkingSpot*>& spots) {
        ParkingSpotManager* manager = factory.getParkingSpotManager(vehicleType, spots);
        ParkingSpot* spot = manager->findParkingSpace();
        delete manager;
        return spot;
    }

    Ticket* generateTicket(Vehicle* vehicle, ParkingSpot* parkingSpot) {
        // Implementation to generate ticket
        return nullptr;
    }
};

// ExitGate class
class ExitGate {
private:
    ParkingSpotManagerFactory factory;

public:
    ExitGate(ParkingSpotManagerFactory factory) : factory(factory) {}

    void removeVehicle(Ticket* ticket) {
        std::vector<ParkingSpot*> emptySpots;
        ParkingSpotManager* manager = factory.getParkingSpotManager(ticket->vehicle->vehicleType, emptySpots);
        manager->removeVehicle(ticket->vehicle);
        delete manager;
    }
};

int main() {
    // Initialize parking spots
    std::vector<ParkingSpot*> spots;
    for (int i = 1; i <= 100; ++i) {
        if (i <= 50)
            spots.push_back(new ParkingSpot(i, 10));
        else
            spots.push_back(new ParkingSpot(i, 20));
    }

    // Create ParkingSpotManagerFactory
    ParkingSpotManagerFactory factory;

    // Create EntranceGate and ExitGate objects
    EntranceGate entranceGate(factory);
    ExitGate exitGate(factory);

    // Example usage
    Vehicle* vehicle = new Vehicle(123, TwoWheeler);
    ParkingSpot* spot = entranceGate.findParkingSpace(vehicle->vehicleType, spots);
    Ticket* ticket = entranceGate.generateTicket(vehicle, spot);

    // Vehicle leaves
    exitGate.removeVehicle(ticket);

    // Cleanup
    delete vehicle;
    delete ticket;
    for (ParkingSpot* spot : spots) {
        delete spot;
    }

    return 0;
}