#include <iostream>
using namespace std;

class DriveStrategy {
public:
    virtual void drive() = 0;
    virtual ~DriveStrategy() {}
};

class XyzDrive: public DriveStrategy {
public:
    void drive() override {
        cout << "Drive from XyzDrive\n";
    }
};

class SpecialDrive: public DriveStrategy {
public:
    void drive() override {
        cout << "Drive from SpecialDrive\n";
    }
};

class Vehicle {
protected:
    DriveStrategy* driveObject;
public:
    Vehicle() {
        driveObject = nullptr;
    }
    void drive() {
        driveObject->drive();
    }
    virtual ~Vehicle() {
        delete driveObject;
    }
};

class SportyVehicle: public Vehicle {
public: 
    SportyVehicle() {
        driveObject = new XyzDrive();
    }
};

class OffRoadVehicle: public Vehicle {
public:
    OffRoadVehicle() {
        driveObject = new SpecialDrive();
    }
};

int main() {
    Vehicle* sporty = new SportyVehicle();
    Vehicle* offroad = new OffRoadVehicle();
    cout << "Sporty vehicle: ";
    sporty->drive();
    cout << "Off-road vehicle: ";
    offroad->drive();
    delete sporty;
    delete offroad;
    return 0;
}