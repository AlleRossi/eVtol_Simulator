#include "charger.h"

class Charger {
private:
    bool isAvailable;
    double chargeCompletionTime;
    Vehicle* currentVehicle;
    double totalChargingTime;

public:
    Charger() : isAvailable(true), chargeCompletionTime(0), currentVehicle(nullptr), totalChargingTime(0) {}

    bool isFree() const { return isAvailable; }

    void startCharging(Vehicle* vehicle) {
        if (isAvailable) {
            isAvailable = false;
            currentVehicle = vehicle;
            chargeCompletionTime = currentVehicle->getChargeTime();
            totalChargingTime += currentVehicle->getChargeTime();
        }
    }

    void update(double step) {
        chargeCompletionTime -= step;
        if (!isAvailable && chargeCompletionTime <= 0) {
            isAvailable = true;
            currentVehicle = nullptr;
            chargeCompletionTime = 0;
        }
    }

    double getTotalChargingTime() {
        return totalChargingTime;
    }
};