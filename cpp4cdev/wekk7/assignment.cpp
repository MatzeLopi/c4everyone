// Implement a game of hex with a nxn board
// Game is interpreted as a graph with n^2 nodes
// Each node has 6 neighbors except for the border and corner nodes -> Border nodes have 4 neighbors, corner nodes have 2 or 3

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <unordered_map>

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

class Hex
{
public:
    /**
     * @brief Constructs a Hex object with the specified size.
     *
     * @param size The size of the Hex object.
     */
    Hex(int size) : size(size)
    {
        // Create the board as a graph with n^2 nodes
        board.resize(size, vector<belongsTo>(size, belongsTo::EMPTY));
    }

    /**
     * Destructor
     */
    ~Hex()
    {
        board.clear();
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
                    cout << board[i][j];
                else
                    cout << board[i][j] << " - ";
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
        if (x < 0 || x >= size || y < 0 || y >= size)
        {
            cout << "Invalid move" << endl;
            return;
        }

        if (board[x][y] == belongsTo::EMPTY)
        {
            board[x][y] = player;
        }
        else
        {
            cout << "Invalid move" << endl;
        }

        // TODO: Logic who is next
        if (check_winner())
        {
            cout << "Player " << player << " wins!" << endl;
        }
        print_board();
    }

    bool check_winner()
    {
        // TODO: Implement the check_winner function
        return false;
    }

private:
    int size;                        // Size of the board
    vector<vector<belongsTo>> board; // The board
    belongsTo player1;               // Player 1
    belongsTo player2;               // Player 2
};

// Main Loop
int main()
{
    Hex hex(10);
    hex.start_game();
    hex.print_board();
    return 0;
}