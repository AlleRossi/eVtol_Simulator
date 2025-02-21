#pragma once
#include <memory>
#include "Vehicle.h"

class Charger {
private:
    bool isAvailable;               //variable to define the status of the charger: free / busy
    double chargeCompletionTime;    //time necessary to complete charging process of current vehicle
    Vehicle* currentVehicle;        //pointer to the current vehicle being charged
    double totalChargingTime;       //total time this charger has been used in this simulation

public:
    Charger();                      //constructor

    bool isFree() const;            //getter function for the current charger status 

    void startCharging(Vehicle* vehicle);       //starts the charging process by updating the status of the charger and saves the projected charging time for the current vehicle

    void update(double step, double currentTime);       //updates the current conditions of the charger and the status of the vehicle in charge

    double getTotalChargingTime();              //returns the total amount of time the charger has been used 

    //getter for the current vehicle
    Vehicle* getCurrentVehicle();               //returns a pointer to the current vehicle being charged
};