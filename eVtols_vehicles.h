#pragma once
#include "Vehicle.h"

//creation of all the EVTOL classes based on the Vehicle class, each with their unique characteristics
class AlphaEVTOL : public Vehicle {                                         
public:
    AlphaEVTOL() : Vehicle("Alpha", 120, 320, 0.6, 1.6, 4, 0.25) {}
};

class BravoEVTOL : public Vehicle {
public:
    BravoEVTOL() : Vehicle("Bravo", 100, 100, 0.2, 1.5, 5, 0.10) {}
};

class CharlieEVTOL : public Vehicle {
public:
    CharlieEVTOL() : Vehicle("Charlie", 160, 220, 0.8, 2.2, 3, 0.05) {}
};

class DeltaEVTOL : public Vehicle {
public:
    DeltaEVTOL() : Vehicle("Delta", 90, 120, 0.62, 0.8, 2, 0.22) {}
};

class EchoEVTOL : public Vehicle {
public:
    EchoEVTOL() : Vehicle("Echo", 30, 150, 0.3, 5.8, 2, 0.61) {}
};

