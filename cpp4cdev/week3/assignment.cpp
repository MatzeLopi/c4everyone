// Homework of week 3
// Populate graph based on dessity

#include <iostream>
#include <random>

using namespace std;

double prob()
{

    return;
}

// Function to generate a graph
bool **create_graph(int size)
{
    bool **graph;
    graph = new bool *[size];
    for (int i = 0; i < size; i++)
    {
        graph[i] = new bool[size];
    }
    return graph;
}

int main()
{
    srand(time(0));
    return 0;
}