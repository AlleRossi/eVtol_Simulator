#include "vehicle.h"


Vehicle::Vehicle(std::string name, double speed, double capacity, double chargeTime,
	double energyUsage, int passasngers, double faultProb)
	: companyName(name), cruiseSpeed(speed), batteryCapacity(capacity), chargeTime(chargeTime),
	energyUse(energyUsage), passengerCount(passasngers), faultProbability(faultProb),
	batteryLevel(capacity), isCharging(false), isIdle(true), startIdleTime(0.0) {}


double Vehicle::fly(double step) {			//returns numbers of miles flown in a time delta
	return step * cruiseSpeed;
};

bool Vehicle::checkForFault() {	//return a bool indication if a fault has occurred while flying
	return ((rand() % 100) < (faultProbability * 100));
};

void Vehicle::updateBatteryLevel(double batteryUsed) {	//update battery level after flight
	batteryLevel -= batteryUsed;
	if (batteryLevel < 0) batteryLevel = 0;
	//debug
	//std::cout << batteryLevel << " ";
}

//change charging status to meet current vehicle conditions
void Vehicle::setChargingStatus(bool status) {
	isCharging = status;
}

//charge of vehicle completed
void Vehicle::chargeComplete() {
	batteryLevel = batteryCapacity;
}

//change idle status
void Vehicle::setIdleStatus(bool status) {
	isIdle = status;
}

//update startIdleTime
void Vehicle::updateStartIdleTime(double currentTime) {
	startIdleTime = currentTime;
}

//reset startIdleTime
void Vehicle::resetStartIdleTime() {
	startIdleTime = 0;
}

//Getters
std::string Vehicle::getCompanyName() const { return companyName; }
double Vehicle::getCruiseSpeed() const { return cruiseSpeed; }
double Vehicle::getBatteryLevel() const { return batteryLevel; }
int Vehicle::getPassengerCount() const { return passengerCount; }
double Vehicle::getChargeTime() const { return chargeTime; }
double Vehicle::getEnergyUse() const { return energyUse; }
bool Vehicle::getIsCharging() { return isCharging; }
bool Vehicle::getIsIdle() { return isIdle; }
double Vehicle::getStartIdleTime() { return startIdleTime; }

