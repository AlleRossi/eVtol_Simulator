// eVtol_Simulator_v_1.cpp: definisce il punto di ingresso dell'applicazione.
//

#include "eVtol_Simulator_v_1.h"
#include <random>
#include <memory>
#include <vector>
#include <iostream>
#include <queue>
#include <map>
#include <chrono>

using namespace std;

class Simulation {
private:

	std::vector<std::unique_ptr<Vehicle>> vehicles;		//vector of smart pointers that point to Vehicle OBJs
	std::vector<Charger> chargers;						//vector of Chargers
	std::queue<Vehicle*> chargeQueue;					//queue for the vehicles that need charging
	std::vector<std::string> companyNames;				//names of the five EVTOL manufacturers of the simulation

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
			std::string company = companyNames[rand() % (0 - companyNames.size())];	//a company name is selected at random from the list of possible companies
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

		while (TimeElapsed < simulationTime) {



			TimeElapsed += timeStep;
			debugCounter += 1;
		}

		std::cout << "number of times the loop was executed: " << debugCounter << "\n";
	}

};

int main()
{
	cout << "Hello CMake." << endl;
	return 0;
}
