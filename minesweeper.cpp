#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

// ANSI color codes for terminal output
#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

class Minesweeper {
private:
    vector<vector<char> > board;      // Stores the actual board with mines and numbers
    vector<vector<bool> > revealed;   // Stores the revealed state of each cell
    vector<vector<bool> > flagged;    // Stores the flagged state of each cell
    int rows, cols, mines;
    int firstRow = -200, firstCol = -200;

    void generateMines() {
        srand(time(0));
        int placedMines = 0;
        while (placedMines < mines) {
            int r = rand() % rows;
            int c = rand() % cols;
            if (board[r][c] == '*') continue;  // Skip if a mine is already there
            if ((r >= (firstRow - 1)) && (r <= (firstRow + 1)) && (c >= (firstCol - 1)) && (c <= (firstCol + 1))) continue; //Skip if in the 9 grids around first click
            board[r][c] = '*';
            placedMines++;
        }
    }

    void calculateNumbers() {
        // For every cell, calculate how many mines are in the neighboring cells
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (board[r][c] == '*') continue;

                int mineCount = 0;
                // Check all 8 neighbors
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dc = -1; dc <= 1; dc++) {
                        int nr = r + dr;
                        int nc = c + dc;
                        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc] == '*') {
                            mineCount++;
                        }
                    }
                }
                if (mineCount > 0) board[r][c] = mineCount + '0';  // Store number as character
                else board[r][c] = '0';  // No neighboring mines
            }
        }
    }

    void printBoard(bool revealAll = false) {
        cout << "Current Board:\n";
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (revealAll || revealed[r][c]) {
                    // Color the numbers
                    if (board[r][c] == '0') {
                        cout << BLACK << board[r][c] << " " << RESET;
                    } else if (board[r][c] == '1') {
                        cout << BLUE << board[r][c] << " " << RESET;
                    } else if (board[r][c] == '2') {
                        cout << GREEN << board[r][c] << " " << RESET;
                    } else if (board[r][c] == '3') {
                        cout << RED << board[r][c] << " " << RESET;
                    } else if (board[r][c] == '4') {
                        cout << MAGENTA << board[r][c] << " " << RESET;
                    } else if (board[r][c] == '*') {
                        cout << RED << board[r][c] << " " << RESET;
                    } else {
                        cout << WHITE << board[r][c] << " " << RESET;
                    }
                } else if (flagged[r][c]) {
                    cout << YELLOW << "F " << RESET;  // Flagged cells are shown in yellow
                } else {
                    cout << "# ";  // Hidden cell
                }
            }
            cout << endl;
        }
    }

    bool isValid(int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }

    void revealCell(int r, int c) {
        if (!isValid(r, c) || revealed[r][c] || flagged[r][c]) return;  // Skip if out of bounds, already revealed, or flagged
        revealed[r][c] = true;

        // If it's a '0', reveal its neighbors recursively
        if (board[r][c] == '0') {
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    revealCell(r + dr, c + dc);
                }
            }
        }
    }

    bool checkWin() {
        int unrevealedCount = 0;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (!revealed[r][c] && board[r][c] != '*') {
                    unrevealedCount++;
                }
            }
        }
        return unrevealedCount == 0;  // Win if all non-mine cells are revealed
    }

public:
    Minesweeper(int r, int c, int m) : rows(r), cols(c), mines(m) {
        board.resize(rows, vector<char>(cols, '0'));
        revealed.resize(rows, vector<bool>(cols, false));
        flagged.resize(rows, vector<bool>(cols, false));
        generateMines();
        calculateNumbers();
    }

    void play() {
        int x, y;
        char action;
        bool gameOver = false;
        bool firstMove = true;
        while (!gameOver) {
            printBoard();
            cout << "Enter row, column, and action (r to reveal, f to flag): ";
            cin >> x >> y >> action;

            if (!isValid(x, y)) {
                cout << "Invalid coordinates! Try again.\n";
                continue;
            }

            if (action == 'r') {
                if (firstMove) {
                    firstRow = x;
                    firstCol = y;
                    fill(board.begin(), board.end(), vector<char>(cols, '0'));
                    generateMines();
                    calculateNumbers();
                }
                firstMove = false;
                if (board[x][y] == '*') {
                    cout << "Game Over! You hit a mine.\n";
                    printBoard(true);  // Reveal all cells
                    gameOver = true;
                } else {
                    revealCell(x, y);
                    if (checkWin()) {
                        cout << "Congratulations! You win!\n";
                        printBoard(true);
                        gameOver = true;
                    }
                }
            } else if (action == 'f') {
                flagged[x][y] = !flagged[x][y];  // Toggle flag
            } else {
                cout << "Invalid action! Use 'r' to reveal or 'f' to flag.\n";
            }
        }
    }
};

int main() {
    int rows = 1, cols = 1, mines = 1;
    while(1) {
        cout << "Hello! Enter the number of rows, columns, and mines: ";
        cin >> rows >> cols >> mines;
        if(mines < 1) continue;
        if(rows < 4) continue;
        if(cols < 4) continue;
        if(mines > (rows * cols - 9)) continue;
        break;
    }

    Minesweeper game(rows, cols, mines);
    game.play();

    return 0;
}