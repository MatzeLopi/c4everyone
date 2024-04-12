// Implement a game of hex with a nxn board
// Game is interpreted as a graph with n^2 nodes
// Each node has 6 neighbors except for the border and corner nodes -> Border nodes have 4 neighbors, corner nodes have 2 or 3

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

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
        os << "X";
        break;
    case belongsTo::RED:
        os << "O";
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
                    if (j > 0 && j < size - 1)
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
                }
                // Check if the node is on the border East or West
                else if (j == 0 || j == size - 1)
                {
                    if (i > 0 && i < size - 1)
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
            cout << "Player 1 is Red" << endl;
            player1 = belongsTo::RED;
            cout << "Player 2 is Blue" << endl;
            player2 = belongsTo::BLUE;
        }
        else if (player_colour == "blue")
        {
            cout << "Player 1 is Blue" << endl;
            player1 = belongsTo::BLUE;
            cout << "Player 2 is Red" << endl;
            player2 = belongsTo::RED;
        }
        else
        {
            cout << "Invalid colour. Please pick Red or Blue" << endl;
            start_game();
        }

        // Set current player to player blue -> Blue always starts
        if (player1 == belongsTo::BLUE)
            currentPlayer = player1;
        else
            currentPlayer = player2;
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

    // Function to make a move
    void make_move(int x, int y, belongsTo player)
    {
        if (player != currentPlayer)
        {
            cout << "It is not your turn" << endl;
            return;
        }
        if (x < 0 || x >= size || y < 0 || y >= size || board[x][y].player != belongsTo::EMPTY)
        {
            cout << "Invalid move" << endl;
            return;
        }

        board[x][y].player = player;

        // TODO: Logic who is next
        if (check_winner())
        {
            cout << "Player " << player << " wins!" << endl;
        }
        print_board();
    }

private:
    // Game loop for the Hex game
    void game_loop()
    {
    }

    // Function to check if a player has won
    bool check_winner()
    {
        // TODO: Implement the check_winner function
        return false;
    }

    // Function to update the player
    void update_player()
    {
        if (currentPlayer == player1)
        {
            currentPlayer = player2;
            previousPlayer = player1;
        }
        else
        {
            currentPlayer = player1;
            previousPlayer = player2;
        }
    }

    int size;                   // Size of the board
    vector<vector<Tile>> board; // The board
    vector<vector<int>> edges;  // The edges
    belongsTo player1;          // Player 1
    belongsTo player2;          // Player 2
    belongsTo currentPlayer;    // Current player
    belongsTo previousPlayer;   // Previous player
};

// Main Loop
int main()
{
    Hex hex(5);
    hex.start_game();
    hex.print_board();
    return 0;
}