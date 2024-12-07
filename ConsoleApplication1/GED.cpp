#include "GED.hpp"
#include "common.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <set>

using namespace std;


int GraphSize(const Graph& graph) {
    int EdgeSum = 0;
    if (checkIfDirected(graph)) {
        for (int i = 0; i < graph.vertices; i++) {
            for (int j = i; j < graph.vertices; j++) {
                EdgeSum += graph.adjacencyMatrix.at(i).at(j);
            }
        }
    }
    else {
        for (int i = 0; i < graph.vertices; i++) {
            for (int j = 0; j < graph.vertices; j++) {
                EdgeSum += graph.adjacencyMatrix.at(i).at(j);
            }
        }
    }

    return EdgeSum;
}

struct CompareEditPath {
    bool operator()(const EditPath& p1, const EditPath& p2) const {
        return p1.totalCost() > p2.totalCost();
    }
};

// Cost functions
double nodeSubstitutionCost(int u1, int u2) {
    return (u1 == u2) ? 0 : 1; // Example: labels same = 0 cost, different = 1 cost
}

double edgeSubstitutionCost(int e1, int e2) {
    return abs(e1 - e2); // Example: difference in edge multiplicity
}

double insertionCost() {
    return 1.0; // Example: cost to insert a vertex or edge
}

double deletionCost() {
    return 1.0; // Example: cost to delete a vertex or edge
}

// Heuristic function lb(p)
double computeHeuristic(const Graph& g1, const Graph& g2,
    const unordered_map<int, int>& mapping,
    const set<int>& deletedNodes) {
    int unmappedG1 = g1.vertices - (mapping.size() + deletedNodes.size());
    int unmappedG2 = g2.vertices - mapping.size();

    // Assume costs are proportional to the number of remaining vertices/edges
    return unmappedG1 * insertionCost() + unmappedG2 * deletionCost();
}

// A*GED Algorithm
double astarGED(const Graph& g1, const Graph& g2) {
    priority_queue<EditPath, vector<EditPath>, CompareEditPath> openSet;
    double minCost = numeric_limits<double>::infinity();

    // Initialize the search with an empty mapping
    openSet.push({ {}, {}, 0.0, computeHeuristic(g1, g2, {}, {}) });

    while (!openSet.empty()) {
        EditPath current = openSet.top();
        openSet.pop();

        // If all nodes in g1 are mapped or deleted, calculate the cost
        if (current.mapping.size() + current.deletedNodes.size() == g1.vertices) {
            double finalCost = current.costSoFar;

            // Add insertion costs for unmapped nodes in g2
            for (int v2 = 0; v2 < g2.vertices; ++v2) {
                if (find_if(current.mapping.begin(), current.mapping.end(),
                    [v2](const auto& pair) { return pair.second == v2; }) == current.mapping.end()) {
                    finalCost += insertionCost();
                }
            }

            minCost = min(minCost, finalCost);
            continue;
        }

        // Expand the current path
        int nextNode = current.mapping.size() + current.deletedNodes.size(); // Next unmapped node in g1

        // Map nextNode to each vertex in g2
        for (int v2 = 0; v2 < g2.vertices; ++v2) {
            if (find_if(current.mapping.begin(), current.mapping.end(),
                [v2](const auto& pair) { return pair.second == v2; }) != current.mapping.end()) {
                continue; // Skip if v2 is already mapped
            }

            EditPath newPath = current;
            newPath.mapping[nextNode] = v2;

            // Compute costs
            double nodeCost = nodeSubstitutionCost(nextNode, v2);
            double edgeCost = 0.0;

            for (const auto& [u1, u2] : current.mapping) {
                if (u2 >= 0) { // Process valid mappings
                    edgeCost += edgeSubstitutionCost(
                        g1.adjacencyMatrix[nextNode][u1],
                        g2.adjacencyMatrix[v2][u2]
                    );
                }
            }

            newPath.costSoFar += nodeCost + edgeCost;
            newPath.heuristicCost = computeHeuristic(g1, g2, newPath.mapping, newPath.deletedNodes);

            openSet.push(newPath);
        }


        EditPath deletionPath = current;
        deletionPath.deletedNodes.insert(nextNode);
        deletionPath.costSoFar += deletionCost();
        deletionPath.heuristicCost = computeHeuristic(g1, g2, deletionPath.mapping, deletionPath.deletedNodes);
        openSet.push(deletionPath);
    }

    return minCost;
}


// Approximate GED Algorithm
double approximateGED(const Graph& g1, const Graph& g2) {
    unordered_map<int, int> nodeMapping; // Maps nodes from g1 to g2
    set<int> mappedG2Nodes; // Tracks already mapped nodes in g2

    // Step 1: Greedy node mapping
    for (int u1 = 0; u1 < g1.vertices; ++u1) {
        double minCost = numeric_limits<double>::infinity();
        int bestMatch = -1;

        for (int u2 = 0; u2 < g2.vertices; ++u2) {
            if (mappedG2Nodes.find(u2) != mappedG2Nodes.end()) continue; 

            double cost = nodeSubstitutionCost(u1, u2);
            if (cost < minCost) {
                minCost = cost;
                bestMatch = u2;
            }
        }

        if (bestMatch != -1) {
            nodeMapping[u1] = bestMatch;
            mappedG2Nodes.insert(bestMatch);
        }
        else {
            nodeMapping[u1] = -1; // Mark as deleted
        }
    }

    // Step 2: Compute node mapping cost
    double totalCost = 0.0;
    for (const auto& [u1, u2] : nodeMapping) {
        if (u2 == -1) {
            totalCost += deletionCost(); 
        }
        else {
            totalCost += nodeSubstitutionCost(u1, u2);
        }
    }

    // Step 3: Compute edge mapping cost
    for (int u1 = 0; u1 < g1.vertices; ++u1) {
        for (int v1 = 0; v1 < g1.vertices; ++v1) {
            int u2 = nodeMapping[u1];
            int v2 = (v1 < g1.vertices && nodeMapping.count(v1)) ? nodeMapping[v1] : -1;

            if (u2 == -1 || v2 == -1) {
                totalCost += deletionCost(); 
            }
            else {
                totalCost += edgeSubstitutionCost(
                    g1.adjacencyMatrix[u1][v1],
                    g2.adjacencyMatrix[u2][v2]
                );
            }
        }
    }

    // Add cost for unmapped nodes in g2 (insertions)
    for (int u2 = 0; u2 < g2.vertices; ++u2) {
        if (mappedG2Nodes.find(u2) == mappedG2Nodes.end()) {
            totalCost += insertionCost();
        }
    }

    return totalCost;
}

