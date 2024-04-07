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
    os << "Vertex: " << v.get_id() << " Data: " << v.data;
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
    Vertex start; // The start vertex of the edge
    Vertex end; // The end vertex of the edge
    double weight; // The weight of the edge
};

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
            for (int j = 0; graph[i].size() < size; j++)
            {
                // Create Edge with weight in range of lowerLimit and upperLimit
                Edge edge(vertices[i], vertices[j], getRandomDouble(lowerLimit, upperLimit)); 
                // Add Edge to map
                edgeMap[make_pair(i, j)] = edge;
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
    void add_edge(int x, int y, bool weight)
    {
        // Add edge to graph
        graph[x].push_back(vertices[y]);

        // Add edge to map
        Edge edge(vertices[x], vertices[y], weight);
        edgeMap[make_pair(x, y)] = edge;
    }

    // Delete Edge
    void delete_edge(int x, int y)
    {
        // Remove edge from graph
        for (int i = 0; i < graph[x].size(); i++)
        {
            if (graph[x][i].get_id() == y)
            {
                graph[x].erase(graph[x].begin() + i);
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

    // Function to print the graph
    void print_graph()
    {
        cout << "Graph:" << endl;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < graph[i].size(); j++)
            {
                cout << graph[i][j] << " ";
            }
            cout << endl;
        }
    }

private:
    int size;
    double density;
    vector<vector<Vertex>> graph;
    vector<Vertex> vertices;
    map<pair<int, int>, Edge> edgeMap;
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
    ShortestPath(Graph graph, Vertex start, Vertex end) : graph(graph), start(start), end(end) {
        // Run Dijkstra's algorithm using the priority queue
        PriorityQueue queue;
        queue.insert(start);
        while (!queue.empty())
        {
            Vertex current = queue.top();
            queue.pop();
            if (current.get_id() == end.get_id())
            {
                break;
            }
            vector<Vertex> neighbors = graph.neighbors(current.get_id());
            for (int i = 0; i < neighbors.size(); i++)
            {
                Vertex neighbor = neighbors[i];
                double new_distance = graph.get_node_value(current.get_id()) + graph.get_node_value(neighbor.get_id());
                if (new_distance < graph.get_node_value(neighbor.get_id()))
                {
                    graph.set_node_value(neighbor.get_id(), new_distance);
                    queue.insert(neighbor);
                }
            }
        }

    }

private:
    Vertex start;
    Vertex end;
    Graph graph;
};

int main()
{
    Graph g(50, 0.1);
    g.print_graph();

    return 0;
}