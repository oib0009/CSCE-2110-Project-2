//CampusGraph.cpp
//Author: Ashish Thapa
//Purpose: Undirected campus graph, BFS reachability,
//shortest paths, and graph connectivity reporting.

#include "CampusGraph.h"
#include "Utilities.h"
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

//find a building by its case-insensitive name.
int CampusGraph::findBuilding(const std::string& name) const {
    std::string key = util::normalized(name);

    for (std::size_t i = 0; i < buildings_.size(); ++i) {
        if (util::normalized(buildings_[i].name) == key) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

//add a building while preventing duplicate names.
bool CampusGraph::addBuilding(const std::string& name) {
    std::string clean = util::trim(name);

    if (clean.empty() || findBuilding(clean) >= 0) {
        return false;
    }

    buildings_.push_back({clean, {}});
    return true;
}

//add an undirected connection between two buildings.
bool CampusGraph::addConnection(
    const std::string& from,
    const std::string& to
) {
    int a = findBuilding(from);
    int b = findBuilding(to);

    if (a < 0 || b < 0 || a == b) {
        return false;
    }

    const auto& neighbors = buildings_[a].neighbors;

    if (std::find(neighbors.begin(), neighbors.end(),
                  static_cast<std::size_t>(b)) != neighbors.end()) {
        return false;
    }

    buildings_[a].neighbors.push_back(static_cast<std::size_t>(b));
    buildings_[b].neighbors.push_back(static_cast<std::size_t>(a));

    ++edges_;
    return true;
}

//return the original accepted spelling of a building.
std::string CampusGraph::canonicalName(
    const std::string& name
) const {
    int index = findBuilding(name);

    return index < 0 ? "" : buildings_[index].name;
}

//display all buildings and their neighbors.
void CampusGraph::display() const {
    if (buildings_.empty()) {
        std::cout << "No buildings are loaded.\n";
        return;
    }

    std::cout << "Campus graph:\n";

    for (const auto& building : buildings_) {
        std::cout << building.name << ": ";

        if (building.neighbors.empty()) {
            std::cout << "(no connections)";
        } else {
            for (std::size_t i = 0;
                 i < building.neighbors.size(); ++i) {

                if (i > 0) std::cout << ", ";

                std::cout <<
                    buildings_[building.neighbors[i]].name;
            }
        }
        std::cout << '\n';
    }
}

//display building counts and individual degrees.
void CampusGraph::displayStats() const {
    std::cout << "Total buildings: "
              << buildings_.size() << '\n';

    std::cout << "Total connections: "
              << edges_ << '\n';

    if (buildings_.empty()) {
        std::cout << "No buildings are loaded.\n";
        return;
    }

    for (const auto& building : buildings_) {
        std::cout << building.name
                  << " - degree "
                  << building.neighbors.size() << '\n';
    }
}

//milestone 1: BFS reachability and distances.
void CampusGraph::bfs(
    const std::string& startName
) const {
    int start = findBuilding(startName);

    if (start < 0) {
        std::cout << "Error: unknown building '"
                  << util::trim(startName) << "'.\n";
        return;
    }

    std::vector<bool> visited(buildings_.size(), false);
    std::vector<int> distances(buildings_.size(), -1);
    std::queue<std::size_t> q;

    q.push(static_cast<std::size_t>(start));
    visited[start] = true;
    distances[start] = 0;

    std::size_t count = 0;

    std::cout << "Reachable buildings from "
              << buildings_[start].name << ":\n";

    while (!q.empty()) {
        std::size_t current = q.front();
        q.pop();
        ++count;

        std::cout << buildings_[current].name
                  << " (distance "
                  << distances[current] << ")\n";

        for (std::size_t next : buildings_[current].neighbors) {
            if (!visited[next]) {
                visited[next] = true;
                distances[next] = distances[current] + 1;
                q.push(next);
            }
        }
    }

    std::cout << "Reachable: " << count
              << " of " << buildings_.size()
              << " buildings\n";
}

//Final:find shortest path using BFS and parent tracking.
void CampusGraph::shortestPath(
    const std::string& startName,
    const std::string& endName
) const {
    int start = findBuilding(startName);
    int end = findBuilding(endName);

    if (start < 0) {
        std::cout << "Error: unknown starting building '"
                  << util::trim(startName) << "'.\n";
        return;
    }

    if (end < 0) {
        std::cout << "Error: unknown destination building '"
                  << util::trim(endName) << "'.\n";
        return;
    }

    std::vector<bool> visited(buildings_.size(), false);
    std::vector<int> parent(buildings_.size(), -1);
    std::queue<std::size_t> q;

    q.push(static_cast<std::size_t>(start));
    visited[start] = true;

    while (!q.empty()) {
        std::size_t current = q.front();
        q.pop();

        if (current == static_cast<std::size_t>(end)) {
            break;
        }

        for (std::size_t next : buildings_[current].neighbors) {
            if (!visited[next]) {
                visited[next] = true;
                parent[next] = static_cast<int>(current);
                q.push(next);
            }
        }
    }

    if (!visited[end]) {
        std::cout << "No path exists between "
                  << buildings_[start].name << " and "
                  << buildings_[end].name << ".\n";
        return;
    }

    std::vector<std::size_t> route;

    for (int current = end; current != -1;
         current = parent[current]) {
        route.push_back(static_cast<std::size_t>(current));
    }

    std::reverse(route.begin(), route.end());

    std::cout << "Shortest path:\n";

    for (std::size_t vertex : route) {
        std::cout << buildings_[vertex].name << '\n';
    }

    std::cout << "Number of connections: "
              << route.size() - 1 << '\n';
}

//Final:report graph connectivity and degree statistics.
void CampusGraph::connectivityReport() const {
    std::cout << "Total buildings: "
              << buildings_.size() << '\n';

    std::cout << "Total connections: "
              << edges_ << '\n';

    if (buildings_.empty()) {
        std::cout << "No buildings are loaded.\n";
        std::cout << "Highest degree: None\n";
        std::cout << "Isolated buildings: None\n";
        std::cout << "Connected components: 0\n";
        return;
    }

    std::size_t maxDegree = 0;

    for (const auto& building : buildings_) {
        maxDegree = std::max(
            maxDegree,
            building.neighbors.size()
        );
    }

    std::cout << "Highest degree (" << maxDegree << "): ";

    bool first = true;

    for (const auto& building : buildings_) {
        if (building.neighbors.size() == maxDegree) {
            if (!first) std::cout << ", ";

            std::cout << building.name;
            first = false;
        }
    }

    std::cout << "\nIsolated buildings: ";
    first = true;

    for (const auto& building : buildings_) {
        if (building.neighbors.empty()) {
            if (!first) std::cout << ", ";

            std::cout << building.name;
            first = false;
        }
    }

    if (first) {
        std::cout << "None";
    }

    std::cout << '\n';

    std::vector<bool> visited(buildings_.size(), false);
    std::size_t components = 0;

    for (std::size_t i = 0; i < buildings_.size(); ++i) {
        if (!visited[i]) {
            ++components;

            std::queue<std::size_t> q;
            q.push(i);
            visited[i] = true;

            while (!q.empty()) {
                std::size_t current = q.front();
                q.pop();

                for (std::size_t next :
                     buildings_[current].neighbors) {

                    if (!visited[next]) {
                        visited[next] = true;
                        q.push(next);
                    }
                }
            }
        }
    }

    std::cout << "Connected components: "
              << components << '\n';
}
