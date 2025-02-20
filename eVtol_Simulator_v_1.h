// eVtol_Simulator_v_1.h: file di inclusione per file di inclusione di sistema standard,
// o file di inclusione specifici del progetto.

#pragma once

#include <iostream>
#include <random>
#include <memory>
#include <vector>
#include <queue>
#include <map>
#include <chrono>
#include "eVtol_Factory.h"
#include "charger.h"

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

	//function that initializes the simulation
	Simulation(int numVehicles, int numChargers);

	void run();

};
