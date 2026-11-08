// Purpose: Declares a general tree for department hierarchy operations.
// Author: Uriel Trejo
#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <string>
#include <vector>

using namespace std;

//This struct is used to represent a department node in the tree.
//Each node would contain the name, a pointer to its parent node, and also a vector of pointers to its children nodes.
//Uses a vector to allow for multiple children node as a general tree is needed.
struct DepartmentNode {
    string name;
    DepartmentNode* parent;
    vector<DepartmentNode*> children;
    DepartmentNode(const string& departmentName, DepartmentNode* parentNode = nullptr);
};

class DepartmentTree {
private:
    DepartmentNode* root; //Root node of the department tree.

    DepartmentNode* findDepartmentNode(DepartmentNode* node, const string& name) const; //Function to find a department node by name.
    void displayNode(DepartmentNode* node, int depth) const; //Function to display a department node and its children.
    void preorderNode(DepartmentNode* node, vector<string>& result) const; //Function to perform a preorder traversal of the department tree.
    void deleteNode(DepartmentNode* node); //Function to delete a department node and its children.

public:
    DepartmentTree();
    ~DepartmentTree();

    bool isEmpty() const; //Function to check if the department tree is empty.
    bool addDepartment(const string& name, const string& parentName, string& error); //Function to add a department to the tree.
    bool loadFromFile(const string& name); //Function to load the department tree from a file, for the data files.
    void searchDepartment(const string& name) const; //Function to search for a department in the tree.
    void displayHierarchy() const; //Function to display the department hierarchy.
    void runPreorder() const; //Function to run a preorder traversal of the department tree.
}

#endif
