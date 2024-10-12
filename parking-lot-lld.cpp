#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

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
        if (spot) {
            spot->parkVehicle(v);
            std::cout << "Vehicle " << v->vehicleNo << " parked in spot " << spot->id << std::endl;
        } else {
            std::cout << "No available parking spots." << std::endl;
        }
    }

    void removeVehicle(Vehicle* v) {
        for (ParkingSpot* spot : spots) {
            if (spot->vehicle && spot->vehicle->vehicleNo == v->vehicleNo) {
                spot->removeVehicle();
                std::cout << "Vehicle " << v->vehicleNo << " removed from spot " << spot->id << std::endl;
                return;
            }
        }
        std::cout << "Vehicle " << v->vehicleNo << " not found in the parking lot." << std::endl;
    }

    virtual ~ParkingSpotManager() {}
};

// TwoWheelerManager and FourWheelerManager classes
class TwoWheelerManager : public ParkingSpotManager {
public:
    TwoWheelerManager(const std::vector<ParkingSpot*>& spots) : ParkingSpotManager(spots) {}

    ParkingSpot* findParkingSpace() override {
        return *std::find_if(spots.begin(), spots.end(), [](ParkingSpot* spot) { return spot->isEmpty; });
    }
};

class FourWheelerManager : public ParkingSpotManager {
public:
    FourWheelerManager(const std::vector<ParkingSpot*>& spots) : ParkingSpotManager(spots) {}

    ParkingSpot* findParkingSpace() override {
        return *std::find_if(spots.begin(), spots.end(), [](ParkingSpot* spot) { return spot->isEmpty; });
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
        long currentTime = std::time(0);
        return new Ticket(currentTime, parkingSpot, vehicle);
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

    int calculateFee(Ticket* ticket) {
        long exitTime = std::time(0);
        long duration = exitTime - ticket->entryTime;
        int hours = duration / 3600 + (duration % 3600 != 0); // Round up to the nearest hour
        return hours * ticket->parkingSpot->price;
    }
};

int main() {
    // Initialize parking spots
    std::vector<ParkingSpot*> twoWheelerSpots;
    std::vector<ParkingSpot*> fourWheelerSpots;
    for (int i = 1; i <= 100; ++i) {
        if (i <= 50)
            twoWheelerSpots.push_back(new ParkingSpot(i, 10));
        else
            fourWheelerSpots.push_back(new ParkingSpot(i, 20));
    }

    // Create ParkingSpotManagerFactory
    ParkingSpotManagerFactory factory;

    // Create EntranceGate and ExitGate objects
    EntranceGate entranceGate(factory);
    ExitGate exitGate(factory);

    // Example usage
    Vehicle* vehicle1 = new Vehicle(123, TwoWheeler);
    Vehicle* vehicle2 = new Vehicle(456, FourWheeler);

    // Park vehicles
    ParkingSpot* spot1 = entranceGate.findParkingSpace(vehicle1->vehicleType, twoWheelerSpots);
    Ticket* ticket1 = entranceGate.generateTicket(vehicle1, spot1);

    ParkingSpot* spot2 = entranceGate.findParkingSpace(vehicle2->vehicleType, fourWheelerSpots);
    Ticket* ticket2 = entranceGate.generateTicket(vehicle2, spot2);

    // Simulate time passing (3 hours)
    std::time_t threeHoursLater = std::time(0) + 10800; // 3 hours = 3 * 60 * 60 seconds
    std::time(&threeHoursLater);

    // Vehicles leave
    int fee1 = exitGate.calculateFee(ticket1);
    exitGate.removeVehicle(ticket1);
    std::cout << "Vehicle " << vehicle1->vehicleNo << " fee: $" << fee1 << std::endl;

    int fee2 = exitGate.calculateFee(ticket2);
    exitGate.removeVehicle(ticket2);
    std::cout << "Vehicle " << vehicle2->vehicleNo << " fee: $" << fee2 << std::endl;

    // Cleanup
    delete vehicle1;
    delete vehicle2;
    delete ticket1;
    delete ticket2;
    for (ParkingSpot* spot : twoWheelerSpots) {
        delete spot;
    }
    for (ParkingSpot* spot : fourWheelerSpots) {
        delete spot;
    }

    return 0;
}
