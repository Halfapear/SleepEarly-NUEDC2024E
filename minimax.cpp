#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const char HUMAN = 'O';
const char AI = 'X';
const char EMPTY = ' ';

struct Move {
    int row, col;
};

// 检查当前玩家是否获胜
bool isWinner(const vector<vector<char>>& board, char player) {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
            return true;
        }
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    } 
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }
    return false;
}

// 评估当前棋盘得分
int evaluate(const vector<vector<char>>& board) {
    if (isWinner(board, AI)) {
        return 10;
    }
    if (isWinner(board, HUMAN)) {
        return -10;
    }
    return 0;
}

// 检查是否还有空位置
bool isMovesLeft(const vector<vector<char>>& board) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == EMPTY) {
                return true;
            }
        }
    }
    return false;
}

// Minimax算法
int minimax(vector<vector<char>>& board, int depth, bool isMaximizing) {
    int score = evaluate(board);

    if (score == 10) {
        return score - depth;
    }

    if (score == -10) {
        return score + depth;
    }

    if (!isMovesLeft(board)) {
        return 0;
    }

    if (isMaximizing) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = AI;
                    best = max(best, minimax(board, depth + 1, !isMaximizing));
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = HUMAN;
                    best = min(best, minimax(board, depth + 1, !isMaximizing));
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    }
}

// 找到最佳步
Move findBestMove(vector<vector<char>>& board) {
    int bestVal = numeric_limits<int>::min();
    Move bestMove = {-1, -1};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == EMPTY) {
                board[i][j] = AI;
                int moveVal = minimax(board, 0, false);
                board[i][j] = EMPTY;
                if (moveVal > bestVal) {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    return bestMove;
}

// 打印棋盘
void printBoard(const vector<vector<char>>& board) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << board[i][j];
            if (j < 2) cout << " | ";
        }
        cout << endl;
        if (i < 2) cout << "---------" << endl;
    }
}

int main() {
    vector<vector<char>> board = {
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };

    char humanPlayer, aiPlayer;
    cout << "Choose your player (X for AI or O for Human): ";
    cin >> humanPlayer;

    if (humanPlayer == 'X' || humanPlayer == 'x') {
        humanPlayer = AI;
        aiPlayer = HUMAN;
    } else {
        humanPlayer = HUMAN;
        aiPlayer = AI;
    }

    printBoard(board);

    bool isHumanTurn = (humanPlayer == HUMAN);

    while (isMovesLeft(board)) {
        if (isHumanTurn) {
            int row, col;
            cout << "Enter your move (row and column): ";
            cin >> row >> col;
            if (board[row][col] == EMPTY) {
                board[row][col] = humanPlayer;
                printBoard(board);
                if (isWinner(board, humanPlayer)) {
                    cout << "You win!" << endl;
                    break;
                }
                isHumanTurn = false;
            } else {
                cout << "Invalid move. Try again." << endl;
            }
        } else {
            cout << "AI is making a move..." << endl;
            Move bestMove = findBestMove(board);
            board[bestMove.row][bestMove.col] = aiPlayer;
            printBoard(board);
            if (isWinner(board, aiPlayer)) {
                cout << "AI wins!" << endl;
                break;
            }
            isHumanTurn = true;
        }

        if (!isMovesLeft(board) && !isWinner(board, aiPlayer) && !isWinner(board, humanPlayer)) {
            cout << "It's a draw!" << endl;
            break;
        }
    }

    return 0;
}
