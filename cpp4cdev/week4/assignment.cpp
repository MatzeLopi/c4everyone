// Homework week 4
// Based on week 3 homework, removed the priority queue and shortest path class for easier readability

#include <iostream>
#include <random>
#include <iomanip>
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

// Function to generate a random double
double getRandomDouble(double lowerLimit, double upperLimit)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(lowerLimit, upperLimit);
    return dis(gen);
}

/**
 * @class Vertex
 * @brief Represents a vertex in a graph.
 *
 * The Vertex class stores information about a vertex in a graph. It contains an ID and a data value.
 * The ID uniquely identifies the vertex, and the data value represents additional information associated with the vertex.
 */
class Vertex
{
public:
    /**
     * @brief Default constructor.
     * @param id The ID of the vertex.
     * @param data The data value associated with the vertex (default: 0.0).
     */
    Vertex(int id, double data = 0.0) : id(id), data(data) {}

    /**
     * @brief Overloads the << operator to allow printing of Vertex objects.
     * @param os The output stream.
     * @param v The Vertex object to be printed.
     * @return The output stream.
     */
    friend ostream &operator<<(ostream &os, const Vertex &v);

    /**
     * @brief Getter for the ID of the vertex.
     * @return The ID of the vertex.
     */
    int get_id() const
    {
        return id;
    }

    double data; // Additional data associated with the vertex.

private:
    int id; // The ID of the vertex.
};

// Print operator overload for Vertex
ostream &operator<<(ostream &os, const Vertex &v)
{
    os << v.get_id();
    return os;
}

/**
 * Represents an edge in a graph.
 */
class Edge
{
public:
    /**
     * Constructs an Edge object.
     *
     * @param start The start vertex of the edge.
     * @param end The end vertex of the edge.
     * @param weight The weight of the edge.
     */
    Edge(Vertex start, Vertex end, double weight) : start(start), end(end), weight(weight) {}

    friend ostream &operator<<(ostream &os, const Edge &e);

    /**
     * Gets the start vertex of the edge.
     *
     * @return The start vertex.
     */
    Vertex get_start()
    {
        return start;
    }

    /**
     * Gets the end vertex of the edge.
     *
     * @return The end vertex.
     */
    Vertex get_end()
    {
        return end;
    }

    /**
     * Gets the weight of the edge.
     *
     * @return The weight.
     */
    double get_weight()
    {
        return weight;
    }

private:
    Vertex start;  // The start vertex of the edge
    Vertex end;    // The end vertex of the edge
    double weight; // The weight of the edge
};

// Function to print the edge
ostream &operator<<(ostream &os, const Edge &e)
{
    os << e.start << " -> " << e.end << " : " << e.weight;
    return os;
}

/**
 * @class Graph
 * @brief Represents a graph data structure.
 *
 * The Graph class provides functionality to create and manipulate a graph.
 * It supports adding and deleting edges, checking adjacency between vertices,
 * getting neighbors of a vertex, setting and getting node values, and printing the graph.
 */
class Graph
{
public:
    /**
     * @brief Constructs a Graph object with the specified size and density.
     *
     * @param size The number of vertices in the graph.
     * @param density The density of the graph, which determines the probability of an edge between two vertices.
     */
    Graph(int size = 0, double density = 0.0) : size(size), density(density), vertices()
    {
        // Create vertices
        for (int i = 0; i < size; i++)
        {
            vertices.push_back(Vertex(i));
        }

        // Create a graph
        graph = vector<vector<Vertex>>(size);
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (getRandomDouble(0.0, 1.0) < density && i != j)
                {
                    graph[i].push_back(vertices[j]);
                }
            }
        }
    }

    /**
     * Constructs a Graph object from a file.
     * File starts with initial Integer N, followed by N lines of triplets of integers (i, j, w) where i and j are the vertices and w is the weight of the edge.
     *
     * @param filename The name of the file to read the graph from.
     */
    Graph(string filename) : vertices()
    {
        ifstream file(filename);

        // Get first line of file and create Vertices
        int size;

        file >> size;
        this->size = size;

        for (int i = 0; i < size; i++)
        {
            vertices.push_back(Vertex(i));
        }

        graph = vector<vector<Vertex>>(size);

        // Read the rest of the file
        int i, j, w;
        int edges = 0;
        while (file >> i >> j >> w)
        {

            cout << i << " " << j << " " << w << endl;
            add_edge(i, j, w);
            edges++;
        }
        this->density = (double)edges / (size * (size - 1));
    }

    /**
     * Destructor for the Graph class.
     * This function is responsible for cleaning up any dynamically allocated memory
     * and releasing any resources held by the Graph object.
     */
    ~Graph()
    {
        graph.clear();
        vertices.clear();
        edgeMap.clear();
    }

    // Function to populate the graph based on density and range of weights
    /**
     * Populates the graph with data within the specified range.
     *
     * @param lowerLimit The lower limit of the data range.
     * @param upperLimit The upper limit of the data range.
     */
    void populate_graph(double lowerLimit, double upperLimit)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++) // Corrected the condition to iterate over all vertices
            {
                if (i != j) // Added a condition to avoid self-loops
                {
                    // Create Edge with weight in range of lowerLimit and upperLimit
                    Edge edge(vertices[i], vertices[j], getRandomDouble(lowerLimit, upperLimit));
                    // Add Edge to map
                    edgeMap.insert({make_pair(i, j), edge});
                }
            }
        }
    }

    /**
     * Determines if two Vertices are adjacent.
     *
     * @param x ID of the first Vertex.
     * @param y ID of the second Vertex.
     * @return True if the Vertices are adjacent, false otherwise.
     */
    bool adjacent(int x, int y)
    {
        for (int i = 0; i < graph[x].size(); i++)
        {
            if (graph[x][i].get_id() == y)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Returns a vector of neighbors for a given vertex.
     *
     * @param x The vertex for which to find neighbors.
     * @return A vector of neighbors for the given vertex.
     */
    vector<Vertex> neighbors(int x)
    {
        return graph[x];
    }

    /**
     * Adds an edge between two vertices in the graph.
     *
     * @param x The ID of the first vertex.
     * @param y The ID of the second vertex.
     * @param weight The weight of the edge.
     */
    void add_edge(int x, int y, double weight)
    {
        // Add edge to graph
        if (!adjacent(x, y))
        {
            graph[x].push_back(vertices[y]);
            graph[y].push_back(vertices[x]);

            // Add edge to map
            Edge edge(vertices[x], vertices[y], weight);
            Edge edge1(vertices[y], vertices[x], weight);
            edgeMap.insert({make_pair(x, y), edge});
            edgeMap.insert({make_pair(y, x), edge1});
        }
    }

    /**
     * @brief Deletes an edge between two vertices.
     *
     * This function removes the edge between the vertices with the given values.
     *
     * @param x The value of the first vertex.
     * @param y The value of the second vertex.
     */
    void
    delete_edge(int x, int y)
    {
        // Remove connection from x
        for (int i = 0; i < graph[x].size(); i++)
        {
            if (graph[x][i].get_id() == y)
            {
                graph[x].erase(graph[x].begin() + i);
                break;
            }
        }
        // Remove connection from y
        for (int i = 0; i < graph[y].size(); i++)
        {
            if (graph[y][i].get_id() == x)
            {
                graph[y].erase(graph[y].begin() + i);
                break;
            }
        }

        // Remove edge from map
        edgeMap.erase(make_pair(x, y));
        edgeMap.erase(make_pair(y, x));
    }

    /**
     * Retrieves the value of a node based on the given parameter.
     *
     * @param x ID of the Vertex for which to retrieve the value.
     * @return The value of the node.
     */
    double get_node_value(int x)
    {
        return vertices[x].data;
    }

    /**
     * Sets the value of a node in the vertices array.
     *
     * @param x The index of the node in the vertices array.
     * @param value The new value to be assigned to the node.
     */
    void set_node_value(int x, double value)
    {
        vertices[x].data = value;
    }

    /**
     * Return the value of the edge between two vertices.
     *
     * @param x ID of the first vertex.
     * @param y ID of the second vertex.
     * @return The value of the edge between x and y.
     */
    double get_edge_value(Vertex x, Vertex y)
    {
        int x_i = x.get_id();
        int y_i = y.get_id();

        Edge edge = edgeMap.at(make_pair(x_i, y_i));
        return edge.get_weight();
    }

    /**
     * Function to calculate the density of the graph.
     */
    double get_density()
    {
        return this->density;
    }

    /**
     * Prints the graph.
     */
    void print_graph()
    {
        cout << "Graph:" << endl;
        for (int i = 0; i < vertices.size(); i++)
        {
            cout << i << " -> ";
            for (int j = 0; j < graph[i].size(); j++)
            {
                cout << graph[i][j] << " ";
            }
            cout << endl;
        }
    }

    vector<vector<Vertex>> graph;
    vector<Vertex> vertices;
    map<pair<int, int>, Edge> edgeMap;

private:
    int size;
    double density;
};

/**
 * Finds the minimum spanning tree of a graph using Prim's algorithm.
 *
 * @param g The graph to find the minimum spanning tree of.
 * @param start The starting vertex for the algorithm.
 * @return The total weight of the minimum spanning tree.
 */
double prim(Graph &g, int start = 0)
{

    vector<bool> visited(g.vertices.size(), false);
    vector<double> dist(g.vertices.size(), numeric_limits<double>::max());

    dist[start] = 0.0;

    double total = 0.0;

    for (int i = 0; i < g.vertices.size(); i++)
    {
        // Distance cant be negative
        int u = -1;

        // Find the vertex with the minimum distance
        for (int j = 0; j < g.vertices.size(); j++)
        {
            if (!visited[j] && (u == -1 || dist[j] < dist[u]))
            {
                u = j;
            }
        }

        // Set the vertex as visited
        visited[u] = true;
        total += dist[u];

        // Check all neighbors of u and select minimum distance
        for (Vertex v : g.neighbors(u))
        {
            int v_i = v.get_id();
            double w = g.get_edge_value(g.vertices[u], v);
            if (w < dist[v_i])
            {
                dist[v_i] = w;
            }
        }
    }

    return total;
}

// Main function
int main()
{
    Graph g("test_data.txt");
    g.print_graph();
    cout << g.get_density() << endl;
    cout << prim(g) << endl;
    return 0;
}