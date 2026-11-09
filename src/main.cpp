// Purpose: Coordinates dataset loading and the Smart Campus menu interface.
// Author: Uriel Trejo
#include "CampusGraph.h"
#include "DataLoader.h"
#include "DepartmentTree.h"
#include "FacilityHashTable.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

//Removes whitespace at beginning and end of string.
static string trimInput(const string& text) {
    size_t first = text.find_first_not_of(" \t\n\r");
    if (first == string::npos) {
        return "";
    }
    size_t last = text.find_last_not_of(" \t\n\r");
    return text.substr(first, last - first + 1);
}

//Displays menu with choices to choose from.
void displayMenu() {
    cout << "Smart Campus Menu:" << endl;
    cout << "1. View all facilities\n";
    cout << "2. Search facility by ID\n";
    cout << "3. View campus graph\n";
    cout << "4. Display graph counts and degrees\n";
    cout << "5. Find reachable buildings (BFS)\n";
    cout << "6. Add department\n";
    cout << "7. Search department\n";
    cout << "8. Display department hierarchy\n";
    cout << "9. Run preorder traversal\n";
    cout << "10. Exit\n";
    cout << "Choose an option: ";
}

//Reads user input for menu option and validates it. Returns true if valid, otherwise false.
bool readMenuOption(int& option) {
    string line;
    if (!getline(cin, line)) return false;
    stringstream input(line);
    char extra;
    if (!(input >> option) || (input >> extra) || option < 0 || option > 10) {
        cout << "Error: Invalid input. Enter a number between 0 and 10." << endl;
        option = -1;
    }
    return true;
}

//This prompts user for a name input, then trims whitespace and checks if it's empty. Returns true if valid, otherwise false.
bool promptName(const string& prompt, string& name) {
    cout << prompt;
    if (!getline(cin, name)) return false;
    name = trimInput(name);
    if (name.empty()) {
        cout << "Error: Name cannot be empty." << endl;
        return false;
    }
    return true;
}

void displayFacility(const Facility& facility) {
    cout << facility.id << " | " << facility.name << " | " << facility.building << " | " << facility.category << endl;
}

int main(int argc, char* argv[]) {
    string dataDirectory = "data";
    if (argc > 1) {
        dataDirectory = argv[1];
    }

    CampusData data = loadCampusData(dataDirectory);
    writeLoadReport(data, cout);

    CampusGraph graph;
    for (const BuildingRecord& building : data.buildings) {
        graph.addBuilding(building.name);
    }
    for (const ConnectionRecord& connection : data.connections) {
        graph.addConnection(connection.buildingA, connection.buildingB);
    }

    DepartmentTree departments;
    for (const DepartmentRecord& department : data.departments) {
        string error;
        string parentName = department.parent;
        if (parentName.empty()) {
            parentName = "None";
        }
        if (!departments.addDepartment(department.name, parentName, error)) {
            cout << "Error adding department: " << error << endl;
        }
    }

    int option;
    string name;
    while (true) {
        displayMenu();
        if (!readMenuOption(option)) break;

        if (option == 0 || option == 10) {
            cout << "Exiting program." << endl;
            break;
        }

        if (option == 1) {
            if (data.facilities.empty()) {
                cout << "No facilities are loaded." << endl;
            }else {
                for (const Facility& facility : data.facilities) {
                        displayFacility(facility);
                }
            }
            cout << "Total facilities: " << data.facilities.size() << endl;
        }
        else if (option == 2) {
            cout << "Enter facility ID: ";
            if (!getline(cin, name)) break;
            string id = FacilityHashTable::normalizeId(name);
            if (!FacilityHashTable :: isValidId(id)) {
                cout << "Invalid ID. Use F followed by exactly three digits." << endl;
            } else {
                const Facility* facility = data.facilityById.find(id);
                if (facility == nullptr) {
                    cout << "Facility not found: " << id << endl;
                } else {
                    displayFacility(*facility);
                }
            }
        }
        else if (option == 3) {
            graph.display();
        }
        else if (option == 4) {
            graph.displayStats();
        }
        else if (option == 5) {
            if (promptName("Enter starting building: ", name)) {
                graph.bfs(name);
            }
        }
        else if (option == 6) {
            if (promptName("Enter department name: ",name )) {
                string parentName = "None";
                
                if (!departments.isEmpty()) {
                    if (!promptName("Enter parent department: ", parentName)) {
                        continue;
                    }
                }
                string error;
                if (departments.addDepartment(name, parentName, error)) {
                    cout << "Department added successfully." << endl;
                } else {
                    cout << "Error: " << error << endl;
                }
            }
        }
        else if (option == 7) {
            if (promptName("Enter department name: ", name)) {
                departments.searchDepartment(name);
            }
        }
        else if (option == 8) {
            departments.displayHierarchy();
        }
        else if (option == 9) {
            departments.runPreorder();
        }
    }
    return 0;
}
