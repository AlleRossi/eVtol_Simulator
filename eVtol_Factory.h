#pragma once
#include "eVtols_vehicles.h"

#include <memory>
#include <string>

class eVtol_Factory {
public:
	static std::unique_ptr<Vehicle> createVehicle(const std::string& company);
};
