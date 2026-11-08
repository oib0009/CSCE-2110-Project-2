//purpose: Implements undirected adjacency lists and BFS. 
//Author: Ashish Thapa.
#include "CampusGraph.h"
#include "Utilities.h"
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

int CampusGraph::findBuilding(const std::string& name) const {
    std::string key=util::normalized(name);
    for (std::size_t i=0; i<buildings_.size(); ++i)
        if (util::normalized(buildings_[i].name)==key) return static_cast<int>(i);
    return -1;
}
//add a uniquely named vertex.
bool CampusGraph::addBuilding(const std::string& name) {
    std::string clean=util::trim(name);
    if (clean.empty() || findBuilding(clean)>=0) return false;
    buildings_.push_back({clean,{}});
    return true;
}
//insert both directions and append neighbors in accepted file order.
bool CampusGraph::addConnection(const std::string& from, const std::string& to) {
    int a=findBuilding(from), b=findBuilding(to);
    if (a<0 || b<0 || a==b) return false;
    const auto& neighbors=buildings_[a].neighbors;
    if (std::find(neighbors.begin(),neighbors.end(),static_cast<std::size_t>(b))!=neighbors.end()) return false;
    buildings_[a].neighbors.push_back(static_cast<std::size_t>(b));
    buildings_[b].neighbors.push_back(static_cast<std::size_t>(a));
    ++edges_;
    return true;
}
//return accepted spelling from buildings.txt.
std::string CampusGraph::canonicalName(const std::string& name) const {
    int i=findBuilding(name); return i<0 ? "" : buildings_[i].name;
}
//print all vertices, including isolated vertices.
void CampusGraph::display() const {
    if (buildings_.empty()) {std::cout << "No buildings are loaded.\n";return;}
    std::cout << "Campus graph:\n";
    for (const auto& b: buildings_) {
        std::cout << b.name << ": ";
        if (b.neighbors.empty()) std::cout << "(no connections)";
        else for (std::size_t i=0;i<b.neighbors.size();++i) {
            if (i) std::cout << ", ";
            std::cout << buildings_[b.neighbors[i]].name;
        }
        std::cout << '\n';
    }
}
//count each undirected connection once; degree equals distinct neighbor count.
void CampusGraph::displayStats() const {
    std::cout << "Total buildings: " << buildings_.size() << '\n';
    std::cout << "Total connections: " << edges_ << '\n';
    if (buildings_.empty()) {std::cout << "No buildings are loaded.\n";return;}
    for (const auto& b: buildings_) std::cout << b.name << " - degree " << b.neighbors.size() << '\n';
}
//queue-based BFS; mark visited on enqueue to retain shortest distances.
void CampusGraph::bfs(const std::string& startName) const {
    int start=findBuilding(startName);
    if (start<0) {std::cout << "Error: unknown building '" << util::trim(startName) << "'.\n";return;}
    std::vector<bool> visited(buildings_.size(),false);
    std::vector<int> distances(buildings_.size(),-1);
    std::queue<std::size_t> q;
    q.push(static_cast<std::size_t>(start)); visited[start]=true; distances[start]=0;
    std::size_t count=0;
    std::cout << "Reachable buildings from " << buildings_[start].name << ":\n";
    while (!q.empty()) {
        auto current=q.front();q.pop();++count;
        std::cout << buildings_[current].name << " (distance " << distances[current] << ")\n";
        for (std::size_t next:buildings_[current].neighbors) if (!visited[next]) {
            visited[next]=true;
            distances[next]=distances[current]+1;
            q.push(next);
        }
    }
    std::cout << "Reachable: " << count << " of " << buildings_.size() << " buildings\n";
}
