#include "eVtol_Factory.h"

class eVtol_Factory {
public:
	static std::unique_ptr<Vehicle> createVehicle(const std::string& company) {
		if (company == "Alpha") {
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
};