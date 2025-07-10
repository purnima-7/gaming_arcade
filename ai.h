#ifndef AI_H
#define AI_H

#include "mancala.h"
#include <vector>

class MancalaAI {
public:
    // Constructor with difficulty level (affects search depth)
    MancalaAI(int difficulty = 2);
    
    // Find the best move for the current game state
    int findBestMove(const MancalaGame& game);
    
    // Set the AI difficulty level
    void setDifficulty(int difficulty);
    
private:
    int difficulty;
    int maxDepth;  // Calculated from difficulty
    
    // Minimax algorithm with alpha-beta pruning
    int minimax(MancalaGame* game, int depth, bool isMaximizing, int alpha, int beta, bool& getExtraTurn);
    
    // Evaluation function to score a game state
    int evaluateBoard(const MancalaGame& game);
    
    // Helper functions for evaluation
    int evaluateStonesDifference(const MancalaGame& game);
    int evaluateExtraTurnPotential(const MancalaGame& game);
    int evaluateCapturePotential(const MancalaGame& game);
    int evaluateStoneDistribution(const MancalaGame& game);
};

#endif // AI_H