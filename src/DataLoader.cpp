// Purpose: Loads and validates the instructor-provided data files.
// Author: Olu
#include "DataLoader.h"

#include <cctype>
#include <fstream>
#include <ostream>
#include <sstream>

namespace {

std::string trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    const std::string::size_type first = value.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    return value.substr(first, value.find_last_not_of(whitespace) - first + 1);
}

std::string uppercase(const std::string& value) {
    std::string result = value;
    for (std::string::size_type i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char>(
            std::toupper(static_cast<unsigned char>(result[i])));
    }
    return result;
}

std::vector<std::string> splitFields(const std::string& line) {
    std::vector<std::string> fields;
    std::string::size_type start = 0;
    while (true) {
        const std::string::size_type separator = line.find('|', start);
        fields.push_back(trim(line.substr(
            start, separator == std::string::npos ? separator : separator - start)));
        if (separator == std::string::npos) {
            return fields;
        }
        start = separator + 1;
    }
}

std::string filePath(const std::string& directory, const std::string& fileName) {
    if (directory.empty() || directory[directory.size() - 1] == '/' ||
        directory[directory.size() - 1] == '\\') {
        return directory + fileName;
    }
    return directory + "/" + fileName;
}

void warn(CampusData& data, FileLoadSummary& summary,
          std::size_t lineNumber, const std::string& reason) {
    std::ostringstream message;
    message << summary.fileName << ':' << lineNumber << ": " << reason;
    data.warnings.push_back(message.str());
    ++summary.rejected;
}

template <typename HandleRecord>
void loadFile(CampusData& data, FileLoadSummary& summary,
              const std::string& directory, const std::string& fileName,
              std::size_t fieldCount, HandleRecord handleRecord) {
    summary.fileName = fileName;
    summary.accepted = 0;
    summary.rejected = 0;

    std::ifstream input(filePath(directory, fileName).c_str());
    if (!input) {
        data.warnings.push_back(fileName + ": unable to open file");
        return;
    }

    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        const std::string cleanedLine = trim(line);
        if (cleanedLine.empty() || cleanedLine[0] == '#') {
            continue;
        }

        const std::vector<std::string> fields = splitFields(line);
        if (fields.size() != fieldCount) {
            warn(data, summary, lineNumber, "incorrect number of fields");
        } else if (handleRecord(fields, lineNumber)) {
            ++summary.accepted;
        }
    }
}

std::string canonicalBuilding(const std::vector<BuildingRecord>& buildings,
                              const std::string& name) {
    const std::string key = uppercase(name);
    for (std::vector<BuildingRecord>::const_iterator building = buildings.begin();
         building != buildings.end(); ++building) {
        if (uppercase(building->name) == key) {
            return building->name;
        }
    }
    return "";
}

bool hasDepartment(const std::vector<DepartmentRecord>& departments,
                   const std::string& name) {
    const std::string key = uppercase(name);
    for (std::vector<DepartmentRecord>::const_iterator department =
             departments.begin(); department != departments.end(); ++department) {
        if (uppercase(department->name) == key) {
            return true;
        }
    }
    return false;
}

bool hasConnection(const std::vector<ConnectionRecord>& connections,
                   const std::string& buildingA,
                   const std::string& buildingB) {
    const std::string a = uppercase(buildingA);
    const std::string b = uppercase(buildingB);
    for (std::vector<ConnectionRecord>::const_iterator connection =
             connections.begin(); connection != connections.end(); ++connection) {
        const std::string existingA = uppercase(connection->buildingA);
        const std::string existingB = uppercase(connection->buildingB);
        if ((a == existingA && b == existingB) ||
            (a == existingB && b == existingA)) {
            return true;
        }
    }
    return false;
}

} // namespace

CampusData loadCampusData(const std::string& dataDirectory) {
    CampusData data;
    data.files.resize(4);

    loadFile(data, data.files[0], dataDirectory, "buildings.txt", 1,
        [&](const std::vector<std::string>& fields, std::size_t line) {
            if (fields[0].empty()) {
                warn(data, data.files[0], line, "building name is empty");
                return false;
            }
            if (!canonicalBuilding(data.buildings, fields[0]).empty()) {
                warn(data, data.files[0], line, "duplicate building name");
                return false;
            }
            BuildingRecord building = {fields[0]};
            data.buildings.push_back(building);
            return true;
        });

    loadFile(data, data.files[1], dataDirectory, "connections.txt", 2,
        [&](const std::vector<std::string>& fields, std::size_t line) {
            const std::string a = canonicalBuilding(data.buildings, fields[0]);
            const std::string b = canonicalBuilding(data.buildings, fields[1]);
            if (a.empty() || b.empty()) {
                warn(data, data.files[1], line, "unknown building");
                return false;
            }
            if (uppercase(a) == uppercase(b)) {
                warn(data, data.files[1], line, "self-connection");
                return false;
            }
            if (hasConnection(data.connections, a, b)) {
                warn(data, data.files[1], line, "duplicate connection");
                return false;
            }
            ConnectionRecord connection = {a, b};
            data.connections.push_back(connection);
            return true;
        });

    loadFile(data, data.files[2], dataDirectory, "facilities.txt", 4,
        [&](const std::vector<std::string>& fields, std::size_t line) {
            if (fields[1].empty() || fields[2].empty() || fields[3].empty()) {
                warn(data, data.files[2], line, "facility fields must not be empty");
                return false;
            }
            const std::string id = FacilityHashTable::normalizeId(fields[0]);
            if (!FacilityHashTable::isValidId(id)) {
                warn(data, data.files[2], line, "invalid facility ID");
                return false;
            }
            const std::string building = canonicalBuilding(data.buildings, fields[2]);
            if (building.empty()) {
                warn(data, data.files[2], line, "unknown building");
                return false;
            }

            Facility facility = {id, fields[1], building, fields[3]};
            if (!data.facilityById.insert(facility)) {
                warn(data, data.files[2], line, "duplicate facility ID");
                return false;
            }
            data.facilities.push_back(facility);
            return true;
        });

    loadFile(data, data.files[3], dataDirectory, "departments.txt", 2,
        [&](const std::vector<std::string>& fields, std::size_t line) {
            if (fields[0].empty() || fields[1].empty()) {
                warn(data, data.files[3], line, "department fields must not be empty");
                return false;
            }
            if (hasDepartment(data.departments, fields[0])) {
                warn(data, data.files[3], line, "duplicate department name");
                return false;
            }

            const std::string parent = uppercase(fields[1]);
            if (data.departments.empty()) {
                if (parent != "NONE") {
                    warn(data, data.files[3], line, "first department must use NONE");
                    return false;
                }
            } else if (parent == "NONE" || !hasDepartment(data.departments, fields[1])) {
                warn(data, data.files[3], line, "unknown department parent");
                return false;
            }

            DepartmentRecord department = {
                fields[0], parent == "NONE" ? "" : fields[1]
            };
            data.departments.push_back(department);
            return true;
        });

    return data;
}

void writeLoadReport(const CampusData& data, std::ostream& output) {
    for (std::vector<FileLoadSummary>::const_iterator file = data.files.begin();
         file != data.files.end(); ++file) {
        output << file->fileName << ": " << file->accepted << " accepted, "
               << file->rejected << " rejected";
        if (file->fileName == "facilities.txt") {
            output << " (capacity " << data.facilityById.bucketCount()
                   << ", entries " << data.facilityById.size()
                   << ", collisions " << data.facilityById.collisionCount() << ')';
        }
        output << '\n';
    }
    for (std::vector<std::string>::const_iterator warning = data.warnings.begin();
         warning != data.warnings.end(); ++warning) {
        output << "Warning: " << *warning << '\n';
    }
}
