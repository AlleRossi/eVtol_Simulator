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
	std::map<std::string, double> totalFlightTime;				//total flight time per company
	std::map<std::string, double> totalDistanceTraveled;		//total distance flown per company
	std::map<std::string, double> totalPassengerMiles;			//total passanger miles flown per company ( flown_miles * number_of_passangers )
	std::map<std::string, double> totalIdleTimePerCompany;		//total time an aircraft wasn't flying per company
	std::map<std::string, int> totalFaults;						//total faults recorded per company
	std::map<std::string, int> numberOfFlightsPerCompany;		//total flights per company ( recorded with the purpose of finding averages)
	std::map<std::string, int> numberOfChargeCyclesPerCompany;	//total charging cycles per company ( recorded with the purpose of finding charging averages)
	

public:

	Simulation(int numVehicles, int numChargers, double ClockSpeed, double SimulationTime) {		//function that initializes the simulation:		numVehicles is the number of vehicles to take part in the simulation
																																				//	numChargers is the number of chargers available in the simulation
																																				//	ClockSpeed is how fast time passes in the simulation, example: 0.01 equals to 36 seconds
																																				//	SimulationTime is how long will the simulation be, example: 1.0 equals to 1 hour
		srand(time(0));																				//set the seed for the random number generation

		initializeVehicles(numVehicles);

		initializeChargers(numChargers);
		simulationTime = SimulationTime;
		timeStep = ClockSpeed;
	}

	//create #numVehicles EVTOLS at random 
	void initializeVehicles(int numVehicles) {
		for (int i = 0; i < numVehicles; i++) {
			std::string company = companyNames[rand() % companyNames.size()];		//a company name is selected at random from the list of possible companies
			vehicles.push_back(eVtol_Factory::createVehicle(company));				//add to vehicles an instance of the selected company's EVTOL
		}
	}

	//create #chargers
	void initializeChargers(int numChargers) { 
		for (int i = 0; i < numChargers; i++) {
			chargers.emplace_back();					//creates an instance of a charger OBJ and adds it to chargers
		}
	}

	//flight phase logical handler
	void flightPhase(double TimeElapsed) {
		for (auto& vehicle : vehicles) {
			if (vehicle->getBatteryLevel() > 0) {
				//debug
				//cout << vehicle->getBatteryLevel() << " ";
				handleActiveFlight(vehicle, TimeElapsed);
			}
			else {
				queueForCharging(vehicle, TimeElapsed);
			}
		}
	}

	//handles all active flight operations
	void handleActiveFlight(std::unique_ptr<Vehicle>& vehicle, double TimeElapsed) {
		if (vehicle->getIsIdle()) {							//check if the aircraft was already in flight or not, if not,last charge cycle duration is recorded
			vehicle->setIdleStatus(false);
			double TimeDelta = TimeElapsed - vehicle->getStartIdleTime();		//when we get in this if statement at the very beginning of the simulation we end up with a negative TimeDelta, which we don't want to include in our data
			if (TimeDelta > 0) {
				totalIdleTimePerCompany[vehicle->getCompanyName()] += (TimeElapsed - vehicle->getStartIdleTime());	//last charge cycle duration is recorded
			}
			vehicle->resetStartIdleTime();
			numberOfFlightsPerCompany[vehicle->getCompanyName()]++;				//since it's the start of a new flight, the number of flights is increased
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
		}
		vehicle->updateBatteryLevel(distance * vehicle->getEnergyUse());
		//debug
		//cout << distance * vehicle->getEnergyUse() << " ";
	}

	//handles the operations regarding the charging queue management
	void queueForCharging(std::unique_ptr<Vehicle>& vehicle, double TimeElapsed) {
		if (!vehicle->getIsCharging()) {
			//add the current vehicle to the charging queue
			//cout << "vehicle is now in charging queue " << vehicle->getCompanyName() << " at time " << TimeElapsed << "\n";
			numberOfChargeCyclesPerCompany[vehicle->getCompanyName()]++;
			vehicle->setIdleStatus(1);					//as the vehicle isn't flying is now considered idle
			vehicle->updateStartIdleTime(TimeElapsed);	//update the time the vehicle starts the charging cycle
			vehicle->setChargingStatus(1);				//as the vehicle joins the charging queue it's status changes to Charging
			chargeQueue.push(vehicle.get());
		}
	}

	//charging phase handler
	void chargingPhase(double TimeElapsed) {
		for (auto& charger : chargers) {
			if (charger.isFree() && !chargeQueue.empty()) {		//for each charger it's checked if it's free, if so any vehicle in charging queue is assigned to it
				Vehicle* EVTOL = chargeQueue.front();			//retrieve the first vehicle in queue (FIFO style queue)
				chargeQueue.pop();								//the retrieved vehicle is eliminated from queue
				charger.startCharging(EVTOL);					//vehicle status is updated
			}
			else charger.update(timeStep, TimeElapsed);			//any charger that is already busy is updated
		}
	}

	//finalize simulation: ensures all charging vehicles's times are logged
	void finalizeSimulation(double TimeElapsed) {
		//check if there was any charging  vehicle at the time the simulation ends, if so, update the total idle times
		for (auto& charger : chargers) {
			if (!charger.isFree()) {
				totalIdleTimePerCompany[charger.getCurrentVehicle()->getCompanyName()] += (TimeElapsed - charger.getCurrentVehicle()->getStartIdleTime());
			}
		}

		//check if there was any vehicle in queue waiting to be charged at the time the simulation ends, if so, update the total idle times
		while (!chargeQueue.empty()) {
			Vehicle* item = chargeQueue.front();
			chargeQueue.pop();
			totalIdleTimePerCompany[item->getCompanyName()] += (TimeElapsed - item->getStartIdleTime());
		}
	}

	void run() {
		double TimeElapsed = timeStep;					//Time elapsed is the main clock variable of the function, it keeps track of the passing of time

		while (TimeElapsed <= simulationTime) {			//Emulates passing of time
			//debug
			//std::cout << "time elapsed value: " << TimeElapsed  << "  floored value of time elapsed" << static_cast<int>(TimeElapsed) << "\n";
			// 
			//flight phase	of the simulation cycle
			flightPhase(TimeElapsed);
			//charging phase of the simulation cycle
			chargingPhase(TimeElapsed);

			TimeElapsed += timeStep;								//Simulation timer is increased
		}
		//debug
		/*for (auto& vehicle : vehicles) {
			cout << vehicle->getCompanyName() << " \n";
		}*/

		//ensures all events are logged correctly 
		finalizeSimulation(TimeElapsed);

	}

	//function to log all of the simulation statistics
	void recordSimulationInfo() {
		std::cout << std::setw(10) << "Company"
			<< std::setw(15) << "Avg Flight"
			<< std::setw(15) << "Avg Distance"
			<< std::setw(15) << "Avg Charge"
			<< std::setw(10) << "Faults"
			<< std::setw(20) << "Passenger Miles"
			<< std::setw(15) << "# Vehicles"
			<< std::setw(10) << "# Flights"
			<< std::endl;

		std::cout << std::string(115, '-') << std::endl;
		std::map<std::string, int> NumberOfVehicles;
		for (auto& vehicle : vehicles) {
			NumberOfVehicles[vehicle->getCompanyName()]++;
		}

		//iterate through one map (assuming all maps have the same keys)
		for (const auto& [company, flightTime] : totalFlightTime) {
			std::cout << std::setw(10) << company
				<< std::setw(15) << (flightTime / numberOfFlightsPerCompany[company])
				<< std::setw(15) << (totalDistanceTraveled[company] / numberOfFlightsPerCompany[company])
				<< std::setw(15) << (totalIdleTimePerCompany[company] / numberOfChargeCyclesPerCompany[company])
				<< std::setw(10) << totalFaults[company]
				<< std::setw(20) << totalPassengerMiles[company]
				<< std::setw(15) << NumberOfVehicles[company]
				<< std::setw(10) << numberOfFlightsPerCompany[company]
				<< std::endl;
		}
	}


};

int main()
{
	auto simulation = Simulation(20, 3, 0.01, 3);
	simulation.run();
	simulation.recordSimulationInfo();

	return 0;
}
