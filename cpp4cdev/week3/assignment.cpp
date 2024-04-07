// Homework of week 3

#include <iostream>
#include <random>
#include <iomanip>
#include <algorithm>
#include <map>

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
    // Constructor for Graph
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

    // Destructor
    ~Graph()
    {
        graph.clear();
        vertices.clear();
        edgeMap.clear();
    }

    // Function to populate the graph based on density and range of weights
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

    // Function to check if two vertices are adjacent
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

    // Function to return a vector of neighbors
    vector<Vertex> neighbors(int x)
    {
        return graph[x];
    }

    // Add Edge
    void add_edge(int x, int y, bool weight) // x,y are IDs of vertices
    {
        // Add edge to graph
        graph[x].push_back(vertices[y]);
        graph[y].push_back(vertices[x]);

        // Add edge to map
        Edge edge(vertices[x], vertices[y], weight);
        edgeMap.insert({make_pair(x, y), edge});
    }

    // Delete Edge
    void delete_edge(int x, int y)
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
    }

    // Get Node Value
    double get_node_value(int x)
    {
        return vertices[x].data;
    }

    // Set Node Value
    void set_node_value(int x, double value)
    {
        vertices[x].data = value;
    }

    // Get Edge Weight
    double get_edge_value(Vertex x, Vertex y)
    {
        int x_i = x.get_id();
        int y_i = y.get_id();

        Edge edge = edgeMap.at(make_pair(x_i, y_i));
        return edge.get_weight();
    }

    // Function to print the graph
    void print_graph()
    {
        cout << "Graph:" << endl;
        for (int i = 0; i < size; i++)
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

class PriorityQueue
{
public:
    // Constructor
    PriorityQueue() : queue() {}

    // Function to check if the queue is empty
    bool empty()
    {
        return queue.empty();
    }
    // Function to return the size of the queue
    int size()
    {
        return queue.size();
    }

    // Function to return the top element of the queue
    Vertex top()
    {
        return queue[0];
    }

    // Function to change the priority of a vertex
    void change_priority(double new_priority, Vertex v)
    {
        for (int i = 0; i < queue.size(); i++)
        {
            if (queue[i].get_id() == v.get_id())
            {
                queue[i].data = new_priority;
                break;
            }
        }
        sort_queue();
    }

    // Function to pop the top element of the queue
    void pop()
    {
        queue.erase(queue.begin());
    }

    // Function to check if an element exists in queue
    bool contains(Vertex v)
    {
        for (int i = 0; i < queue.size(); i++)
        {
            if (queue[i].get_id() == v.get_id())
            {
                return true;
            }
        }
        return false;
    }
    // Insert new element to que
    void insert(Vertex v)
    {
        queue.push_back(v);
        sort_queue();
    }

    // Function to print the queue
    void print_queue()
    {
        for (int i = 0; i < queue.size(); i++)
        {
            cout << queue[i].get_id() << " ";
        }
        cout << endl;
    }
    // Destructor
    ~PriorityQueue()
    {
        queue.clear();
    }

private:
    vector<Vertex> queue;

    // Function to sort the queue
    void sort_queue()
    {
        sort(queue.begin(), queue.end(), [](Vertex a, Vertex b)
             { return a.data < b.data; });
    }
};

class ShortestPath
// Class to find the shortest path between two vertices inside a graph using the priority queue
{
public:
    // Constructor based on Graph, start and end vertices
    ShortestPath(Graph graph, Vertex start, Vertex end) : graph(graph), start(start), end(end)
    {
        // Run Dijkstra's algorithm using the priority queue
        PriorityQueue queue;
        queue.insert(start);
        for (int i = 0; i < graph.vertices.size(); i++)
        {
            if (graph.vertices[i].get_id() != start.get_id())
            {
                graph.vertices[i].data = numeric_limits<double>::infinity();
                queue.insert(graph.vertices[i]);
            }
        }

        while (!queue.empty())
        {
            Vertex u = queue.top();
            queue.pop();
            vector<Vertex> neighbors = graph.neighbors(u.get_id());
            for (int i = 0; i < neighbors.size(); i++)
            {
                Vertex v = neighbors[i];

                cout << "Edge between " << u.get_id() << " and " << v.get_id() << endl;
                double alt = u.data + graph.get_edge_value(u, v);
                if (alt < v.data)
                {
                    v.data = alt;
                    queue.change_priority(alt, v);
                }
            }
            queue.print_queue();
        }

        // Print the shortest path
        cout << "Shortest path from " << start.get_id() << " to " << end.get_id() << " is: " << endl;
        Vertex current = end;
        path.push_back(current);
        while (current.get_id() != start.get_id())
        {
            vector<Vertex> neighbors = graph.neighbors(current.get_id());
            for (int i = 0; i < neighbors.size(); i++)
            {
                Vertex v = neighbors[i];
                if (v.data + graph.get_edge_value(v, current) == current.data)
                {
                    path.push_back(v);
                    current = v;
                    break;
                }
            }
        }
    }

private:
    Vertex start;
    Vertex end;
    Graph graph;
    vector<Vertex> path;
};

int main()
{
    // Generate a graph with 50 vertices and a density of 0.1
    Graph g(50, 0.1);
    g.populate_graph(0.0, 10.0);
    // g.print_graph();

    ShortestPath sp(g, Vertex(0), Vertex(11));

    return 0;
}