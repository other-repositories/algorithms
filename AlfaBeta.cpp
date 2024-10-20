#include <iostream>
#include <vector>
#include <limits.h>

class Position {
public:
    std::vector<std::vector<char>> board; 
    char currentPlayer;

    Position(const std::vector<std::vector<char>> & initialBoard,unsigned char new_player = 0xff) {
        board = initialBoard;
        currentPlayer = (new_player != 0xff ? new_player : determineNextPlayer());
    }

    bool CheckWin(char symbol) const {
        return 
            (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) ||
            (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) ||
            (board[0][0] == symbol && board[0][1] == symbol && board[0][2] == symbol) ||
            (board[1][0] == symbol && board[1][1] == symbol && board[1][2] == symbol) ||
            (board[2][0] == symbol && board[2][1] == symbol && board[2][2] == symbol) ||
            (board[0][0] == symbol && board[1][0] == symbol && board[2][0] == symbol) ||
            (board[0][1] == symbol && board[1][1] == symbol && board[2][1] == symbol) ||
            (board[0][2] == symbol && board[1][2] == symbol && board[2][2] == symbol);
    }

    char determineNextPlayer() {
        int xCount = 0, oCount = 0;
        for (const auto& row : board) {
            for (char cell : row) {
                if (cell == 'X') xCount++;
                else if (cell == 'O') oCount++;
            }
        }
        return (xCount == oCount) ? 'X' : 'O';
    }

    int evaluate() const {

        auto score = CountMoves();

        if (CheckWin('X'))
            return score;
        if (CheckWin('O'))
            return -score;
        
        return 0;
    }

    std::vector<Position> getChildren() const {
        std::vector<Position> children;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (board[row][col] == '-') { // Empty cell
                    std::vector<std::vector<char>> newBoard = board;
                    newBoard[row][col] = currentPlayer; 
                    char nextPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                    children.push_back(Position(newBoard, nextPlayer));
                }
            }
        }
        return children;
    }

    int CountMoves() const {
        int count = 0;
        for (const auto& row : board) {
            for (char cell : row) {
                if (cell == '-') count++; 
            }
        }
        return count;
    }

    bool isGameOver() const {
        if (evaluate() != 0) return true; 
        if (CountMoves())
            return false;
        return true;
    }

    void display() const {
        for (const auto& row : board) {
            for (char cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

std::pair<int, std::vector<Position>> minimax(Position position, int depth, int alpha, int beta, bool maximizingPlayer) {
    std::vector<Position> bestPath;

    if (depth == 0 || position.isGameOver()) {
        return { position.evaluate(), {position} }; 
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        std::vector<Position> bestChildPath;

        for (Position child : position.getChildren()) {
            auto [eval, childPath] = minimax(child, depth - 1, alpha, beta, false);
            if (eval > maxEval) {
                maxEval = eval;
                bestChildPath = childPath; 
            }
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;  
            }
        }
        bestChildPath.insert(bestChildPath.begin(), position);
        return { maxEval, bestChildPath };
    }
    else {
        int minEval = INT_MAX;
        std::vector<Position> bestChildPath;

        for (Position child : position.getChildren()) {
            auto [eval, childPath] = minimax(child, depth - 1, alpha, beta, true);
            if (eval < minEval) {
                minEval = eval;
                bestChildPath = childPath;
            }
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        bestChildPath.insert(bestChildPath.begin(), position);  
        return { minEval, bestChildPath };
    }
}

auto ProcessAlfaBeta(const Position & position) {
    return minimax(position, 9, INT_MIN, INT_MAX, true);
}

int main() {
    auto [result, bestPath] = ProcessAlfaBeta({
        {
            {'-', '-', '-'},
            {'-', '-', '-'},
            {'-', '-', '-'}
        }
    });

    std::cout << "Best evaluation: " << result << std::endl << std::endl;
    std::cout << "Best sequence of moves:\n";

    for (const auto& pos : bestPath) {
        pos.display();
    }

    return 0;
}
