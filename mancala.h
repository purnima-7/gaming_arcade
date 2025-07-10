#ifndef MANCALA_H
#define MANCALA_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MancalaGame {
public:
    // Constants for the board
    static const int PLAYER1_STORE = 6;
    static const int PLAYER2_STORE = 13;
    static const int TOTAL_PITS = 14;
    static const int PITS_PER_PLAYER = 6;
    static const int INITIAL_STONES = 4;

    // Constructor and destructor
    MancalaGame();
    ~MancalaGame();

    // Game state methods
    bool isPlayer1Turn() const;
    bool isGameOver() const;
    int getWinner() const; // 0 = tie, 1 = player1, 2 = player2
    int getScore(int player) const; // player 1 or 2

    // Move validation and execution
    bool isValidMove(int pit) const;
    bool makeMove(int pit);
    
    // Board state access
    int getStonesInPit(int pit) const;
    std::vector<int> getPossibleMoves() const;
    
    // Clone the game for AI simulation
    MancalaGame* clone() const;
    
    // GUI-related methods
    void displayBoard(sf::RenderWindow& window, const sf::Font& font);
    int getPitFromMousePosition(int x, int y) const;
    
    // Helper methods for rendering
    void renderPit(sf::RenderWindow& window, int pit, const sf::Font& font);
    void renderStore(sf::RenderWindow& window, int store, const sf::Font& font);
    void renderTurnIndicator(sf::RenderWindow& window, const sf::Font& font);
    void renderGameOver(sf::RenderWindow& window, const sf::Font& font);

private:
    int board[TOTAL_PITS];
    bool player1Turn;
    
    // Helper methods for game logic
    int getOpposingPit(int pit) const;
    bool isOwnPit(int pit) const;
    void collectRemainingStones();
    
    // GUI-related variables
    sf::CircleShape pitShapes[TOTAL_PITS];
    sf::RectangleShape storeShapes[2];
    const float PIT_RADIUS = 40.0f;
    const float STORE_WIDTH = 60.0f;
    const float STORE_HEIGHT = 180.0f;
    const float BOARD_MARGIN_X = 100.0f;
    const float BOARD_MARGIN_Y = 150.0f;
    const float PIT_SPACING = 100.0f;
};

#endif // MANCALA_H