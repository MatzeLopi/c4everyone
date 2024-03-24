// Homework of week 3
// Populate graph based on dessity

#include <iostream>
#include <random>
#include <iomanip>
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
// Vertex class
class Vertex
{
public:
    // Default constructor
    Vertex(int id, double data = 0.0) : id(id), data(data) {}

    // Getter for id
    int get_id() const
    {
        return id;
    }
    double data;

private:
    int id;
};

// Graph class
class Graph
{

public:
    // Constructor for Graph
    Graph(int size = 0, double density = 0.0) : size(size), density(density), vertices()
    {
        // Create a graph
        double **graph;
        graph = new double *[size];
        for (int i = 0; i < size; i++)
        {
            graph[i] = new double[size];
            this->vertices.push_back(Vertex(i));
        }
        this->graph = graph;
    }

    // Destructor
    ~Graph()
    {
        for (int i = 0; i < size; i++)
        {
            delete[] graph[i];
        }
        delete[] graph;
    }

    // Function to populate the graph
    void populate_graph(double lower_limit = 0.0, double upper_limit = 1.0)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // Graph should not have self loops
                if (i == j)
                {
                    graph[i][j] = false;
                    continue;
                }
                if (getRandomDouble(0.0, 1.0) < density)
                {
                    graph[i][j] = getRandomDouble(lower_limit, upper_limit);
                }
                else
                {
                    graph[i][j] = false;
                }
            }
        }
    }

    bool adjacent(int x, int y)
    {
        return graph[x][y];
    }

    // Function to return a vector of neighbors
    vector<int> neighbors(int x)
    {
        vector<int> result;
        for (int i = 0; i < size; i++)
        {
            if (graph[x][i])
            {
                result.push_back(i);
            }
        }
        return result;
    }

    // Add Edge
    void add_edge(int x, int y, bool weight)
    {
        graph[x][y] = weight;
    }

    // Delete Edge
    void delete_edge(int x, int y)
    {
        graph[x][y] = false;
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
            for (int j = 0; j < size; j++)
            {
                if (graph[i][j])

                    cout << 1 << " ";
                else
                    cout << 0 << " ";
            }
            cout << endl;
        }
    }

    // Function to print the graph with weights
    void print_with_weights()
    {
        cout << "Graph:" << endl;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                cout << fixed << setprecision(2) << graph[i][j] << " ";
            }
            cout << endl;
        }
    }

private:
    int size;
    double density;
    double **graph;
    vector<Vertex> vertices;
};

class PriorityQueue
// TODO: Implement the priority queue
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

private:
    vector<Vertex> queue;
};

int main()
{
    Graph g(5, 0.5);
    g.populate_graph(1, 10);
    g.print_graph();
    g.print_with_weights();
    return 0;
}