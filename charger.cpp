#include "charger.h"

class Charger {
private:
    bool isAvailable;
    double chargeCompletionTime;
    Vehicle* currentVehicle;

public:
    Charger() : isAvailable(true), chargeCompletionTime(0), currentVehicle(nullptr) {}

    bool isFree() const { return isAvailable; }

    void startCharging(Vehicle* vehicle, double currentTime) {
        if (isAvailable) {
            isAvailable = false;
            currentVehicle = vehicle;
            chargeCompletionTime = currentTime + vehicle->getChargeTime();
        }
    }

    void update(double currentTime) {
        if (!isAvailable && currentTime >= chargeCompletionTime) {
            isAvailable = true;
            currentVehicle = nullptr;
        }
    }
};