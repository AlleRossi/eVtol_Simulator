#include "vehicle.h"

class Vehicle {
protected:
	std::string companyName;
	double cruiseSpeed;			//Mph
	double batteryCapacity;		//kWh
	double chargeTime;			//hours
	double energyUse;			//kWh/mile
	int passengerCount;			//passengers capacity
	double faultProbability;		//probability of fault per hour

public:
	Vehicle(std::string name, double speed, double capacity, double chargeTime,
		double energyUsage, int passasngers, double faultProb)
		: companyName(name), cruiseSpeed(speed), batteryCapacity(capacity), chargeTime(chargeTime),
		energyUse(energyUsage), passengerCount(passasngers), faultProbability(faultProb) {}

	virtual ~Vehicle() = default;	//sets default destructor for Vehicle OBJ

	virtual double fly() {			//returns numbers of miles flown on a single charge
		return (batteryCapacity / energyUse) * cruiseSpeed;
	};

	virtual bool checkForFault() {	//return a bool indication if a fault has occurred while flying
		return ((rand() % 100) < (faultProbability * 100));
	};

	//Getters
	std::string getCompanyName() const { return companyName; }
	double getCruiseSpeed() const { return cruiseSpeed; }
	double getBatteryLevel() const { return batteryCapacity; }
	int getPassengerCount() const { return passengerCount; }
	double getChargeTime() const { return chargeTime; }
};
