#include "charger.h"


Charger::Charger(): isAvailable(true), chargeCompletionTime(0), currentVehicle(nullptr), totalChargingTime(0) {}

bool Charger::isFree() const { return isAvailable; }

void Charger::startCharging(Vehicle* vehicle) {
    if (isAvailable) {
        isAvailable = false;
        currentVehicle = vehicle;
        chargeCompletionTime = currentVehicle->getChargeTime();
        totalChargingTime += currentVehicle->getChargeTime();
    }
}

void Charger::update(double step) {
    chargeCompletionTime -= step;
    if (!isAvailable && chargeCompletionTime <= 0) {
        isAvailable = true;
        currentVehicle->setChargingStatus(0);
        currentVehicle = nullptr;
        chargeCompletionTime = 0;
    }
}

double Charger::getTotalChargingTime() {
    return totalChargingTime;
}
