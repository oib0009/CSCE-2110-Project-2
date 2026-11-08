// Purpose: Defines the facility record used by the Smart Campus system.
// Author: Olu
#ifndef FACILITY_H
#define FACILITY_H

#include <string>

struct Facility {
    std::string id;
    std::string name;
    std::string building;
    std::string category;
};

#endif // FACILITY_H
