#ifndef EXACT_HAMILTONIAN_HPP
#define EXACT_HAMILTONIAN_HPP

#include "common.hpp"


bool hasHamiltonianCycleUtil(const Graph& graph, int pos, vector<bool>& visited, int count, bool isDirected);

bool hasHamiltonianCycle(const Graph& graph, bool isDirected);

int minimalExtension(Graph graph);

#endif //EXACT_HAMILTONIAN_HPP