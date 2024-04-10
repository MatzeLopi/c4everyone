/**
 *  Homework 3 of the C++ for C Developers course Part A
 *
 * I went with the list representation of the graph, since it should at least represent 50 vertices and the adjacency matrix get large fast.
 * Also most real world graphs are sparse, so the adjacency list representation is more efficient. The graph is populated with random weights between 0 and 10. (Can be changed as you like)
 *
 * Everything else should be fairly straight forward.
 **/

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

// Operator overload for comparison of Vertex objects
bool operator<(const Vertex &v1, const Vertex &v2)
{
    return v1.get_id() < v2.get_id();
}

bool operator==(const Vertex &v1, const Vertex &v2)
{
    return v1.get_id() == v2.get_id();
}

bool operator!=(const Vertex &v1, const Vertex &v2)
{
    return v1.get_id() != v2.get_id();
}

bool operator>(const Vertex &v1, const Vertex &v2)
{
    return v1.get_id() > v2.get_id();
}
bool operator<=(const Vertex &v1, const Vertex &v2)
{
    return v1.get_id() <= v2.get_id();
}

bool operator>=(const Vertex &v1, const Vertex &v2)
{
    return v1.get_id() >= v2.get_id();
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

struct Node
{
    Vertex vertex;
    double priority;
};

// Priority Queue
class PriorityQueue
{
public:
    PriorityQueue() : queue() {}

    // Destructor
    ~PriorityQueue()
    {
        queue.clear();
    }

    int size()
    {
        return queue.size();
    }

    Vertex top()
    {
        return queue.front().vertex;
    }

    Vertex pop()
    {
        Vertex vertex = queue.front().vertex;
        queue.erase(queue.begin());
        return vertex;
    }

    bool contains(Vertex vertex)
    {
        for (int i = 0; i < queue.size(); i++)
        {
            if (queue[i].vertex.get_id() == vertex.get_id())
            {
                return true;
            }
        }
        return false;
    }

    void change_priority(Vertex vertex, double priority)
    {
        for (int i = 0; i < queue.size(); i++)
        {
            if (queue[i].vertex.get_id() == vertex.get_id())
            {
                queue[i].priority = priority;
                break;
            }
        }
        sort_queue();
    }

    bool empty()
    {
        return queue.empty();
    }

    void insert(Vertex vertex, double priority)
    {
        Node node{vertex, priority};
        queue.push_back(node);
        sort_queue();
    }

    void print_queue()
    {
        for (int i = 0; i < queue.size(); i++)
        {
            cout << queue[i].vertex << " : " << queue[i].priority << endl;
        }
    }

private:
    vector<Node> queue;

    void sort_queue()
    {
        sort(queue.begin(), queue.end(), [](Node a, Node b)
             { return a.priority < b.priority; });
    }
};

// Shortest Path

class ShortestPath
{
public:
    // Constructor
    ShortestPath(Graph graph, Vertex start, Vertex end) : graph(graph), start(start), end(end)
    {
        PriorityQueue pq = PriorityQueue();

        pq.insert(start, 0.0);
        min_cost.insert({start, 0.0});

        while (!pq.empty())
        {
            Vertex current_vertex = pq.pop();
            if (current_vertex.get_id() == end.get_id())
            {
                cout << "Shortest path found!" << endl;
                break;
            }
            vector<Vertex> neighbors = graph.neighbors(current_vertex.get_id());
            for (int i = 0; i < neighbors.size(); i++)
            {
                Vertex neighbor = neighbors[i];
                double edge_weight = graph.get_edge_value(current_vertex, neighbor);
                double cost = min_cost[current_vertex] + edge_weight;
                // Check if the neighbor has not been visited or the cost is less than the previous cost
                if (!min_cost.count(neighbor) || cost < min_cost[neighbor])
                {
                    min_cost[neighbor] = cost;
                    pq.insert(neighbor, cost);
                }
            }
        }
        print_path();
    }

    void print_path()
    {
        cout << "Shortest path from " << start << " to " << end << " is: " << min_cost[end] << endl;

        // Walk the path backwards
        vector<Vertex> path;
        Vertex current_vertex = end;
        while (current_vertex != start)
        {
            cout << current_vertex << " <- ";
            vector<Vertex> neighbors = graph.neighbors(current_vertex.get_id());
            PriorityQueue pq = PriorityQueue();
            for (int i = 0; i < neighbors.size(); i++)
            {
                Vertex neighbor = neighbors[i];
                pq.insert(neighbor, min_cost[neighbor]);
            }
            current_vertex = pq.pop();
            // Check if current Vertex is already in the path
            if (find(path.begin(), path.end(), current_vertex) != path.end())
            {
                cout << "\n Cycle detected! " << current_vertex << " is already in the path. Exiting..." << endl;
                break;
            }
            else
                path.push_back(current_vertex);
        }
        cout << start << endl;
    }
    // Get cost of shortest path
    double get_cost()
    {
        return min_cost[end];
    }

private:
    Graph graph;
    Vertex start;
    Vertex end;
    map<Vertex, double> min_cost;
};

int main()
{
    // Generate a graph with 50 vertices and a density of 0.1
    string out_message = "I decided to go with the adjacent list representation for the graph since the graph is sparc populated and relatively big.\n I also decided to populate the graph with random weights between 0 and 10. \n The graph is then printed and a shortest path is calculated between two vertices. \n I learned much about Graphs and Priority Queue";
    Graph g20(50, 0.2);
    Graph g40(50, 0.4);
    g20.populate_graph(0.0, 10.0);
    g40.populate_graph(0.0, 10.0);

    // Can be used for debugging
    // g20.print_graph();
    // g40.print_graph();

    Vertex start = Vertex(0, 0.0);
    Vertex v2 = Vertex(10, 0.0);
    vector<double> costs20;
    for (int i = 1; i < 50; i++)
    {
        Vertex v = Vertex(i, 0.0);
        ShortestPath sp = ShortestPath(g20, start, v);
        costs20.push_back(sp.get_cost());
    }

    double sum = 0.0;
    for (double cost : costs20)
    {
        sum += cost;
    }
    double average = sum / costs20.size();

    vector<double> costs40;
    for (int i = 1; i < 50; i++)
    {
        Vertex v = Vertex(i, 0.0);
        ShortestPath sp = ShortestPath(g40, start, v);
        costs40.push_back(sp.get_cost());
    }

    double sum40 = 0.0;
    for (double cost : costs40)
    {
        sum40 += cost;
    }
    double average40 = sum40 / costs40.size();

    cout << "\n\n\n"
         << endl;
    cout << out_message << endl;
    cout << "Average cost Graph 20%: " << average << endl;
    cout << "Average cost Graph 40%: " << average40 << endl;
    return 0;
}
