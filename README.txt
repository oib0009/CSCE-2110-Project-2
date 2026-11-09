# CSCE 2110 Project 2: Smart Campus Navigation and Facility Analytics
REPO LINK: https://github.com/oib0009/CSCE-2110-Project-2

## Milestone 1

### Team Members
- Olu
- Ashish
- Uriel

## Project Goal

This C++ project loads campus data from the instructor-provided files and lets users explore facilities, buildings, and department information through a menu-based program.

## What Is Included

- Data loading and validation
- Custom facility hash table
- Undirected campus graph with BFS
- Department tree and menu integration

## Team Responsibilities
- Olu: Data loading and facility hash table
- Ash: Campus graph and BFS
- Uriel: Department tree, menu, and overall integration

## Main Menu

- View all facilities
- Search for a facility by ID
- Display the campus graph
- Display graph counts and building degrees
- Find reachable buildings using BFS
- Add a department
- Search for a department
- Display the department hierarchy
- Display preorder traversal
- Exit

## Compilation and How To Run
The program is written in C++. The program can be compiled using g++ with C++11.

To compile, open a terminal in the projects root directory! Then run: 
```bash
g++ -std=c++11 -Wall -Wextra -pedantic -I include src/*.cpp -o campus
```
To run the program using the provided data files, use: 
```bash
./campus
```
To run using different data:
```bash
./campus testdata/hash_collision
./campus testdata/empty_tree
./campus testdata/missing_file
```
The program loads the data files and shows any possible warnings before showing the menu.

## Directory Structure
- data/ - The provided data files for project 2
- include/ - Header files
- src/ - C++ implementation files and main.cpp
- testdata/ - Test files
- .gitignore
- CONTRIBUTIONS.txt - Team member contributions
- README.md - Project information and building/running instruction
- design_document.txt - Data structure design details
- test_results.txt - Milestone 1 testing results

## Important Requirements

- Keep original provided data files in data/. Store additional testing files in testdata/.
- Keep data loading, hash table, graph, and department tree in separate modules
- Handle missing or invalid data with warnings and summaries
- Validate menu input and avoid crashes
- Keep README.md, CONTRIBUTIONS.txt, design_document.txt, and test_results.txt updated
- Verify the project builds and runs in the UNT CSE CELL environment


