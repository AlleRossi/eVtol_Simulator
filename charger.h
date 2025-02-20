#pragma once
#include <memory>
#include "Vehicle.h"

class Charger {
private:
    bool isAvailable;
    double chargeCompletionTime;
    Vehicle* currentVehicle;

public:
    Charger();

    bool isFree() const;

    void startCharging(Vehicle* vehicle, double currentTime);

    void update(double currentTime);
};