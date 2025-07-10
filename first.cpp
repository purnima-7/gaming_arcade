#include <iostream>
#include <vector>
using namespace std;

class Mancala {
private:
    vector<int> board; // 14 pits: 0-5 (P1), 6 (P1 store), 7-12 (P2), 13 (P2 store)
    bool player1Turn;

public:
    Mancala() {
        board = vector<int>(14, 4);
        board[6] = 0;  // P1 store
        board[13] = 0; // P2 store
        player1Turn = true;
        board[6] = board[13] = 0;
    }

    void displayBoard() {
        cout << "\n   ";
        for (int i = 12; i >= 7; --i) cout << board[i] << " ";
        cout << "\n" << board[13] << "                  " << board[6] << "\n   ";
        for (int i = 0; i <= 5; ++i) cout << board[i] << " ";
        cout << "\n";
    }

    bool isGameOver() {
        int sum1 = 0, sum2 = 0;
        for (int i = 0; i < 6; ++i) sum1 += board[i];
        for (int i = 7; i < 13; ++i) sum2 += board[i];
        return sum1 == 0 || sum2 == 0;
    }

    void collectRemainingStones() {
        for (int i = 0; i < 6; ++i) {
            board[6] += board[i];
            board[i] = 0;
        }
        for (int i = 7; i < 13; ++i) {
            board[13] += board[i];
            board[i] = 0;
        }
    }

    void playTurn(int pit) {
        int index = pit;
        int stones = board[index];
        board[index] = 0;

        while (stones > 0) {
            index = (index + 1) % 14;
            if ((player1Turn && index == 13) || (!player1Turn && index == 6)) continue; // skip opponent store
            board[index]++;
            stones--;
        }

        // Capture condition
        if (player1Turn && index >= 0 && index < 6 && board[index] == 1) {
            int opposite = 12 - index;
            board[6] += board[opposite] + 1;
            board[index] = board[opposite] = 0;
        } else if (!player1Turn && index >= 7 && index < 13 && board[index] == 1) {
            int opposite = 12 - index;
            board[13] += board[opposite] + 1;
            board[index] = board[opposite] = 0;
        }

        // Extra turn
        if ((player1Turn && index == 6) || (!player1Turn && index == 13)) {
            cout << "Extra turn!\n";
        } else {
            player1Turn = !player1Turn;
        }
    }

    void playGame() {
        while (!isGameOver()) {
            displayBoard();
            cout << (player1Turn ? "Player 1" : "Player 2") << "'s turn. Choose pit (0-5 for P1, 7-12 for P2): ";
            int pit;
            cin >> pit;

            // Validate move
            if (pit < 0 || pit >= 14 || board[pit] == 0 || (player1Turn && pit > 5) || (!player1Turn && (pit < 7 || pit > 12))) {
                cout << "Invalid move. Try again.\n";
                continue;
            }

            playTurn(pit);
        }

        collectRemainingStones();
        displayBoard();

        cout << "Game over!\n";
        if (board[6] > board[13]) cout << "Player 1 wins!\n";
        else if (board[6] < board[13]) cout << "Player 2 wins!\n";
        else cout << "It's a tie!\n";
    }
};

int main() {
    Mancala game;
    game.playGame();
    return 0;
}
