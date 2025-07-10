#include "mancala.h"
#include <iostream>

MancalaGame::MancalaGame() {
    // Initialize the board with 4 stones in each small pit
    for (int i = 0; i < TOTAL_PITS; i++) {
        if (i == PLAYER1_STORE || i == PLAYER2_STORE) {
            board[i] = 0;  // Stores start empty
        } else {
            board[i] = INITIAL_STONES;
        }
    }
    
    // Player 1 starts
    player1Turn = true;
    
    // Initialize the GUI components
    for (int i = 0; i < TOTAL_PITS; i++) {
        if (i == PLAYER1_STORE) {
            storeShapes[0].setSize(sf::Vector2f(STORE_WIDTH, STORE_HEIGHT));
            storeShapes[0].setPosition(BOARD_MARGIN_X + PIT_SPACING * PITS_PER_PLAYER + 20, BOARD_MARGIN_Y);
            storeShapes[0].setFillColor(sf::Color(200, 200, 255));
            storeShapes[0].setOutlineColor(sf::Color::Black);
            storeShapes[0].setOutlineThickness(2.0f);
        } else if (i == PLAYER2_STORE) {
            storeShapes[1].setSize(sf::Vector2f(STORE_WIDTH, STORE_HEIGHT));
            storeShapes[1].setPosition(BOARD_MARGIN_X - STORE_WIDTH - 20, BOARD_MARGIN_Y);
            storeShapes[1].setFillColor(sf::Color(255, 200, 200));
            storeShapes[1].setOutlineColor(sf::Color::Black);
            storeShapes[1].setOutlineThickness(2.0f);
        } else {
            pitShapes[i].setRadius(PIT_RADIUS);
            
            // Position pits based on their index
            float x, y;
            if (i < PLAYER1_STORE) {  // Player 1's pits (bottom row)
                x = BOARD_MARGIN_X + (PITS_PER_PLAYER - 1 - i) * PIT_SPACING;
                y = BOARD_MARGIN_Y + PIT_SPACING;
                pitShapes[i].setFillColor(sf::Color(200, 255, 200));
            } else {  // Player 2's pits (top row)
                x = BOARD_MARGIN_X + (i - PLAYER1_STORE - 1) * PIT_SPACING;
                y = BOARD_MARGIN_Y;
                pitShapes[i].setFillColor(sf::Color(255, 255, 200));
            }
            
            pitShapes[i].setPosition(x, y);
            pitShapes[i].setOutlineColor(sf::Color::Black);
            pitShapes[i].setOutlineThickness(2.0f);
        }
    }
}

MancalaGame::~MancalaGame() {
    // No dynamic allocations to clean up
}

bool MancalaGame::isPlayer1Turn() const {
    return player1Turn;
}

bool MancalaGame::isGameOver() const {
    // Check if all pits on one side are empty
    bool player1SideEmpty = true;
    bool player2SideEmpty = true;
    
    for (int i = 0; i < PITS_PER_PLAYER; i++) {
        if (board[i] > 0) {
            player1SideEmpty = false;
        }
        if (board[i + PLAYER1_STORE + 1] > 0) {
            player2SideEmpty = false;
        }
    }
    
    return player1SideEmpty || player2SideEmpty;
}

int MancalaGame::getWinner() const {
    if (!isGameOver()) {
        return 0;  // Game not over yet
    }
    
    // Count stones in each player's store
    int player1Score = board[PLAYER1_STORE];
    int player2Score = board[PLAYER2_STORE];
    
    // Include stones remaining on the board in the final score
    for (int i = 0; i < PITS_PER_PLAYER; i++) {
        player1Score += board[i];
        player2Score += board[i + PLAYER1_STORE + 1];
    }
    
    if (player1Score > player2Score) {
        return 1;  // Player 1 wins
    } else if (player2Score > player1Score) {
        return 2;  // Player 2 wins
    } else {
        return 0;  // Tie
    }
}

int MancalaGame::getScore(int player) const {
    if (player == 1) {
        return board[PLAYER1_STORE];
    } else {
        return board[PLAYER2_STORE];
    }
}

bool MancalaGame::isValidMove(int pit) const {
    // Check if the pit exists and belongs to the current player
    if (pit < 0 || pit >= TOTAL_PITS || pit == PLAYER1_STORE || pit == PLAYER2_STORE) {
        return false;
    }
    
    // Check if the pit belongs to the current player
    if (player1Turn && pit >= PLAYER1_STORE) {
        return false;
    }
    if (!player1Turn && pit < PLAYER1_STORE) {
        return false;
    }
    
    // Check if the pit has stones
    return board[pit] > 0;
}

bool MancalaGame::makeMove(int pit) {
    if (!isValidMove(pit)) {
        return false;
    }
    
    // Pick up all stones from the selected pit
    int stones = board[pit];
    board[pit] = 0;
    
    int currentPit = pit;
    
    // Distribute stones
    while (stones > 0) {
        // Move to the next pit
        currentPit = (currentPit + 1) % TOTAL_PITS;
        
        // Skip opponent's store
        if ((player1Turn && currentPit == PLAYER2_STORE) || 
            (!player1Turn && currentPit == PLAYER1_STORE)) {
            continue;
        }
        
        // Drop a stone
        board[currentPit]++;
        stones--;
    }
    
    // Check for capture
    if (board[currentPit] == 1 && isOwnPit(currentPit) && currentPit != PLAYER1_STORE && currentPit != PLAYER2_STORE) {
        int opposingPit = getOpposingPit(currentPit);
        if (board[opposingPit] > 0) {
            // Capture the stones
            int playerStore = player1Turn ? PLAYER1_STORE : PLAYER2_STORE;
            board[playerStore] += board[opposingPit] + 1;  // Opposing stones + the last stone placed
            board[opposingPit] = 0;
            board[currentPit] = 0;
        }
    }
    
    // Check if the game is over
    if (isGameOver()) {
        collectRemainingStones();
        return true;
    }
    
    // Extra turn if last stone lands in player's store
    if ((player1Turn && currentPit == PLAYER1_STORE) || 
        (!player1Turn && currentPit == PLAYER2_STORE)) {
        return true;  // Player gets another turn
    }
    
    // Switch turns
    player1Turn = !player1Turn;
    return true;
}

int MancalaGame::getStonesInPit(int pit) const {
    if (pit >= 0 && pit < TOTAL_PITS) {
        return board[pit];
    }
    return 0;
}

std::vector<int> MancalaGame::getPossibleMoves() const {
    std::vector<int> moves;
    
    int start = player1Turn ? 0 : PLAYER1_STORE + 1;
    int end = player1Turn ? PLAYER1_STORE : TOTAL_PITS - 1;
    
    for (int i = start; i < end; i++) {
        if (board[i] > 0) {
            moves.push_back(i);
        }
    }
    
    return moves;
}

MancalaGame* MancalaGame::clone() const {
    MancalaGame* newGame = new MancalaGame();
    
    // Copy the board state
    for (int i = 0; i < TOTAL_PITS; i++) {
        newGame->board[i] = board[i];
    }
    
    // Copy the turn state
    newGame->player1Turn = player1Turn;
    
    return newGame;
}

void MancalaGame::displayBoard(sf::RenderWindow& window, const sf::Font& font) {
    // Clear the window
    window.clear(sf::Color(240, 240, 240));
    
    // Draw all pits and stores
    for (int i = 0; i < TOTAL_PITS; i++) {
        if (i == PLAYER1_STORE || i == PLAYER2_STORE) {
            renderStore(window, i, font);
        } else {
            renderPit(window, i, font);
        }
    }
    
    // Draw turn indicator
    renderTurnIndicator(window, font);
    
    // If game is over, show the winner
    if (isGameOver()) {
        renderGameOver(window, font);
    }
}

int MancalaGame::getPitFromMousePosition(int x, int y) const {
    // Check pits
    for (int i = 0; i < TOTAL_PITS; i++) {
        if (i == PLAYER1_STORE || i == PLAYER2_STORE) {
            continue;  // Skip stores
        }
        
        sf::FloatRect bounds = pitShapes[i].getGlobalBounds();
        if (bounds.contains(x, y)) {
            return i;
        }
    }
    
    return -1;  // No pit selected
}

void MancalaGame::renderPit(sf::RenderWindow& window, int pit, const sf::Font& font) {
    // Draw the pit
    window.draw(pitShapes[pit]);
    
    // Draw the number of stones
    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(board[pit]));
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    
    // Center the text in the pit
    sf::FloatRect bounds = pitShapes[pit].getGlobalBounds();
    text.setPosition(
        bounds.left + bounds.width / 2 - text.getLocalBounds().width / 2,
        bounds.top + bounds.height / 2 - text.getLocalBounds().height / 2
    );
    
    window.draw(text);
    
    // Highlight valid moves for the current player
    if (isValidMove(pit)) {
        sf::CircleShape highlight = pitShapes[pit];
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Green);
        highlight.setOutlineThickness(3.0f);
        window.draw(highlight);
    }
}

void MancalaGame::renderStore(sf::RenderWindow& window, int store, const sf::Font& font) {
    // Draw the store
    int storeIndex = (store == PLAYER1_STORE) ? 0 : 1;
    window.draw(storeShapes[storeIndex]);
    
    // Draw the number of stones
    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(board[store]));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    
    // Center the text in the store
    sf::FloatRect bounds = storeShapes[storeIndex].getGlobalBounds();
    text.setPosition(
        bounds.left + bounds.width / 2 - text.getLocalBounds().width / 2,
        bounds.top + bounds.height / 2 - text.getLocalBounds().height / 2
    );
    
    window.draw(text);
    
    // Draw player labels
    sf::Text label;
    label.setFont(font);
    label.setCharacterSize(16);
    label.setFillColor(sf::Color::Black);
    
    if (store == PLAYER1_STORE) {
        label.setString("Player 1");
    } else {
        label.setString("Player 2");
    }
    
    label.setPosition(
        bounds.left + bounds.width / 2 - label.getLocalBounds().width / 2,
        bounds.top + bounds.height + 10
    );
    
    window.draw(label);
}

void MancalaGame::renderTurnIndicator(sf::RenderWindow& window, const sf::Font& font) {
    sf::Text text;
    text.setFont(font);
    text.setString(player1Turn ? "Player 1's Turn" : "Player 2's Turn");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(BOARD_MARGIN_X, BOARD_MARGIN_Y + STORE_HEIGHT + 50);
    
    window.draw(text);
}

void MancalaGame::renderGameOver(sf::RenderWindow& window, const sf::Font& font) {
    int winner = getWinner();
    
    sf::Text text;
    text.setFont(font);
    
    if (winner == 0) {
        text.setString("Game Over! It's a tie!");
    } else {
        text.setString("Game Over! Player " + std::to_string(winner) + " wins!");
    }
    
    text.setCharacterSize(28);
    text.setFillColor(sf::Color::Red);
    text.setPosition(BOARD_MARGIN_X, BOARD_MARGIN_Y + STORE_HEIGHT + 90);
    
    window.draw(text);
}

int MancalaGame::getOpposingPit(int pit) const {
    if (pit < PLAYER1_STORE) {
        // Player 1's pit, return the opposing Player 2's pit
        return TOTAL_PITS - 2 - pit;
    } else if (pit > PLAYER1_STORE && pit < PLAYER2_STORE) {
        // Player 2's pit, return the opposing Player 1's pit
        return TOTAL_PITS - 2 - pit;
    }
    
    return -1;  // Not a regular pit
}

bool MancalaGame::isOwnPit(int pit) const {
    if (player1Turn) {
        return pit < PLAYER1_STORE;
    } else {
        return pit > PLAYER1_STORE && pit < PLAYER2_STORE;
    }
}

void MancalaGame::collectRemainingStones() {
    // Collect all stones from Player 1's side
    for (int i = 0; i < PITS_PER_PLAYER; i++) {
        board[PLAYER1_STORE] += board[i];
        board[i] = 0;
    }
    
    // Collect all stones from Player 2's side
    for (int i = PLAYER1_STORE + 1; i < PLAYER2_STORE; i++) {
        board[PLAYER2_STORE] += board[i];
        board[i] = 0;
    }
}