// Purpose: Loads and validates the instructor-provided data files.
// Author: Olu
#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "Facility.h"
#include "FacilityHashTable.h"

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

struct BuildingRecord {
    std::string name;
};

struct ConnectionRecord {
    std::string buildingA;
    std::string buildingB;
};

struct DepartmentRecord {
    std::string name;
    std::string parent;
};

struct FileLoadSummary {
    std::string fileName;
    std::size_t accepted;
    std::size_t rejected;
};

struct CampusData {
    std::vector<BuildingRecord> buildings;
    std::vector<ConnectionRecord> connections;
    std::vector<Facility> facilities;
    std::vector<DepartmentRecord> departments;
    FacilityHashTable facilityById;
    std::vector<FileLoadSummary> files;
    std::vector<std::string> warnings;
};

// Loads buildings, connections, facilities, then departments.
CampusData loadCampusData(const std::string& dataDirectory = "data");
void writeLoadReport(const CampusData& data, std::ostream& output);

#endif // DATA_LOADER_H
