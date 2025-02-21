// eVtol_Simulator_v_1.cpp: defines starting point of application.
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

	double simulationTime;								//3 hours
	double timeStep;									//time step for each iteration of the simulation loop

	//statistics tracking for each EVTOL manufacturer 
	std::map<std::string, double> totalFlightTime;
	std::map<std::string, double> totalDistanceTraveled;
	std::map<std::string, int> totalFaults;
	std::map<std::string, double> totalPassengerMiles;
	std::map<std::string, double> totalIdleTimePerCompany;

public:

	Simulation(int numVehicles, int numChargers, double ClockSpeed, double SimulationTime) {		//function that initializes the simulation:		numVehicles is the number of vehicles to take part in the simulation
																																				//	numChargers is the number of chargers available in the simulation
																																				//	ClockSpeed is how fast time passes in the simulation, example: 0.01 equals to 36 seconds
																																				//	SimulationTime is how long will the simulation be, example: 1.0 equals to 1 hour
		srand(time(0));																				//set the seed for the random number generation

		//create #numVehicles EVTOLS at random 
		for (int i = 0; i < numVehicles; i++) {
			std::string company = companyNames[rand() % companyNames.size()];		//a company name is selected at random from the list of possible companies
			vehicles.push_back(eVtol_Factory::createVehicle(company));				//add to vehicles an instance of the selected company's EVTOL
		}

		//create #chargers 
		for (int i = 0; i < numChargers; i++) {
			chargers.emplace_back();					//creates an instance of a charger OBJ and adds it to chargers
		}
		simulationTime = SimulationTime;
		timeStep = ClockSpeed;
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
					if (vehicle->getIsIdle()) {
						vehicle->setIdleStatus(false);
						totalIdleTimePerCompany[vehicle->getCompanyName()] += (TimeElapsed - vehicle->getStartIdleTime());
						vehicle->resetStartIdleTime();
					}
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
						//add the current vehicle to the charging queue
						cout << "vehicle is now in charging queue " << vehicle->getCompanyName() << " at time " << TimeElapsed << "\n";
						vehicle->setIdleStatus(1);					//as the vehicle isn't flying is now considered idle
						vehicle->updateStartIdleTime(TimeElapsed);	//update the time the vehicle starts the charging cycle
						vehicle->setChargingStatus(1);				//as the vehicle joins the charging queue it's status changes to Charging
						chargeQueue.push(vehicle.get());
					}
				}
			}
			//charging phase
			for (auto& charger : chargers) {						
				if (charger.isFree() && !chargeQueue.empty()) {		//for each charger it's checked if it's free, if so any vehicle in charging queue is assigned to it
					Vehicle* EVTOL = chargeQueue.front();			//retrieve the first vehicle in queue (FIFO style queue)
					chargeQueue.pop();								//the retrieved vehicle is eliminated from queue
					charger.startCharging(EVTOL);					//vehicle status is updated
				}
				else charger.update(timeStep, TimeElapsed);			//any charger that is already busy is updated
			}

			TimeElapsed += timeStep;								//Simulation timer is increased
			debugCounter += 1;
		}
		//debug
		for (auto& vehicle : vehicles) {
			cout << vehicle->getCompanyName() << " \n";
		}
		std::cout << "number of times the loop was executed: " << debugCounter << "\n";
		std::cout << "number of times we checked for faults: " << debugFaultCounter << "\n";

		//check if there was any charging  vehicle at the time the simulation ends, if so, update the total idle times
		for (auto& charger : chargers) {
			if (!charger.isFree()) {
				totalIdleTimePerCompany[charger.getCurrentVehicle()->getCompanyName()] += (TimeElapsed - charger.getCurrentVehicle()->getStartIdleTime());
			}
		}

		//check if there was any vehicle in queue waiting to be charged at the time the simulation ends, if so, update the total idle times
		while(!chargeQueue.empty()) {
			Vehicle* item = chargeQueue.front();
			chargeQueue.pop();
			totalIdleTimePerCompany[item->getCompanyName()] += (TimeElapsed - item->getStartIdleTime());
		}
	}

};

int main()
{
	auto simulation = Simulation(20, 3, 0.01, 3);
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