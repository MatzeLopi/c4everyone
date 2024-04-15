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
const int simulations = 100;

/**
 * @brief Generates a random integer between the specified range.
 *
 * This function generates a random integer between the specified range using the Mersenne Twister engine.
 *
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 *
 * @return The random integer.

*/
int generate_random_int(int min, int max)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

/**
 * @brief Enum class to represent the player.
 *
 * This enum class represents the player as belonging to the Blue team, the Red team, or being empty.
 *
 */
enum class belongsTo : short
{
    BLUE,
    RED,
    EMPTY
};

/**
 * @brief Output stream operator for belongsTo enum class.
 *
 * This function overloads the output stream operator for the belongsTo enum class.
 * It prints the player as "B" for Blue, "R" for Red, and "." for Empty.
 *
 * @param os The output stream.
 * @param belongs The player.
 *
 * @return The output stream.
 */
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

/**
 * @brief Struct to represent a Tile on the board.
 */
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

// Operator overload for !=
bool operator!=(const Tile &lhs, const Tile &rhs)
{
    return !(lhs == rhs);
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
    /**
     * @brief Gets the neighbors of a Tile.
     *
     * This function gets the neighbors of a Tile based on the edges representation of the board.
     * It returns the neighbors of the Tile that have the same player as the Tile.
     *
     * @param tile The Tile for which to get the neighbors.
     * @param neighbors The list of neighbors.
     *
     * @return The neighbors of the Tile.
     */
    vector<Tile> get_neighbors(Tile tile, vector<Tile> neighbors, bool ignoreNSWE = false)
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
            if (ignoreNSWE)
            {
                if (n.player == tile.player && n != north && n != south && n != west && n != east)
                    neighbors.push_back(n);
                else
                    continue;
            }
            else
            {
                if (n.player == tile.player)
                    neighbors.push_back(n);
                else
                    continue;
            }
        }
        return neighbors;
    }

    /**
     * @brief Game loop.
     *
     * This function is the main game loop for the Hex game.
     * It alternates between the human player and the AI player until a winner is found.
     * The human player makes a make_move by entering the x and y coordinates of the move.
     * The AI player makes a move by performing a lookahead search based on alpha-beta pruning.
     * The game ends when a player wins.
     *
     */
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

                while (make_move(x, y, currentPlayer) == false)
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
    bool make_move(int x, int y, belongsTo player)
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

    /**
     * @brief Sets a node on the board.
     *
     * This function sets a node on the board at the specified position (x, y) for the specified player.
     * It updates the board and the edges representation of the board.
     *
     * @param x The x-coordinate of the node.
     * @param y The y-coordinate of the node.
     * @param player The player of the node.
     *
     * @return void
     */
    void set_node(int x, int y, belongsTo player)
    {
        board[x][y].player = player;

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
     * @brief Undoes a move on the board.
     *
     * This function undoes a move on the board at the specified position (x, y).
     *
     * @param x The x-coordinate of the move.
     * @param y The y-coordinate of the move.
     */
    void undo_move(int x, int y)
    {
        board[x][y].player = belongsTo::EMPTY;
        for (int i = 0; i < size * size + 4; i++)
        {
            for (int j = 0; j < edges[i].size(); j++)
            {
                if (edges[i][j].x == x && edges[i][j].y == y)
                {
                    edges[i][j].player = belongsTo::EMPTY;
                }
            }
        }
        round--;
        update_player();
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
    bool dfs(Tile current, Tile end, vector<pair<int, int>> visited, bool ignoreNSWE = false)
    {
        assert(current.player != belongsTo::EMPTY);
        assert(current.player == end.player);
        vector<Tile> neighbors;

        if (current == end)
            return true;

        visited.push_back({current.x, current.y});

        neighbors = get_neighbors(current, neighbors, ignoreNSWE);
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
    bool check_winner(bool simulation = false)
    {

        vector<pair<int, int>> visited_red;
        visited_red = vector<pair<int, int>>(size * size + 4, {-1, -1});

        if (dfs(north, south, visited_red))
        {
            if (simulation)
            {
                currentPlayer = belongsTo::RED;
                return true;
            }
            else
            {
                cout << "Red wins!" << endl;
                currentPlayer = belongsTo::RED;
                return true;
            }
        }

        vector<pair<int, int>> visited_blue;
        visited_blue = vector<pair<int, int>>(size * size + 4, {-1, -1});

        if (dfs(west, east, visited_blue))
        {
            if (simulation)
            {
                currentPlayer = belongsTo::BLUE;
                return true;
            }
            else
            {
                cout << "Blue wins!" << endl;
                currentPlayer = belongsTo::BLUE;
                return true;
            }
        }

        return false;
    }

    /**
     * @brief Gets the tiles of a player on the board.
     *
     * This function gets the tiles of a player on the board.
     *
     * @param player The player for which to get the tiles.
     *
     * @return The tiles of the player.
     */
    vector<Tile> get_player_tiles(belongsTo player)
    {
        vector<Tile> player_tiles;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j].player == player)
                {
                    player_tiles.push_back(board[i][j]);
                }
            }
        }
        return player_tiles;
    }

    /**
     * @brief Gets the longest path of a player on the board.
     *
     * This function calculates the longest path of a player on the board by performing a depth-first search from each node of the player.
     * It returns the length of the longest path.
     *
     * @param player The player for which to calculate the longest path.
     *
     * @return The length of the longest path of the player.
     */
    int get_longest_path(belongsTo player)
    {
        int longest_path = 0;
        vector<pair<int, int>> visited;
        vector<Tile> neighbors;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j].player == player)
                {
                    Tile current = board[i][j];
                    neighbors = get_neighbors(current, neighbors, true);
                    for (Tile tile : neighbors)
                    {
                        if (tile.player == player)
                        {
                            if (dfs(tile, current, visited, true))
                            {
                                longest_path++;
                            }
                        }
                    }
                }
            }
        }
        cout << longest_path << endl;
        return longest_path;
    }

    /**
     * @brief Evaluates a node using the minimax algorithm with alpha-beta pruning.
     *
     * This function evaluates a node using the minimax algorithm with alpha-beta pruning.
     * It recursively evaluates the node based on the specified depth and returns the score of the node.
     * The score is calculated as the difference between the longest path of the AI player and the longest path of the human player.
     *
     * @param tile The current Tile being evaluated.
     * @param depth The depth of the node in the search tree.
     * @param alpha The alpha value for alpha-beta pruning.
     * @param beta The beta value for alpha-beta pruning.
     * @param maximizingPlayer The maximizing player in the search tree.
     *
     */
    int evaluate_node(Tile tile, int depth, int alpha, int beta, bool maximizingPlayer)
    {
        vector<pair<int, int>> reasonableMoves; // Initialize the set of reasonable moves

        // Check if the depth is 0 or the game has ended
        if (depth == 0)
        {
            return get_longest_path(ai_player) - get_longest_path(human_player);
        }
        else if (check_winner(true))
        {
            return 1000;
        }

        vector<pair<int, int>> moves;
        vector<Tile> neighbors(6);
        neighbors = get_neighbors(tile, neighbors);

        // Iterate over the neighbors of the current tile -> If tile is empty add to moves
        for (Tile n : neighbors)
        {
            if (n.player == belongsTo::EMPTY)
            {
                moves.push_back({n.x, n.y});
            }
        }
        int eval;

        // Check if max agent
        if (maximizingPlayer)
        {

            for (auto move : moves)
            {
                // Make the move
                make_move(move.first, move.second, ai_player);

                // Update Tile
                tile = board[move.first][move.second];

                // Recursively evaluate the node
                eval = evaluate_node(tile, depth - 1, alpha, beta, false);

                // Undo the move
                undo_move(move.first, move.second);

                // Perform alpha-beta pruning
                alpha = max(alpha, eval);
                if (beta <= alpha)
                    break;
            }
            return eval;
        }
        // Check if min agent
        else
        {
            for (auto move : moves)
            {
                // Make the move
                make_move(move.first, move.second, human_player);

                // Update Tile
                tile = board[move.first][move.second];

                // Recursively evaluate the node
                eval = evaluate_node(tile, depth - 1, alpha, beta, true);

                // Undo the move
                undo_move(move.first, move.second);

                // Perform alpha-beta pruning
                beta = min(beta, eval);
                if (beta <= alpha)
                    break;
            }
            return eval;
        }
    }

    /**
     * @brief Gets the reasonable moves for the AI player.
     *
     * This function gets the reasonable moves for the AI player based on the evaluation of the nodes using the minimax algorithm with alpha-beta pruning.
     * It evaluates each node and selects the moves with the highest score.
     *
     * @return The reasonable moves for the AI player.
     */
    vector<pair<int, int>> get_reasonable_move()
    {
        vector<pair<int, int>> legalMoves = get_legal_moves(); // Get all legal moves
        vector<pair<int, int>> reasonableMoves;                // Initialize the set of reasonable moves
        map<int, pair<int, int>> moveScores;                   // Initialize the map of move scores

        // Evaluate each move
        for (auto move : legalMoves)
        {
            // Make the move
            make_move(move.first, move.second, ai_player);

            // Evaluate the node
            int eval = evaluate_node(board[move.first][move.second], 2, INT_MIN, INT_MAX, false);

            // Add the move to the set of reasonable moves
            moveScores[eval] = move;

            // Undo the move
            undo_move(move.first, move.second);
        }
        // Add the first 10 moves with the highest scores to the set of reasonable moves
        int count = 0;
        for (auto it = moveScores.rbegin(); it != moveScores.rend(); it++)
        {
            if (count == 10)
                break;
            reasonableMoves.push_back(it->second);
            count++;
        }
        return reasonableMoves;
    }

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
                Hex tempHex(*this);                                        // Create a copy of the current Hex object
                tempHex.make_move(move.first, move.second, currentPlayer); // Make move on the copy

                // Perform random moves until the game ends
                while (!tempHex.check_winner(true))
                {
                    vector<pair<int, int>> tempLegalMoves = tempHex.get_legal_moves();
                    if (tempLegalMoves.size() == 0)
                    {
                        break;
                    }
                    int randomIndex = generate_random_int(0, tempLegalMoves.size() - 1);
                    pair<int, int> randomMove = tempLegalMoves[randomIndex];

                    tempHex.make_move(randomMove.first, randomMove.second, tempHex.currentPlayer);
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

    /**
     * @brief Gets the legal moves on the board.
     *
     * This function gets the legal moves on the board.
     *
     * @return The legal moves on the board.
     */
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

    /**
     * @brief AI move.
     *
     * This function makes a move for the AI player.
     * It uses the Monte Carlo simulation to find the best move for the AI player.
     * It then makes the move on the board.
     *
     * @return void
     */
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
            make_move(best_move.first, best_move.second, currentPlayer);
        }
    }

    /**
     * @brief Pi rule.
     *
     * This function implements the Pi rule, which allows the human player to change sides with the AI player.
     * The human player can choose to change sides with the AI player or not.
     * If the human player chooses to change sides, the players are swapped.
     *
     * @param choice The choice of the human player.
     * @param ai Whether the AI player is making the move.
     *
     * @return void
     */
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