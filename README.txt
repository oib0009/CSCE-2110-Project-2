CSCE 2110 Project 2 - Smart Campus Navigation and Facility Analytics
Milestone 1 README

Team members:
- Olu
- Ash
- Uriel

Project goal
This C++ project loads campus data from the instructor-provided files and lets users explore facilities, buildings, and department information through a menu-based program.

What is included
- Data loading and validation
- Custom facility hash table
- Undirected campus graph with BFS
- Department tree and menu integration

Team responsibilities
- Olu: Data loading and facility hash table
- Ash: Campus graph and BFS
- Uriel: Department tree, menu, and overall integration

Main menu
1. View all facilities
2. Search for a facility by ID
3. Display the campus graph
4. Display graph counts and building degrees
5. Find reachable buildings using BFS
6. Add a department
7. Search for a department
8. Display the department hierarchy
9. Display preorder traversal
10. Exit

Important requirements
- Use the instructor-provided data files only
- Keep data loading, hash table, graph, and department tree in separate modules
- Handle missing or invalid data with warnings and summaries
- Validate menu input and avoid crashes
- Keep README.txt, CONTRIBUTIONS.txt, design_document.txt, and test_results.txt updated
- Verify the project builds and runs in the UNT CSE CELL environment

Acceptance checklist
- All four input files load in the required order
- Missing or malformed data is handled correctly
- Facility lookup works regardless of letter case
- Hash table collisions are detected and shown
- Graph displays connections, counts, and building degrees
- BFS finds reachable buildings from a selected start point
- Department operations work correctly
- Menu options 1-10 function correctly
- Project compiles and runs successfully
