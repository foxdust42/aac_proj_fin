// HOW TO USE: 
// Add to ConsoleApplication1.cpp in the project source and adjust as needed and recompile.

#include "gen.h"
void generateRandomGraph(Graph& graph, int vertices, int density)
{
    graph.vertices = vertices;
    graph.adjacencyMatrix = vector<vector<int>>(vertices, vector<int>(vertices, 0));

    srand(time(0));

    for (int i = 0; i < vertices; ++i)
    {
        for (int j = i + 1; j < vertices; ++j)
        {
            if ((rand() % 100) < density)
            {
                graph.adjacencyMatrix[i][j] = 1;
                graph.adjacencyMatrix[j][i] = 1;
            }
        }
    }
}

void generateSparseGraph(Graph& graph, int vertices)
{
    graph.vertices = vertices;
    graph.adjacencyMatrix = vector<vector<int>>(vertices, vector<int>(vertices, 0));

    for (int i = 0; i < vertices - 1; ++i)
    {
        graph.adjacencyMatrix[i][i + 1] = 1;
        graph.adjacencyMatrix[i + 1][i] = 1;
    }
}

void generateDenseGraph(Graph& graph, int vertices)
{
    graph.vertices = vertices;
    graph.adjacencyMatrix = vector<vector<int>>(vertices, vector<int>(vertices, 1));

    for (int i = 0; i < vertices; ++i)
    {
        graph.adjacencyMatrix[i][i] = 0;
    }
}

void generateOneEdgeGraph(Graph& graph, int vertices)
{
    graph.vertices = vertices;
    graph.adjacencyMatrix = vector<vector<int>>(vertices, vector<int>(vertices, 0));
    graph.adjacencyMatrix[0][1] = 1;
}

void generateStarGraphDirected(Graph& graph, int vertices)
{
    graph.vertices = vertices;
    graph.adjacencyMatrix = vector<vector<int>>(vertices, vector<int>(vertices, 0));

    for (int i = 1; i < vertices; ++i)
    {
        graph.adjacencyMatrix[0][i] = 1;
    }
}

void generateStarGraphUnDirected(Graph& graph, int vertices)
{
    graph.vertices = vertices;
    graph.adjacencyMatrix = vector<vector<int>>(vertices, vector<int>(vertices, 0));

    for (int i = 1; i < vertices; ++i)
    {
        graph.adjacencyMatrix[0][i] = 1;
        graph.adjacencyMatrix[i][0] = 1;
    }
}


void runTests()
{
    ofstream results("test_results.txt");
    if (!results.is_open())
    {
        cerr << "Error: Could not open results file." << endl;
        return;
    }

    for (int n = 3; n <= 7; ++n)
    {
        results << "Testing for n = " << n << " vertices:\n";
        vector<Graph> testGraphs(6);

        // Generate test cases
        generateSparseGraph(testGraphs[0], n);
        generateDenseGraph(testGraphs[1], n);
        generateRandomGraph(testGraphs[2], n, 50); // 50% density
        generateOneEdgeGraph(testGraphs[3], n);
        generateStarGraphDirected(testGraphs[4], n);
        generateStarGraphUnDirected(testGraphs[5], n);

        string graphTypes[] = {"Sparse", "Dense", "Random", "One Edge", "Star Directed", "Star Undirected"};

        for (int i = 0; i < 6; ++i)
        {
            auto start = chrono::high_resolution_clock::now();
            int result = minimalExtension(testGraphs[i], checkIfDirected(testGraphs[i])).size();
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end - start;

            results << graphTypes[i] << " graph: Minimal extensions = " << result
                << ", Time taken = " << elapsed.count() << " seconds.\n";
        }

        results << "\n";
    }

    results.close();
}