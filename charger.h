#pragma once
#include <memory>
#include "Vehicle.h"

class Charger {
private:
    bool isAvailable;
    double chargeCompletionTime;
    Vehicle* currentVehicle;
    double totalChargingTime;

public:
    Charger();

    bool isFree() const;

    void startCharging(Vehicle* vehicle);

    void update(double step, double currentTime);

    double getTotalChargingTime();
};