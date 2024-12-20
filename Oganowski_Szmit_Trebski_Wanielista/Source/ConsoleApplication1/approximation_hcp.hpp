#pragma once
#include <vector>

#include "common.hpp"

struct WeightedGraph {
  int vertices;
  std::vector<std::vector<int>> weightMatrix;
};

struct edge {
  int start;
  int end;
};

struct cycle_segment {
  int start;
  int end;
  bool reverse = false;
};

WeightedGraph transform_to_complete_weighted_graph(
    const std::vector<std::vector<int>>& adjacencyMatrix);
void print_weighted_graph(const WeightedGraph& graph);
std::vector<std::pair<int, int>> hamiltonian_completion_approximation(
    const std::vector<std::vector<int>>& graph);