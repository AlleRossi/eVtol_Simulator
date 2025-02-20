#ifndef VEHICLE_H
#define VHEICLE_H


#include <string>
#include <iostream>
#include <cstdlib>

class Vehicle {
protected:
    std::string companyName;
    double cruiseSpeed;       // mph
    double batteryCapacity;   // kWh
    double chargeTime;        // hours
    double energyUse;         // kWh/mile
    int passengerCount;
    double faultProbability;  // probability of fault per hour
    double batteryLevel;      // Current battery level (kWh)

public:
    Vehicle(std::string name, double speed, double capacity, double chargeTime,
        double energyUse, int passengers, double faultProb);

    virtual ~Vehicle() = default;  // enables default destructor for base class

    // Simulate flying for one hour and return distance traveled
    virtual double fly();

    // Check if a fault occurs during flight
    bool checkForFault();

    // Getters
    std::string getCompanyName() const;
    double getCruiseSpeed() const;
    double getBatteryLevel() const;
    int getPassengerCount() const;
    double getChargeTime() const;
};

#endif