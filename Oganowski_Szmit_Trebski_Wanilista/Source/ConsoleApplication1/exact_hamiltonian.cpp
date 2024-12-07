#include "exact_hamiltonian.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <chrono>

bool hasHamiltonianCycleUtil(const Graph& graph, int pos, vector<bool>& visited, int count, bool isDirected)
{
    int n = graph.vertices;
    if (count == n)
    {
        if (isDirected)
        {
            return graph.adjacencyMatrix[pos][0] > 0;
        }
        return (graph.adjacencyMatrix[pos][0] > 0 || graph.adjacencyMatrix[0][pos] > 0);
    }
    for (int v = 0; v < n; ++v)
    {
        if (graph.adjacencyMatrix[pos][v] && !visited[v])
        {
            visited[v] = true;
            if (hasHamiltonianCycleUtil(graph, v, visited, count + 1, isDirected))
            {
                return true;
            }
            visited[v] = false;
        }
    }
    return false;
}

bool hasHamiltonianCycle(const Graph& graph, bool isDirected)
{
    int n = graph.vertices;
    vector<bool> visited(n, false);
    visited[0] = true;
    return hasHamiltonianCycleUtil(graph, 0, visited, 1, isDirected);
}

vector<pair<int, int>> minimalExtension(Graph graph)
{
    int n = graph.vertices;
    bool isDirected = checkIfDirected(graph);
    vector<pair<int, int>> missingEdges;

    for (int i = 0; i < n; ++i)
    {
        int jStart = isDirected ? 0 : i + 1;
        for (int j = jStart; j < n; ++j)
        {
            if (i != j && graph.adjacencyMatrix[i][j] == 0)
            {
                missingEdges.push_back({ i, j });
            }
        }
    }

    if (hasHamiltonianCycle(graph, isDirected))
    {
        return {};
    }

    int m = missingEdges.size();
    for (int k = 1; k <= m; ++k)
    {
        vector<bool> bitmask(k, true);
        bitmask.resize(m, false);

        do
        {
            for (int i = 0; i < m; ++i)
            {
                if (bitmask[i])
                {
                    auto edge = missingEdges[i];
                    graph.adjacencyMatrix[edge.first][edge.second] = 1;
                    if (!isDirected)
                    {
                        graph.adjacencyMatrix[edge.second][edge.first] = 1;
                    }
                }
            }

            if (hasHamiltonianCycle(graph, isDirected))
            {
                vector<pair<int , int>> result;
                for(int j=0 ; j<m ; j++)
                {
                    if(bitmask[j])
                    {
                        result.push_back(missingEdges[j]);
                    }
                }
                return result;
            }

            for (int i = 0; i < m; ++i)
            {
                if (bitmask[i])
                {
                    auto edge = missingEdges[i];
                    graph.adjacencyMatrix[edge.first][edge.second] = 0;
                    if (!isDirected)
                    {
                        graph.adjacencyMatrix[edge.second][edge.first] = 0;
                    }
                }
            }
        } while (prev_permutation(bitmask.begin(), bitmask.end()));
    }
    return {};
}