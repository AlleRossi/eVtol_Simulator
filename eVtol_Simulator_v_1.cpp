// eVtol_Simulator_v_1.cpp: definisce il punto di ingresso dell'applicazione.
//

#include <iostream>
#include <random>
#include <memory>
#include <vector>
#include <queue>
#include <map>
#include <chrono>
#include <cmath>
#include "eVtol_Factory.h"
#include "charger.h"
const double EPSILON = 1e-9;

using namespace std;

class Simulation {
private:

	std::vector<std::unique_ptr<Vehicle>> vehicles;		//vector of smart pointers that point to Vehicle OBJs
	std::vector<Charger> chargers;						//vector of Chargers
	std::queue<Vehicle*> chargeQueue;					//queue for the vehicles that need charging
	std::vector<std::string> companyNames				//names of the five EVTOL manufacturers of the simulation
		= { "Alpha", "Bravo", "Charlie", "Delta", "Echo" };				

	double simulationTime = 3.0;							//3 hours
	double timeStep = 0.01;								//time step for each iteration of the simulation loop

	//statistics tracking for each EVTOL manufacturer 
	std::map<std::string, double> totalFlightTime;
	std::map<std::string, double> totalDistanceTraveled;
	std::map<std::string, int> totalFaults;
	std::map<std::string, double> totalPassengerMiles;

public:

	Simulation(int numVehicles, int numChargers) {		//function that initializes the simulation
		srand(time(0));									//set the seed for the random number generation

		//create #numVehicles EVTOLS at random 
		for (int i = 0; i < numVehicles; i++) {
			std::string company = companyNames[rand() % companyNames.size()];	//a company name is selected at random from the list of possible companies
			vehicles.push_back(eVtol_Factory::createVehicle(company));				//add to vehicles an instance of the selected company's EVTOL
		}

		//create #chargers 
		for (int i = 0; i < numChargers; i++) {
			chargers.emplace_back();					//creates an instance of a charger OBJ and adds it to chargers
		}
	}

	void run() {
		double TimeElapsed = 0.0;
		int debugCounter = 0;
		int debugFaultCounter = 0;

		while (TimeElapsed <= simulationTime) {
			//debug
			//std::cout << "time elapsed value: " << TimeElapsed  << "  floored value of time elapsed" << static_cast<int>(TimeElapsed) << "\n";
			//flight phase
			for (auto& vehicle : vehicles) {
				if (vehicle->getBatteryLevel() > 0) {
					//debug
					//cout << vehicle->getBatteryLevel() << " ";
					double distance = vehicle->fly(timeStep);
					totalFlightTime[vehicle->getCompanyName()] += timeStep;			//update flight time per company
					totalDistanceTraveled[vehicle->getCompanyName()] += distance;	//update flight distance epr company

					//compute passenger miles
					totalPassengerMiles[vehicle->getCompanyName()] += distance * vehicle->getPassengerCount();

					//check if a fault has occurred every hour
					if ((std::abs(TimeElapsed - std::floor(TimeElapsed)) < EPSILON)
						&& TimeElapsed > 0) {											//by checking if the time elapsed is equal to itself
						if (vehicle->checkForFault()) {									//floored to the closest whole number, we make sure it checks
							totalFaults[vehicle->getCompanyName()] += 1;				//for fault every hour and not every time step
						}
						debugFaultCounter++;
					}
					vehicle->updateBatteryLevel(distance * vehicle->getEnergyUse());
					//cout << distance * vehicle->getEnergyUse() << " ";
				}else {	
					if (!vehicle->getIsCharging()) {
						cout << "vehicle is now in charging queue " << vehicle->getCompanyName() << " at time " << TimeElapsed << "\n";
						//add the current vehicle to the charging queue
						vehicle->setChargingStatus(1);
						chargeQueue.push(vehicle.get());
					}
				}
			}
			//charging phase
			for (auto& charger : chargers) {
				if (charger.isFree() && !chargeQueue.empty()) {
					Vehicle* EVTOL = chargeQueue.front();
					chargeQueue.pop();
					charger.startCharging(EVTOL);
				}
				else charger.update(timeStep, TimeElapsed);
			}

			TimeElapsed += timeStep;
			debugCounter += 1;
		}
		//debug
		for (auto& vehicle : vehicles) {
			cout << vehicle->getCompanyName() << " \n";
		}
		std::cout << "number of times the loop was executed: " << debugCounter << "\n";
		std::cout << "number of times we checked for faults: " << debugFaultCounter << "\n";
	}

};

int main()
{
	auto simulation = Simulation(20, 3);
	simulation.run();

	return 0;
}
/*	add to vehicle :	bool isIdle to count the flight per vehicle
						double timeIdle to count the total time spent in charge each time

	add to simulation a map to count the total idle time

	get the average dividing the total flight time by the number of flights
	get the average dividing the total charge time by the number of flights - 1

	create a function to log the relevant statistics

*/