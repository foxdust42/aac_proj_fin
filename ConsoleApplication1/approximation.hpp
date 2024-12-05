#pragma once
#include <vector>
#include "common.hpp"

struct WeightedGraph {
    int vertices;
    std::vector<std::vector<int>> weightMatrix;
};

WeightedGraph transform_to_complete_weighted_graph(
    const std::vector<std::vector<int>>& adjacencyMatrix);
void print_weighted_graph(const WeightedGraph& graph);
int hamiltonian_completion_approximation(const std::vector<std::vector<int>>& graph);
bool three_opt_move(std::vector<int>& cycle, const std::vector<std::vector<int>>& weights);
