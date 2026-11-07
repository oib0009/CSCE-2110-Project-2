//purpose: Campus graph, degree reporting, and BFS reachability. 
//Author: Ashish Thapa.
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
    std::size_t edges_=0;
public:
    int findBuilding(const std::string& name) const; //returns -1 when name is not an accepted building.
    bool addBuilding(const std::string& name);
    bool addConnection(const std::string& from, const std::string& to); //rejects unknown endpoints, self-edges and undirected duplicates.
    std::string canonicalName(const std::string& name) const;
    void display() const;
    void displayStats() const;
    void bfs(const std::string& startName) const;
    std::size_t buildingCount() const {return buildings_.size();}
    std::size_t connectionCount() const {return edges_;}
};
#endif
