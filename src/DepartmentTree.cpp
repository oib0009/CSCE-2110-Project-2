// Purpose: Implements general department tree operations and traversals declared in DepartmentTree.h
// Author: Uriel Trejo
#include "DepartmentTree.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//This removes the whitepsace at the beginning and end of a string. To make text from .txt files easier to work with.
static string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\n\r");
    if (first == string::npos) {
        return "";
    }
    size_t last = text.find_last_not_of(" \t\n\r");
    return text.substr(first, last - first + 1);
}

//THis compares two strings, names in this case, without caring about uppercase or lowercase letters.
static string lowerCase(string text) {
    for (char& letter : text) {
        letter = static_cast<char>(tolower(static_cast<unsigned char>(letter)));
    }
    return text;
}

DepartmentNode::DepartmentNode(const string& departmentName, DepartmentNode* parentNode) {
    name = departmentName;
    parent = parentNode;
}

//Starts empty tree with no root node.
DepartmentTree::DepartmentTree() {
    root = nullptr;
}

//Frees all the memory uesd by the tree by deleting all nodes.
DepartmentTree::~DepartmentTree() {
    deleteNode(root);
}

//Deletes a department node and all its children nodes recursively.
void DepartmentTree::deleteNode(DepartmentNode* node) {
    if (node == nullptr) return;
    for (DepartmentNode* child : node->children) {
        deleteNode(child);
    }
    delete node;
}

//Checks if the department tree is empty by checking if the root node is null. Gives true or false.
bool DepartmentTree::isEmpty() const {
    return root == nullptr;
}

//Searche for a department node by name in the tree starting from the given node. Then returns a pointer to the found node or nullptr if not found.
DepartmentNode* DepartmentTree::findNode(DepartmentNode* node, const string& name) const {
    if (node == nullptr) return nullptr;
    if (lowerCase(node->name) == lowerCase(trim(name))) {
        return node;
    }
    for (DepartmentNode* child : node->children) {
        DepartmentNode* found = findNode(child, name);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

//Adds a department to the tree, checks various conditions and returns true if successful or false with an error message if not.
//Comments for clarity.
bool DepartmentTree::addDepartment(const string& name, const string& parentName, string& error) {
    string cleanName = trim(name);
    string cleanParentName = trim(parentName);
    error = "";

    //Checks for empty department name and returns error if empty.
    if (cleanName.empty()) {
        error = "Department name cannot be empty.";
        return false;
    }
    //Checks for empty parent name and returns error if empty.
    if (findNode(root, cleanName) != nullptr) {
        error = "Duplicate department name: " + cleanName;
        return false;
    }
    //Checks if root node is null, if so it checks if the parent name is "NONE" and creates the root node. If not then returns error.
    if (root == nullptr) {
        if (lowerCase(cleanParentName) != "none") {
            error = "The first department must have 'none' as its parent.";
            return false;
        }
        root = new DepartmentNode(cleanName);
        return true;
    }
    //Checks if the parent name is empty or "none" and then returns error since a non-root department needs existing parent.
    if (cleanParentName.empty() || lowerCase(cleanParentName) == "none") {
        error = "A non-root department needs an existing parent.";
        return false;
    }
    //Finds parent node by name and returns error if not found.
    //If found, creates a new department node and adds it as a child of the parent node.
    DepartmentNode* parentNode = findNode(root, cleanParentName);
    if (parentNode == nullptr) {
        error = "Unknown parent department: " + cleanParentName;
        return false;
    }
    //Creates a new departmen node and adds it as a child of the parent node.
    DepartmentNode* newNode = new DepartmentNode(cleanName, parentNode);
    parentNode->children.push_back(newNode);
    return true;
}

//Loads department tree from file, checks for various conditions. Returns true if successful or false with error messages if not.
bool DepartmentTree::loadFromFile(const string& filename) {
    ifstream file(filename);
    int accepted = 0;
    int rejected = 0;

    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        cout << filename << ": 0 accepted, 0 rejected" << endl;
        return false;
    }

    string line;
    int lineNumber = 0;
    while (getline(file, line)) {
        lineNumber++;
        string cleanLine = trim(line);
        if (cleanLine.empty() || cleanLine[0] == '#') {
            continue; // Skip empty lines and comments
        }

        size_t seperator = line.find('|');
        if (seperator == string::npos || line.find('|', seperator + 1) != string::npos) {
            cout << "Warning: " << filename << ":" << lineNumber << " - Expected DepartmentName | ParentName" << endl;
            rejected++;
            continue;
        }

        string name = trim(line.substr(0, seperator));
        string parent = trim(line.substr(seperator + 1));
        if (name.empty() || parent.empty()) {
            cout << "Warning: " << filename << ":" << lineNumber << " - Empty required field" << endl;
            rejected++;
            continue;
        }

        string error;
        if (addDepartment(name, parent, error)) {
            accepted++;
        } else {
            cout << "Warning: " << filename << ":" << lineNumber << " - " << error << endl;
            rejected++;
        }
    }
    cout << filename << ": " << accepted << "accepted, " << rejected << " rejected" << endl;
    return true;
}

//This displays the department hierarchy starting from the root node, and then recursively displays each child node
void DepartmentTree::searchDepartment(const string& name) const {
    DepartmentNode* found = findNode(root, name);
    if (found == nullptr) {
        cout << "Department not found: " << trim(name) << endl;
        return;
    }

    int depth = 0;
    DepartmentNode* current = found;
    while (current->parent != nullptr) {
        depth++;
        current = current->parent;
    }
    cout << "Department: " << found->name << endl;
    cout << "Parent: " << (found->parent == nullptr ? "None" : found->parent->name) << endl;
    cout << "Direct children: " << found->children.size() << endl;
    cout << "Depth: " << depth << endl;
}

//Displays a department node and its children recursively along with indentation based on depth of node in tree.
void DepartmentTree::displayNode(DepartmentNode* node, int depth) const {
    if (node == nullptr) return;
    cout << string(depth * 4, ' ') << node->name << endl;
    for (DepartmentNode* child : node->children) {
        displayNode(child, depth + 1);
    }
}

//Displays department heirarchy starting from root node and then recursively displays each child node. Includes indentation for depth.
void DepartmentTree::displayHierarchy() const {
    if (root == nullptr) {
        cout << "No departments are loaded." << endl;
        return;
    }
    displayNode(root, 0);
}

//Performs preorder traversal of the department tree starting from the given node and stores the names in the result vector.
void DepartmentTree::preorderNode(DepartmentNode* node, vector<string>& result) const {
    if (node == nullptr) return;
    result.push_back(node->name);
    for (DepartmentNode* child : node->children) {
        preorderNode(child, result);
    }
}

//Runs preorder traversal of the department tree and then displays results in a single line with arrows between department names.
void DepartmentTree::runPreorder() const {
    if (root == nullptr) {
        cout << "No departments are loaded." << endl;
        return;
    }
    vector<string> names;
    preorderNode(root, names);
    for (size_t i = 0; i < names.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << names[i];
    }
    cout<<endl;
}