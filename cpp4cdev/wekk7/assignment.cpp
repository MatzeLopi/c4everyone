// Implement a game of hex with a nxn board
// Game is interpreted as a graph with n^2 nodes
// Each node has 6 neighbors except for the border and corner nodes -> Border nodes have 4 neighbors, corner nodes have 2 or 3

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <map>
#include <random>

using namespace std;

// Function to generate a random integer in the range [min, max]
int generate_random_int(int min, int max)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// Enum class to represent the player
enum class belongsTo : short
{
    BLUE,
    RED,
    EMPTY
};

// Output stream operator for belongsTo enum class
ostream &operator<<(ostream &os, belongsTo belongs)
{
    switch (belongs)
    {
    case belongsTo::BLUE:
        os << "B";
        break;
    case belongsTo::RED:
        os << "R";
        break;
    case belongsTo::EMPTY:
        os << ".";
        break;
    default:
        os << " ";
        break;
    }
    return os;
}

// Struct to represent a tile
struct Tile
{
    int x;
    int y;
    belongsTo player;
};

class Hex
{
public:
    /**
     * @brief Constructs a Hex object with the specified size.
     *
     * @param size The size of the Hex object.
     */
    /**
     * @brief Constructs a Hex object with the specified size.
     *
     * This constructor initializes the Hex board as a graph with n^2 nodes, where n is the specified size.
     * It creates the board as a vector of vectors of Tile objects, with each Tile representing a node on the board.
     * It also creates the edges between the nodes based on the specified size.
     *
     * @param size The size of the Hex board.
     */
    Hex(int size) : size(size)
    {
        // Create the board as a graph with n^2 nodes
        board = vector<vector<Tile>>(size, vector<Tile>(size));
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                board[i][j] = {i, j, belongsTo::EMPTY};
            }
        }

        // Create the edges
        edges = vector<vector<int>>(size * size, vector<int>());

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // Upper left corner
                if (i == 0 && j == 0)
                {
                    edges[0].push_back(1);
                    edges[0].push_back(size);
                }
                // Upper right corner
                else if (i == 0 && j == size - 1)
                {
                    edges[size - 1].push_back(size - 2);
                    edges[size - 1].push_back(2 * size - 1);
                    edges[size - 1].push_back(2 * size - 2);
                }
                // Lower left corner
                else if (i == size - 1 && j == 0)
                {
                    edges[i * size].push_back((i - 1) * size);
                    edges[i * size].push_back((i - 1) * size - 2);
                    edges[i * size].push_back(i * size + 1);
                }
                // Lower right corner
                else if (i == size - 1 && j == size - 1)
                {
                    edges[size * size - 1].push_back(size * size - size);
                    edges[size * size - 1].push_back(size * size - 1);
                }
                // Check if the node is on the border North or South
                else if (i == 0 || i == size - 1)
                {

                    // Right and Left neighbors
                    edges[i * size + j].push_back(i * size + j - 1);
                    edges[i * size + j].push_back(i * size + j + 1);

                    // Lower neighbors
                    if (i == 0)
                    {
                        edges[i * size + j].push_back((i + 1) * size + j);
                        edges[i * size + j].push_back((i + 1) * size + j + 1);
                    }
                    // Upper neighbors
                    else if (i == size - 1)
                    {
                        edges[i * size + j].push_back((i - 1) * size + j);
                        edges[i * size + j].push_back((i - 1) * size + j + 1);
                    }
                }
                // Check if the node is on the border East or West
                else if (j == 0 || j == size - 1)
                {

                    edges[i * size + j].push_back((i - 1) * size + j);
                    edges[i * size + j].push_back((i + 1) * size + j);

                    if (j == 0)
                    {
                        edges[i * size + j].push_back((i - 1) * size + j + 1);
                        edges[i * size + j].push_back(i * size + j + 1);
                    }
                    else if (j == size - 1)
                    {
                        edges[i * size + j].push_back((i + 1) * size + j - 1);
                        edges[i * size + j].push_back(i * size + j - 1);
                    }
                }
                // Needs to be middle Node
                else
                {
                    edges[i * size + j].push_back((i - 1) * size + j);     // North
                    edges[i * size + j].push_back((i + 1) * size + j);     // South
                    edges[i * size + j].push_back(i * size + j - 1);       // West
                    edges[i * size + j].push_back(i * size + j + 1);       // East
                    edges[i * size + j].push_back((i - 1) * size + j - 1); // Northwest
                    edges[i * size + j].push_back((i + 1) * size + j + 1); // Southeast
                }
            }
        }
    }

    /**
     * Destructor
     */
    ~Hex()
    {
    }

    void start_game()
    {
        // Start the game
        cout << "Starting the game..." << endl;
        print_board();

        cout << "Please pick a colour: Red or Blue" << endl;
        string player_colour;
        cin >> player_colour;

        // Convert the input to lowercase
        transform(player_colour.begin(), player_colour.end(), player_colour.begin(), ::tolower);

        if (player_colour == "red")
        {
            cout << "You are Red" << endl;
            human_player = belongsTo::RED;
            cout << "AI is Blue" << endl;
            ai_player = belongsTo::BLUE;
        }
        else if (player_colour == "blue")
        {
            cout << "You are Blue" << endl;
            human_player = belongsTo::BLUE;
            cout << "AI is Red" << endl;
            ai_player = belongsTo::RED;
        }
        else
        {
            cout << "Invalid colour. Please pick Red or Blue" << endl;
            start_game();
        }

        // Set current player to player blue -> Blue always starts
        if (human_player == belongsTo::BLUE)
            currentPlayer = human_player;
        else
            currentPlayer = ai_player;

        // Game loop
        game_loop();
    }

    /**
     * Prints the game board.
     */
    void print_board()
    {

        string tabs = "";
        // Print the board
        for (int i = 0; i < size; i++)
        {
            cout << tabs;

            for (int j = 0; j < size; j++)
            {
                if (j == size - 1)
                    cout << board[i][j].player;
                else
                    cout << board[i][j].player << " - ";
            }
            cout << endl;

            // Connect the nodes using slashes
            if (i != size - 1)
            {
                cout << tabs;
                for (int j = 0; j < size; j++)
                {
                    if (j == size - 1)
                        cout << " \\";
                    else
                        cout << " \\ /";
                }
                cout << endl;
            }
            // Two spaces needed for aligning the board
            tabs += "  ";
        }
    }

    // Function to print the edges
    void print_edges()
    {
        for (int i = 0; i < size * size; i++)
        {
            cout << i << ": ";
            for (int j = 0; j < edges[i].size(); j++)
            {
                cout << edges[i][j] << " ";
            }
            cout << endl;
        }
    }

private:
    // Game loop for the Hex game
    void game_loop()
    {
        while (!check_winner())
        {

            int x, y;
            if (currentPlayer == ai_player)
            {
                ai_move();
            }
            else
            {
                if (round == 1)
                {
                    pi_rule();
                    if (currentPlayer == ai_player)
                    {
                        ai_move();
                    }
                    else
                        continue;
                }
                cout << "Player " << currentPlayer << " turn" << endl;
                cin >> x >> y;

                while (move(x, y, currentPlayer) == false)
                {
                    cout << "Invalid move" << endl;
                    cin >> x >> y;
                }
            }

            print_board();
        }
    }

    bool move(int x, int y, belongsTo player)
    {
        if (x < 0 || x >= size || y < 0 || y >= size || board[x][y].player != belongsTo::EMPTY)
            return false;
        else
        {
            set_node(x, y, player);
            update_player();
            return true;
        }
    }

    // Set Node to player
    void set_node(int x, int y, belongsTo player)
    {
        board[x][y].player = player;
        round++;
    }
    // Function to check if a path exists between two nodes using DFS
    bool check_path_exists(int start, int end)
    {
        // Create a visited array to keep track of visited nodes
        vector<bool> visited(size * size, false);

        // Call the DFS function to check for a path
        return dfs(start, end, visited);
    }

    // DFS function to traverse the graph and check for a path
    bool dfs(int current, int end, vector<bool> &visited)
    {
        // Mark the current node as visited
        visited[current] = true;

        // If the current node is the end node, return true
        if (current == end)
            return true;

        // Traverse all the neighbors of the current node
        for (int neighbor : edges[current])
        {
            // If the neighbor is not visited, recursively call DFS on it
            if (!visited[neighbor])
            {
                if (dfs(neighbor, end, visited))
                    return true;
            }
        }

        // If no path is found, return false
        return false;
    }

    // Function to check if a player has won
    bool check_winner()
    {
        // TODO Implement the check winner
        return false;
    }

    // Function to make an AI move
    void ai_move()
    {
        // Function to make an AI move
        if (round == 1)
        {
            // Randomly choose Y or N
            char choice = generate_random_int(0, 1) == 0 ? 'Y' : 'N';
            pi_rule(choice, true);
        }

        // Make a random move (for now
        int x = generate_random_int(0, size - 1);
        int y = generate_random_int(0, size - 1);

        while (move(x, y, currentPlayer) == false)
        {
            x = generate_random_int(0, size - 1);
            y = generate_random_int(0, size - 1);
        }
    }

    // Function to implement the pi rule
    void pi_rule(char choice = 'N', bool ai = false)
    {
        if (!ai)
        {
            cout << "Want to change with the opponent? (Y/N)" << endl;
            cin >> choice;
        }

        if (choice == 'Y' || choice == 'y')
        {
            belongsTo temp = human_player;
            human_player = ai_player;
            ai_player = temp;

            cout << "You are now: " << human_player << endl;
            cout << "AI is now: " << ai_player << endl;
            round++;
        }
        else if (choice == 'N' || choice == 'n')
        {
            cout << "You are still: " << human_player << endl;
            round++;
            return;
        }
        else
        {
            cout << "Invalid choice" << endl;
            pi_rule();
        }
    }

    // Function to update the player
    void update_player()
    {
        if (currentPlayer == human_player)
        {
            currentPlayer = ai_player;
            previousPlayer = human_player;
        }
        else
        {
            currentPlayer = human_player;
            previousPlayer = ai_player;
        }
    }

    int size;                   // Size of the board
    int round = 0;              // Round number
    vector<vector<Tile>> board; // The board
    vector<vector<int>> edges;  // The edges
    belongsTo human_player;     // Player 1
    belongsTo ai_player;        // Player 2
    belongsTo currentPlayer;    // Current player
    belongsTo previousPlayer;   // Previous player
};

// Main Loop
int main()
{
    srand(time(0));
    Hex hex(5);
    hex.start_game();
    hex.print_edges();
    hex.print_board();
    return 0;
}