#include "charger.h"

Charger::Charger(): isAvailable(true), chargeCompletionTime(0), currentVehicle(nullptr), totalChargingTime(0) {}

bool Charger::isFree() const { return isAvailable; }

void Charger::startCharging(Vehicle* vehicle) {                //starts the charging process by updating the status of the charger and saves the projected charging time for the current vehicle
    if (isAvailable) {
        isAvailable = false;
        currentVehicle = vehicle;
        chargeCompletionTime = currentVehicle->getChargeTime();
        totalChargingTime += currentVehicle->getChargeTime();
    }
}

void Charger::update(double step, double currentTime) {         //updates the current conditions of the charger and the status of the vehicle in charge
    if(chargeCompletionTime > 0) chargeCompletionTime -= step;
    if (!isAvailable && chargeCompletionTime <= 0) {
        isAvailable = true;
        currentVehicle->setChargingStatus(0);
        currentVehicle->chargeComplete();
        //std::cout << "vehicle is now done charging " << currentVehicle->getCompanyName()<< " at time " << currentTime << "\n";      //debug and log
        currentVehicle = nullptr;
        chargeCompletionTime = 0;
    }
}

double Charger::getTotalChargingTime() {       //returns the total time the charger was being used
    return totalChargingTime;
}

//getter for the current vehicle
Vehicle* Charger::getCurrentVehicle() {
    return currentVehicle;
}
