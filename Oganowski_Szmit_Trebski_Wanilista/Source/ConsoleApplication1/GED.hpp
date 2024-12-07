#ifndef GED_HPP
#define GEDS_HPP

#include "common.hpp"
#include <unordered_map>
#include <set>

struct EditPath {
    unordered_map<int, int> mapping;   // Maps nodes from g1 to g2
    set<int> deletedNodes;            // Tracks deleted nodes from g1
    double costSoFar;                 // Cost accumulated so far
    double heuristicCost;             // Estimated remaining cost

    double totalCost() const {
        return costSoFar + heuristicCost;
    }
};

double nodeSubstitutionCost(int u1, int u2);
int GraphSize(const Graph& graph);
double edgeSubstitutionCost(int e1, int e2);
double insertionCost();
double deletionCost();
double computeHeuristic(const Graph& g1, const Graph& g2, const unordered_map<int, int>& mapping, const set<int>& deletedNodes);
double astarGED(const Graph& g1, const Graph& g2);
double approximateGED(const Graph& g1, const Graph& g2);

#endif //GED_HPP
