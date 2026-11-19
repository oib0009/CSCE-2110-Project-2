
//purpose: Campus graph, degree reporting, BFS reachability,
//shortest path, and connectivity report.
//author: Ashish Thapa
#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <string>
#include <vector>
#include <cstddef>

class CampusGraph {
    struct Building {
        std::string name;
        std::vector<std::size_t> neighbors;
    };

    std::vector<Building> buildings_;
    std::size_t edges_ = 0;

public:
    //find a building by name; return -1 if not found.
    int findBuilding(const std::string& name) const;

    //add a new building.
    bool addBuilding(const std::string& name);

    //add an undirected connection between buildings.
    bool addConnection(const std::string& from,
                       const std::string& to);

    //return the accepted spelling of a building name.
    std::string canonicalName(const std::string& name) const;

    //display all buildings and their connections.
    void display() const;

    //display building counts and degrees.
    void displayStats() const;

    //find reachable buildings using BFS.
    void bfs(const std::string& startName) const;

    //fINAL:find the shortest path using BFS.
    void shortestPath(const std::string& startName,
                      const std::string& endName) const;

    //FINAL:display graph connectivity statistics.
    void connectivityReport() const;

    //return total buildings and connections.
    std::size_t buildingCount() const {
        return buildings_.size();
    }
    std::size_t connectionCount() const {
        return edges_;
    }
};

#endif
