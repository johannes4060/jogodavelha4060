#include <iostream>
#include <vector>

using namespace std;

// Definindo o tamanho do tabuleiro do jogo da velha
const int BOARD_SIZE = 3;

// Definindo os possíveis valores de uma célula no tabuleiro
enum CellValue { EMPTY, X, O };

// Definindo uma estrutura para representar uma posição no tabuleiro
struct Move {
    int row, col;
};

// Função para imprimir o tabuleiro do jogo da velha
void printBoard(vector<vector<CellValue>>& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY)
                cout << "-";
            else if (board[i][j] == X)
                cout << "X";
            else
                cout << "O";
            cout << " ";
        }
        cout << endl;
    }
}

// Função para verificar se um jogador venceu
bool isWinner(vector<vector<CellValue>>& board, CellValue player) {
    // Verificando linhas e colunas
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true; // Linha i completa
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return true; // Coluna i completa
    }
    // Verificando diagonais
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true; // Diagonal principal completa
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true; // Diagonal secundária completa
    return false;
}

// Função para verificar se o tabuleiro está cheio
bool isBoardFull(vector<vector<CellValue>>& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY)
                return false; // Encontrou uma célula vazia
        }
    }
    return true; // Tabuleiro cheio
}

// Função para avaliar o tabuleiro
int evaluateBoard(vector<vector<CellValue>>& board) {
    if (isWinner(board, X))
        return 10; // Jogador X venceu
    else if (isWinner(board, O))
        return -10; // Jogador O venceu
    else
        return 0; // Empate
}

// Função Minimax recursiva para encontrar a melhor jogada
int minimax(vector<vector<CellValue>>& board, int depth, bool isMaximizing) {
    int score = evaluateBoard(board); // Avaliando o estado atual do tabuleiro

    // Caso base: se houver um vencedor ou o tabuleiro estiver cheio
    if (score != 0)
        return score;

    // Se o tabuleiro estiver cheio, não há mais jogadas possíveis
    if (isBoardFull(board))
        return 0;

    // Se for a vez do jogador maximizar
    if (isMaximizing) {
        int bestScore = -1000;
        // Percorrendo todas as células do tabuleiro
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                // Verificando se a célula está vazia
                if (board[i][j] == EMPTY) {
                    // Fazendo uma jogada para o jogador maximizar
                    board[i][j] = X;
                    // Chamando a função Minimax recursivamente para a próxima jogada
                    bestScore = max(bestScore, minimax(board, depth + 1, !isMaximizing));
                    // Desfazendo a jogada
                    board[i][j] = EMPTY;
                }
            }
        }
        return bestScore;
    } else { // Se for a vez do jogador minimizar
        int bestScore = 1000;
        // Percorrendo todas as células do tabuleiro
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                // Verificando se a célula está vazia
                if (board[i][j] == EMPTY) {
                    // Fazendo uma jogada para o jogador minimizar
                    board[i][j] = O;
                    // Chamando a função Minimax recursivamente para a próxima jogada
                    bestScore = min(bestScore, minimax(board, depth + 1, !isMaximizing));
                    // Desfazendo a jogada
                    board[i][j] = EMPTY;
                }
            }
        }
        return bestScore;
    }
}

// Função para encontrar a melhor jogada usando o algoritmo Minimax
Move findBestMove(vector<vector<CellValue>>& board) {
    int bestScore = -1000; // Inicializando a melhor pontuação como um valor muito baixo
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;

    // Percorrendo todas as células do tabuleiro
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            // Verificando se a célula está vazia
            if (board[i][j] == EMPTY) {
                // Fazendo uma jogada para o jogador maximizar
                board[i][j] = X;
                // Chamando a função Minimax recursivamente para avaliar a jogada
                int moveScore = minimax(board, 0, false);
                // Desfazendo a jogada
                board[i][j] = EMPTY;
                // Atualizando a melhor jogada se a pontuação da jogada atual for melhor
                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    bestMove.row = i;
                    bestMove.col = j;
                }
            }
        }
    }

    return bestMove; // Retornando a melhor jogada encontrada
}

// Função para a jogada do jogador humano
void playerMove(vector<vector<CellValue>>& board) {
    int row, col;
    cout << "Sua vez (jogador O). Insira a linha e coluna (0-2): ";
    cin >> row >> col;
    // Verificando se a posição é válida e se está vazia
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == EMPTY) {
        board[row][col] = O;
    } else {
        cout << "Jogada inválida. Tente novamente.\n";
        playerMove(board); // Pedindo outra jogada ao jogador
    }
}

int main() {
    vector<vector<CellValue>> board(BOARD_SIZE, vector<CellValue>(BOARD_SIZE, EMPTY)); // Inicializando o tabuleiro vazio

    // Loop do jogo
    while (true) {
        cout << "Tabuleiro atual:\n";
        printBoard(board); // Imprimindo o tabuleiro atual

        // Verificando se o jogador X ganhou
        if (isWinner(board, X)) {
            cout << "Você perdeu! Jogador X venceu.\n";
            break;
        }
        // Verificando se o jogador O ganhou
        else if (isWinner(board, O)) {
            cout << "Parabéns! Você venceu.\n";
            break;
        }
        // Verificando se o jogo empatou
        else if (isBoardFull(board)) {
            cout << "Empate!\n";
            break;
        }

        playerMove(board); // Jogador humano faz sua jogada

        cout << "Tabuleiro atual:\n";
        printBoard(board); // Imprimindo o tabuleiro atual

        // Verificando se o jogo terminou após a jogada do jogador humano
        if (isWinner(board, O) || isBoardFull(board))
            break;

        cout << "Vez da máquina (jogador X)...\n";
        Move bestMove = findBestMove(board); // Encontrando a melhor jogada para a máquina
        board[bestMove.row][bestMove.col] = X; // Fazendo a jogada da máquina

        // Verificando se o jogo terminou após a jogada da máquina
        if (isWinner(board, X) || isBoardFull(board))
            break;
    }

    return 0;
}

