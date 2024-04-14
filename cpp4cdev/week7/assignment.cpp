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
const int simulations = 200;

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

// Output stream operator for Tile struct
ostream &operator<<(ostream &os, const Tile &tile)
{
    os << "(" << tile.x << ", " << tile.y << ", " << tile.player << ")";
    return os;
}

// Operator overload for ==
bool operator==(const Tile &lhs, const Tile &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.player == rhs.player;
}

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

        // Initialize the border Nodes of the board
        north.x = size;
        north.y = size;
        north.player = belongsTo::RED;

        south.x = size + 1;
        south.y = size;
        south.player = belongsTo::RED;

        west.x = size;
        west.y = size + 1;
        west.player = belongsTo::BLUE;

        east.x = size + 1;
        east.y = size + 1;
        east.player = belongsTo::BLUE;

        // Create the edges
        edges = vector<vector<Tile>>(size * size + 4, vector<Tile>());
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // Upper left corner
                if (i == 0 && j == 0)
                {
                    edges[0].push_back(board[0][1]);
                    edges[0].push_back(board[1][0]);
                    edges[0].push_back(north);
                    edges[0].push_back(west);
                }
                // Upper right corner
                else if (i == 0 && j == size - 1)
                {
                    edges[size - 1].push_back(board[0][size - 2]);
                    edges[size - 1].push_back(board[1][size - 1]);
                    edges[size - 1].push_back(board[1][size - 2]);
                    edges[size - 1].push_back(north);
                    edges[size - 1].push_back(east);
                }
                // Lower left corner
                else if (i == size - 1 && j == 0)
                {
                    edges[i * size].push_back(board[size - 1][1]);
                    edges[i * size].push_back(board[size - 2][0]);
                    edges[i * size].push_back(board[size - 2][1]);
                    edges[i * size].push_back(south);
                    edges[i * size].push_back(west);
                }
                // Lower right corner
                else if (i == size - 1 && j == size - 1)
                {
                    edges[size * size - 1].push_back(board[size - 1][size - 2]);
                    edges[size * size - 1].push_back(board[size - 2][size - 1]);
                    edges[size * size - 1].push_back(south);
                    edges[size * size - 1].push_back(east);
                }
                // Check if the node is on the border North or South
                else if (i == 0 || i == size - 1)
                {

                    // Right and Left neighbors
                    edges[i * size + j].push_back(board[i][j - 1]);
                    edges[i * size + j].push_back(board[i][j + 1]);

                    // Lower neighbors
                    if (i == 0)
                    {
                        edges[i * size + j].push_back(board[(i + 1)][j]);
                        edges[i * size + j].push_back(board[(i + 1)][j + 1]);
                        edges[i * size + j].push_back(north);
                    }
                    // Upper neighbors
                    else if (i == size - 1)
                    {
                        edges[i * size + j].push_back(board[(i - 1)][j]);
                        edges[i * size + j].push_back(board[(i - 1)][j + 1]);
                        edges[i * size + j].push_back(south);
                    }
                }
                // Check if the node is on the border East or West
                else if (j == 0 || j == size - 1)
                {

                    edges[i * size + j].push_back(board[i - 1][j]);
                    edges[i * size + j].push_back(board[i + 1][j]);

                    if (j == 0)
                    {
                        edges[i * size + j].push_back(board[i][j + 1]);
                        edges[i * size + j].push_back(board[i - 1][j + 1]);
                        edges[i * size + j].push_back(west);
                    }
                    else if (j == size - 1)
                    {
                        edges[i * size + j].push_back(board[i][j - 1]);
                        edges[i * size + j].push_back(board[i + 1][j - 1]);
                        edges[i * size + j].push_back(east);
                    }
                }
                // Needs to be middle Node
                else
                {
                    edges[i * size + j].push_back(board[i - 1][j]);
                    edges[i * size + j].push_back(board[i - 1][j + 1]);
                    edges[i * size + j].push_back(board[i][j - 1]);
                    edges[i * size + j].push_back(board[i][j + 1]);
                    edges[i * size + j].push_back(board[i + 1][j - 1]);
                    edges[i * size + j].push_back(board[i + 1][j]);
                }
            }
        }

        // Add one node for N
        for (int i = 0; i < size; i++)
        {
            edges[size * size].push_back(board[0][i]);
        }
        // Add node for S
        for (int i = 0; i < size; i++)
        {
            edges[size * size + 1].push_back(board[size - 1][i]);
        }
        // Add node for W
        for (int i = 0; i < size; i++)
        {
            edges[size * size + 2].push_back(board[i][0]);
        }
        // Add node for E
        for (int i = 0; i < size; i++)
        {
            edges[size * size + 3].push_back(board[i][size - 1]);
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
        for (int i = 0; i < size * size + 4; i++)
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
    vector<Tile> get_neighbors(Tile tile, vector<Tile> neighbors)
    {
        int index;
        if (tile.x > size || tile.y > size)
            index = size * size + tile.x % size + (tile.y % size) * 2;
        else
            index = tile.x * size + tile.y % size;
        vector<Tile> potential_neighbors(6);
        potential_neighbors = edges[index];

        for (auto n : potential_neighbors)
        {
            if (n.player == tile.player)
                neighbors.push_back(n);
            else
                continue;
        }
        return neighbors;
    }

    // Game loop for the Hex game
    void game_loop()
    {
        while (!check_winner())
        {

            int x = 0;
            int y = 0;
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
        print_board();
    }

    /**
     * @brief Makes a move on the board.
     *
     * This function makes a move on the board at the specified position (x, y) for the specified player.
     *
     * @param x The x-coordinate of the move.
     * @param y The y-coordinate of the move.
     * @param player The player making the move.
     *
     * @return True if the move is valid, false otherwise.
     */
    bool move(int x, int y, belongsTo player)
    {
        if (x < 0 || x >= size || y < 0 || y >= size || board[x][y].player != belongsTo::EMPTY)
            return false;
        else
        {
            set_node(x, y, player);
            update_player();
            // print_board();
            return true;
        }
    }

    // Set Node to player
    void set_node(int x, int y, belongsTo player)
    {
        board[x][y].player = player;

        // Need to update the Tiles in the edges representation
        for (int i = 0; i < size * size + 4; i++)
        {
            for (int j = 0; j < edges[i].size(); j++)
            {
                if (edges[i][j].x == x && edges[i][j].y == y)
                {
                    edges[i][j].player = player;
                }
            }
        }
        round++;

        last_move = board[x][y];
    }

    /**
     * @brief Depth-first search to check if a player has won.
     *
     * This function performs a depth-first search to check if a player has won the game.
     *
     * @param current The current Tile being visited.
     * @param end The end Tile to reach.
     * @param visited The list of visited Tiles.
     *
     * @return True if the player has won, false otherwise.
     */
    bool dfs(Tile current, Tile end, vector<pair<int, int>> visited)
    {
        assert(current.player != belongsTo::EMPTY);
        assert(current.player == end.player);
        vector<Tile> neighbors;

        if (current == end)
            return true;

        visited.push_back({current.x, current.y});

        neighbors = get_neighbors(current, neighbors);
        for (Tile n : neighbors)
        {
            // Check if the neighbor has already been visited
            if (find(visited.begin(), visited.end(), make_pair(n.x, n.y)) != visited.end())
                continue;
            else
            {
                if (dfs(n, end, visited))
                    return true;
            }
        }

        return false;
    }

    /**
     * @brief Checks if a player has won the game.
     *
     * This function checks if a player has won the game by performing a depth-first search from the North edge to the South edge for the Red player,
     * and from the West edge to the East edge for the Blue player.
     *
     * @return True if a player has won, false otherwise.
     */
    bool check_winner()
    {

        vector<pair<int, int>> visited_red;
        visited_red = vector<pair<int, int>>(size * size + 4, {-1, -1});

        if (dfs(north, south, visited_red))
        {
            cout << "Red wins!" << endl;
            currentPlayer = belongsTo::RED;
            return true;
        }

        vector<pair<int, int>> visited_blue;
        visited_blue = vector<pair<int, int>>(size * size + 4, {-1, -1});

        if (dfs(west, east, visited_blue))
        {
            cout << "Blue wins!" << endl;
            currentPlayer = belongsTo::BLUE;
            return true;
        }

        return false;
    }

    // TODO Implement lookahead based on a minimax algorithm

    /**
     * @brief Monte Carlo simulation to find the best move.
     *
     * This function performs a Monte Carlo simulation to find the best move for the AI player.
     * It simulates the game for a specified number of times and calculates the score based on the result of the game.
     * The move with the highest score is chosen as the best move.
     *
     * @return The best move for the AI player.
     */
    pair<int, int> monte_carlo_simulation()
    {
        // Perform Monte Carlo simulation to find the best move
        vector<pair<int, int>> legalMoves = get_legal_moves(); // Get all legal moves

        // Initialize variables to keep track of the best move
        int bestScore = INT_MIN;
        pair<int, int> bestMove;

        // Perform simulations for each legal move
        for (auto move : legalMoves)
        {
            int totalScore = 0;

            // Simulate the game for each move
            for (int i = 0; i < simulations; i++)
            {
                Hex tempHex(*this);                                   // Create a copy of the current Hex object
                tempHex.move(move.first, move.second, currentPlayer); // Make move on the copy

                // Perform random moves until the game ends
                while (!tempHex.check_winner())
                {
                    vector<pair<int, int>> tempLegalMoves = tempHex.get_legal_moves();
                    if (tempLegalMoves.size() == 0)
                    {
                        break;
                    }
                    int randomIndex = generate_random_int(0, tempLegalMoves.size() - 1);
                    pair<int, int> randomMove = tempLegalMoves[randomIndex];

                    tempHex.move(randomMove.first, randomMove.second, tempHex.currentPlayer);
                }

                // Calculate the score based on the result of the game
                if (tempHex.currentPlayer == ai_player)
                    totalScore++;
                else if (tempHex.currentPlayer == human_player)
                    totalScore--;
            }

            // Update the best move if the current move has a higher score
            if (totalScore > bestScore)
            {
                bestScore = totalScore;
                bestMove = move;
            }
        }

        // Make the best move
        return {bestMove.first, bestMove.second};
    }

    // Function to get all legal moves
    vector<pair<int, int>> get_legal_moves()
    {
        vector<pair<int, int>> legalMoves;

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j].player == belongsTo::EMPTY)
                {
                    legalMoves.push_back({i, j});
                }
            }
        }

        return legalMoves;
    }

    // Function to make an AI move
    void ai_move()
    {
        // Function to make an AI move
        if (round == 1)
        {
            // Use monte carlo simulation to check if opponent move was good
            pair<int, int> best_move = monte_carlo_simulation();
            if (best_move.first == last_move.x && best_move.second == last_move.y)
                pi_rule('Y', true);
            else
                pi_rule('N', true);
        }
        else
        {
            pair<int, int> best_move = monte_carlo_simulation();
            move(best_move.first, best_move.second, currentPlayer);
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
    Tile north;                 // North edge
    Tile south;                 // South edge
    Tile west;                  // West edge
    Tile east;                  // East edge
    Tile last_move;             // Last move
    vector<vector<Tile>> board; // The board
    vector<vector<Tile>> edges; // The edges
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
    return 0;
}