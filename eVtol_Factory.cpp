#include "eVtol_Factory.h"
//Factory class that has only one method to create instances of EVTOLs 
std::unique_ptr<Vehicle> eVtol_Factory::createVehicle(const std::string& company) {		//takes a single company name to create corresponding EVTOL obj, in case of wrong company name, you get undefined behavior
	if (company == "Alpha") {								//used if-else structure because strings are not directly supported in Switch-case structures in C++
		return std::make_unique<AlphaEVTOL>();
	}
	else if (company == "Bravo") {
		return std::make_unique<BravoEVTOL>();
	}
	else if (company == "Charlie") {
		return std::make_unique<CharlieEVTOL>();
	}
	else if (company == "Delta") {
		return std::make_unique<DeltaEVTOL>();
	}
	else if (company == "Echo") {
		return std::make_unique<EchoEVTOL>();
	}
	return nullptr;
}
