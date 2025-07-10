# Mancala Game with AI

This project implements a fully functional Mancala game with an AI opponent using the Minimax algorithm with Alpha-Beta pruning.

## Game Rules

Mancala is a two-player turn-based strategy game played on a board with 14 pits: 6 small pits for each player and 2 stores (one for each player).

### Setup
- 4 stones in each of the 12 small pits
- Empty stores at the beginning
- Player 1 (human) plays first

### Basic Movement
1. On your turn, select one of your 6 pits that contains stones
2. All stones from that pit are picked up
3. Moving counter-clockwise, drop one stone in each pit, including your own store but skipping your opponent's store
4. If the last stone lands in your store, you get an extra turn

### Capture Rules
- If the last stone lands in an empty pit on your side, you capture:
  - The stone you just placed
  - All stones in the directly opposite pit on your opponent's side
- Captured stones go into your store

### Game End
- The game ends when all six pits on one side are empty
- The player with stones remaining in their pits adds those to their store
- The player with more stones in their store wins

## AI Implementation

The AI player uses the Minimax algorithm with Alpha-Beta pruning to find the optimal move. The evaluation function considers several factors:

1. Difference in stones in the stores (primary factor)
2. Potential for extra turns
3. Potential for captures
4. Distribution of stones on the board

You can choose from three difficulty levels:
- Easy: Low search depth (2)
- Medium: Moderate search depth (4)
- Hard: High search depth (7)

## Project Structure

```
mancala_project/
│
├── src/
│   ├── main.cpp        // Game loop and GUI
│   ├── mancala.h/cpp   // Core game logic
│   ├── ai.h/cpp        // Minimax AI
│   └── assets/         // Fonts, images
│
├── CMakeLists.txt
└── README.md
```

## Building the Project

### Prerequisites
- C++14 compatible compiler
- CMake 3.10 or higher
- SFML 2.5 or higher

### Build Instructions

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/mancala-game.git
   cd mancala-game
   ```

2. Create a build directory:
   ```
   mkdir build
   cd build
   ```

3. Configure and build:
   ```
   cmake ..
   make
   ```

4. Run the game:
   ```
   ./mancala
   ```

## Controls

- In the menu:
  - Click on difficulty buttons to start a new game
  - Click on "Quit" to exit

- During gameplay:
  - Click on one of your pits (bottom row) to make a move
  - After the game ends, click anywhere to return to the menu

## Technical Details

### Minimax Algorithm

The AI uses a Minimax search with Alpha-Beta pruning to efficiently explore the game tree and find the optimal move. The search depth varies by difficulty level.

### Evaluation Function

The board evaluation considers multiple heuristics:
1. The difference between AI's store and player's store (3x weight)
2. Potential for gaining extra turns
3. Potential for capturing opponent's stones (2x weight)
4. Distribution of stones (favors having more stones spread across multiple pits)

### Game State Representation

The game state is represented by a 14-element array, with indices:
- 0-5: Player 1's pits
- 6: Player 1's store
- 7-12: Player 2's pits
- 13: Player 2's store

## Extensions and Future Improvements

Possible enhancements to consider:
1. Adding sound effects
2. Implementing different visual themes
3. Adding a history of moves
4. Creating a replay functionality
5. Supporting network play between two human players
6. Implementing a machine learning-based AI

## License

This project is licensed under the MIT License - see the LICENSE file for details.