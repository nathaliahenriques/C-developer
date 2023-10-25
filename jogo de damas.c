#include <stdio.h>
#include <stdbool.h>

#define BOARD_SIZE 8

// Definir peças
#define EMPTY ' '
#define WHITE 'W'
#define BLACK 'B'
#define WHITE_KING 'K'
#define BLACK_KING 'Q'

// Estrutura para representar uma peça no tabuleiro
typedef struct {
    char type;
    bool isKing;
} Piece;

// Função para inicializar o tabuleiro
void initialize_board(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 != 0) {
                if (i < 3) {
                    board[i][j].type = BLACK;
                } else if (i > 4) {
                    board[i][j].type = WHITE;
                } else {
                    board[i][j].type = EMPTY;
                }
                board[i][j].isKing = false;
            } else {
                board[i][j].type = EMPTY;
                board[i][j].isKing = false;
            }
        }
    }
}

// Função para exibir o tabuleiro
void display_board(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j].type);
        }
        printf("\n");
    }
}

// Função para validar movimentos
bool is_valid_move(Piece board[BOARD_SIZE][BOARD_SIZE], int fromRow, int fromCol, int toRow, int toCol, char currentPlayer) {
    // Verificar se a posição de destino está dentro dos limites do tabuleiro
    if (toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE) {
        return false;
    }

    // Verificar se a posição de destino está vazia
    if (board[toRow][toCol].type != EMPTY) {
        return false;
    }

    // Verificar se o movimento é diagonal
    if (abs(fromRow - toRow) != abs(fromCol - toCol)) {
        return false;
    }

    // Verificar se o movimento é válido para peças brancas
    if (currentPlayer == WHITE) {
        if (fromRow >= toRow) {
            return false; // Peças brancas só podem mover para cima
        }
    }
    // Verificar se o movimento é válido para peças pretas
    else if (currentPlayer == BLACK) {
        if (fromRow <= toRow) {
            return false; // Peças pretas só podem mover para baixo
        }
    }

    // Verificar se o movimento é uma captura válida
    int diffRow = toRow - fromRow;
    int diffCol = toCol - fromCol;
    int captureRow = fromRow + (diffRow / 2);
    int captureCol = fromCol + (diffCol / 2);

    // Verificar se há uma peça adversária no meio (capturando)
    if (board[captureRow][captureCol].type == (currentPlayer == WHITE ? BLACK : WHITE)) {
        return true; // É uma captura válida
    }

    // Se nenhuma condição de movimento inválido ou captura for atendida, o movimento é válido
    return false;
}

// Função para executar um movimento
void make_move(Piece board[BOARD_SIZE][BOARD_SIZE], int fromRow, int fromCol, int toRow, int toCol) {
    // Mova a peça da posição de origem para a posição de destino
    board[toRow][toCol] = board[fromRow][fromCol];
    
    // Limpe a posição de origem
    board[fromRow][fromCol].type = EMPTY;
    
    // Verifique se a peça se tornou uma "dama" (promovida) e atualize a flag isKing, se necessário
    if (toRow == 0 || toRow == BOARD_SIZE - 1) {
        board[toRow][toCol].isKing = true;
    }
    
    // Verifique se um movimento de captura ocorreu
    int diffRow = toRow - fromRow;
    int diffCol = toCol - fromCol;
    int captureRow = fromRow + (diffRow / 2);
    int captureCol = fromCol + (diffCol / 2);

    if (abs(diffRow) == 2 && abs(diffCol) == 2) {
        // Um movimento de captura ocorreu
        // Remova a peça capturada do tabuleiro
        board[captureRow][captureCol].type = EMPTY;
        board[captureRow][captureCol].isKing = false; // A peça capturada não é mais uma dama
    }
}

int main() {
    Piece board[BOARD_SIZE][BOARD_SIZE];
    char currentPlayer = WHITE;

    initialize_board(board);

    int fromRow, fromCol, toRow, toCol;

    while (1) {
        display_board(board);

        // Solicitar entrada do jogador atual
        printf("Jogador %c, faça o seu movimento (linha coluna para linha coluna): ", currentPlayer);
        scanf("%d %d %d %d", &fromRow, &fromCol, &toRow, &toCol);

        // Verificar a validade do movimento
        if (is_valid_move(board, fromRow, fromCol, toRow, toCol, currentPlayer)) {
            // Executar o movimento
            make_move(board, fromRow, fromCol, toRow, toCol);

            // Trocar o jogador atual
            if (currentPlayer == WHITE) {
                currentPlayer = BLACK;
            } else {
                currentPlayer = WHITE;
            }

            // Verificar se o jogo terminou (vencedor ou empate)
            if (is_game_over(board, currentPlayer)) {
                display_board(board);
                printf("O jogo acabou. Jogador %c venceu!\n", (currentPlayer == WHITE) ? 'B' : 'W');
                break; // Encerre o loop do jogo
            }
        } else {
            printf("Movimento inválido. Tente novamente.\n");
        }
    }

    return 0;
}

// Função para verificar se o jogo terminou (vitória ou empate)
bool is_game_over(Piece board[BOARD_SIZE][BOARD_SIZE], char currentPlayer) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].type == currentPlayer) {
                for (int di = -1; di <= 1; di += 2) {
                    for (int dj = -1; dj <= 1; dj += 2) {
                        int toRow = i + di;
                        int toCol = j + dj;
                        
                        if (is_valid_move(board, i, j, toRow, toCol, currentPlayer)) {
                            return false; // O jogador ainda pode fazer movimentos simples válidos
                        }
                        
                        if (board[i][j].isKing) {
                            // Se a peça atual é uma "dama," verifique movimentos diagonais em todas as direções
                            for (int mult = 1; mult <= BOARD_SIZE; mult++) {
                                toRow = i + di * mult;
                                toCol = j + dj * mult;
                                if (is_valid_move(board, i, j, toRow, toCol, currentPlayer)) {
                                    return false; // O jogador ainda pode fazer movimentos válidos com "damas"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true; // O jogador não pode fazer mais movimentos válidos
}