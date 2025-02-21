#pragma once
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
    bool isCharging;          // current charging status of vehicle
    bool isIdle;              // current usage status of vehicle
    double startIdleTime;     // start idle time (simulation time the vehicle joins charging queue and completes charging cycle)  

public:
    Vehicle(std::string name, double speed, double capacity, double chargeTime,
        double energyUse, int passengers, double faultProb);

    virtual ~Vehicle() = default;  //enables default destructor for base class

    //simulate flying for one time step and return distance traveled
    virtual double fly(double step);

    //check if a fault occurs during flight
    bool checkForFault();

    //update the battery level 
    virtual void updateBatteryLevel(double batteryUsed);

    //change charging status to meet current vehicle conditions
    virtual void setChargingStatus(bool status);

    //charge of vehicle completed
    virtual void chargeComplete();

    //change idle status
    virtual void setIdleStatus(bool status);

    //update startIdleTime
    virtual void updateStartIdleTime(double currentTime);

    //reset start Idle Time
    virtual void resetStartIdleTime();

    //Getters
    std::string getCompanyName() const;
    double getCruiseSpeed() const;
    double getBatteryLevel() const;
    int getPassengerCount() const;
    double getChargeTime() const;
    double getEnergyUse() const;
    bool getIsCharging();
    bool getIsIdle();
    double getStartIdleTime();

};
