#include "ai.h"
#include <limits>
#include <algorithm>
#include <iostream>

MancalaAI::MancalaAI(int difficulty) {
    setDifficulty(difficulty);
}

void MancalaAI::setDifficulty(int diff) {
    difficulty = std::max(1, std::min(5, diff));  // Clamp between 1-5
    
    // Set the max search depth based on difficulty
    switch (difficulty) {
        case 1: maxDepth = 2; break;
        case 2: maxDepth = 3; break;
        case 3: maxDepth = 4; break;
        case 4: maxDepth = 5; break;
        case 5: maxDepth = 7; break;
        default: maxDepth = 3;
    }
}

int MancalaAI::findBestMove(const MancalaGame& game) {
    MancalaGame* gameCopy = game.clone();
    std::vector<int> possibleMoves = gameCopy->getPossibleMoves();
    
    if (possibleMoves.empty()) {
        delete gameCopy;
        return -1;  // No valid moves
    }
    
    int bestScore = std::numeric_limits<int>::min();
    int bestMove = possibleMoves[0];  // Default to first move
    
    // Try each possible move and evaluate with minimax
    for (int move : possibleMoves) {
        MancalaGame* moveSimulation = gameCopy->clone();
        bool extraTurn = false;
        
        // Make the move
        moveSimulation->makeMove(move);
        
        // Evaluate the resulting position
        bool getExtraTurn = false;
        int score = minimax(moveSimulation, maxDepth - 1, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), getExtraTurn);
        
        // Update best move if this is better
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
        
        delete moveSimulation;
    }
    
    delete gameCopy;
    return bestMove;
}

int MancalaAI::minimax(MancalaGame* game, int depth, bool isMaximizing, int alpha, int beta, bool& getExtraTurn) {
    // Terminal conditions
    if (depth == 0 || game->isGameOver()) {
        getExtraTurn = false;
        return evaluateBoard(*game);
    }
    
    std::vector<int> possibleMoves = game->getPossibleMoves();
    
    // No valid moves
    if (possibleMoves.empty()) {
        getExtraTurn = false;
        return evaluateBoard(*game);
    }
    
    if (isMaximizing) {  // AI's turn (maximizing)
        int bestScore = std::numeric_limits<int>::min();
        
        for (int move : possibleMoves) {
            MancalaGame* moveSimulation = game->clone();
            bool extraTurn = false;
            
            // Make the move and check if it results in an extra turn
            bool moveSuccess = moveSimulation->makeMove(move);
            bool gotExtraTurn = moveSuccess && moveSimulation->isPlayer1Turn() == game->isPlayer1Turn();
            
            bool dummyExtraTurn = false;
            int score;
            
            // If we get an extra turn, it's still our turn (maximizing)
            if (gotExtraTurn) {
                score = minimax(moveSimulation, depth, true, alpha, beta, dummyExtraTurn);
            } else {
                score = minimax(moveSimulation, depth - 1, false, alpha, beta, dummyExtraTurn);
            }
            
            delete moveSimulation;
            
            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
            
            if (beta <= alpha) {
                break;  // Beta cutoff
            }
        }
        
        return bestScore;
    } else {  // Human's turn (minimizing)
        int bestScore = std::numeric_limits<int>::max();
        
        for (int move : possibleMoves) {
            MancalaGame* moveSimulation = game->clone();
            bool extraTurn = false;
            
            // Make the move and check if it results in an extra turn
            bool moveSuccess = moveSimulation->makeMove(move);
            bool gotExtraTurn = moveSuccess && moveSimulation->isPlayer1Turn() == game->isPlayer1Turn();
            
            bool dummyExtraTurn = false;
            int score;
            
            // If we get an extra turn, it's still the opponent's turn (minimizing)
            if (gotExtraTurn) {
                score = minimax(moveSimulation, depth, false, alpha, beta, dummyExtraTurn);
            } else {
                score = minimax(moveSimulation, depth - 1, true, alpha, beta, dummyExtraTurn);
            }
            
            delete moveSimulation;
            
            bestScore = std::min(bestScore, score);
            beta = std::min(beta, bestScore);
            
            if (beta <= alpha) {
                break;  // Alpha cutoff
            }
        }
        
        return bestScore;
    }
}

int MancalaAI::evaluateBoard(const MancalaGame& game) {
    // Game over condition has highest priority
    if (game.isGameOver()) {
        int winner = game.getWinner();
        if (winner == 2) {  // AI wins
            return 10000;
        } else if (winner == 1) {  // Human wins
            return -10000;
        } else {  // Tie
            return 0;
        }
    }
    
    // Combine multiple evaluation factors with different weights
    int score = 0;
    
    // Main factor: Difference in stones in stores (3x weight)
    score += evaluateStonesDifference(game) * 3;
    
    // Secondary factors
    score += evaluateExtraTurnPotential(game);
    score += evaluateCapturePotential(game) * 2;
    score += evaluateStoneDistribution(game);
    
    return score;
}

int MancalaAI::evaluateStonesDifference(const MancalaGame& game) {
    // Simple difference between AI's store and human's store
    return game.getScore(2) - game.getScore(1);
}

int MancalaAI::evaluateExtraTurnPotential(const MancalaGame& game) {
    int score = 0;
    bool isAITurn = !game.isPlayer1Turn();
    
    // Check each pit on the current player's side
    int start = isAITurn ? MancalaGame::PLAYER1_STORE + 1 : 0;
    int end = isAITurn ? MancalaGame::PLAYER2_STORE : MancalaGame::PLAYER1_STORE;
    int playerStore = isAITurn ? MancalaGame::PLAYER2_STORE : MancalaGame::PLAYER1_STORE;
    
    for (int i = start; i < end; i++) {
        int stones = game.getStonesInPit(i);
        
        // If the number of stones equals the distance to the store,
        // moving from this pit would result in an extra turn
        int distanceToStore = (playerStore - i + MancalaGame::TOTAL_PITS) % MancalaGame::TOTAL_PITS;
        
        if (stones == distanceToStore) {
            score += 5;  // Potential for extra turn
        }
    }
    
    // Negate score if it's human's turn
    return isAITurn ? score : -score;
}

int MancalaAI::evaluateCapturePotential(const MancalaGame& game) {
    int score = 0;
    bool isAITurn = !game.isPlayer1Turn();
    
    // Check each pit on the current player's side
    int start = isAITurn ? MancalaGame::PLAYER1_STORE + 1 : 0;
    int end = isAITurn ? MancalaGame::PLAYER2_STORE : MancalaGame::PLAYER1_STORE;
    
    for (int i = start; i < end; i++) {
        int stones = game.getStonesInPit(i);
        
        // Empty pits on own side are potential capture targets
        if (stones == 0) {
            // Calculate which pit would land in this empty pit
            for (int j = start; j < end; j++) {
                if (j == i) continue;
                
                int distance = (i - j + MancalaGame::TOTAL_PITS) % MancalaGame::TOTAL_PITS;
                if (game.getStonesInPit(j) == distance) {
                    // The opposing pit that would be captured
                    int opposingPit = (MancalaGame::TOTAL_PITS - 2 - i);
                    int opposingStones = game.getStonesInPit(opposingPit);
                    
                    if (opposingStones > 0) {
                        // Higher score for capturing more stones
                        score += opposingStones + 1;  // +1 for the capturing stone
                    }
                }
            }
        }
    }
    
    // Negate score if it's human's turn
    return isAITurn ? score : -score;
}

int MancalaAI::evaluateStoneDistribution(const MancalaGame& game) {
    int score = 0;
    bool isAITurn = !game.isPlayer1Turn();
    
    // Evaluate AI's side (Player 2)
    int aiTotalStones = 0;
    int aiPitsWithStones = 0;
    
    for (int i = MancalaGame::PLAYER1_STORE + 1; i < MancalaGame::PLAYER2_STORE; i++) {
        int stones = game.getStonesInPit(i);
        aiTotalStones += stones;
        
        if (stones > 0) {
            aiPitsWithStones++;
        }
    }
    
    // Evaluate human's side (Player 1)
    int humanTotalStones = 0;
    int humanPitsWithStones = 0;
    
    for (int i = 0; i < MancalaGame::PLAYER1_STORE; i++) {
        int stones = game.getStonesInPit(i);
        humanTotalStones += stones;
        
        if (stones > 0) {
            humanPitsWithStones++;
        }
    }
    
    // Prefer having more stones on your side (mobility)
    int stonesDiff = aiTotalStones - humanTotalStones;
    
    // Prefer having stones distributed in multiple pits (flexibility)
    int pitsDiff = aiPitsWithStones - humanPitsWithStones;
    
    // Combine the factors
    score = stonesDiff + pitsDiff * 2;
    
    return score;
}