#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define SIZE 8

#define TWO_CHANCE 2
#define THREE_CHANCE 2
#define FIVE_CHANCE 2
#define TOTAL_CHANCE 8

#define TRUE 1
#define FALSE 0
#define WIN_NUM 2048
#define NONE 0
#define LOSE -1
#define WIN 1
#define FULL 2
typedef struct _game {
	int board[SIZE][SIZE];
} game;

game moveLeft(game board);
game rotateClockwise(game board);
game newTile(game board);
game makeNewBoard(void);
game printBoard(game board);
game makeAction(game board, char move);
game evalPairs(game board);
int findWin(game board);
int anyNext(game board);

int main(int argc, char *argv[]) {
	srand(time(NULL));
	game board = makeNewBoard();
    board = printBoard(board);
    printf(">>> ");
    while (findWin(board) == NONE || findWin(board) == FULL) {
        char move = getchar();
        while (move == '\n') {
            printf(">>> ");
            move = getchar();
        }
		board = makeAction(board, move);
		board = printBoard(board);
        printf("\n");
	}

    board = printBoard(board);
    if (findWin(board) == WIN) {
        printf("YOU WIN!\n");
    } else {
        printf("GAME OVER\n");
    }
	return EXIT_SUCCESS;
}

game moveLeft(game board) {
	game newGame = board;
    int row = 0;
    while (row < SIZE) {
        int freeCol = 0;
        int col = 0;
        while (col < SIZE) {
            if (newGame.board[row][col] != 0) {
                if (freeCol != col) {
                    newGame.board[row][freeCol] = newGame.board[row][col];
                    newGame.board[row][col] = 0;
                }
                freeCol++;
            }
            col++;
        }
        row++;
    }
	return newGame;
}

game rotateClockwise(game board) {
	game newGame;
    int row = 0;
    while (row < SIZE) {
        int col = 0;
        while (col < SIZE) {
            newGame.board[col][SIZE-row-1] = board.board[row][col];
            col++;
        }
        row++;
    }

	return newGame;

}


game makeNewBoard(void){
	game newBoard;
	int startRow = 0;
	while (startRow < SIZE) {
		int startCol = 0;
		while (startCol < SIZE) {
			newBoard.board[startRow][startCol] = 0;
			startCol++;
		}
		startRow++;
	}
	
	newBoard = newTile(newBoard);
	newBoard = newTile(newBoard);

	return newBoard;
}

game printBoard(game board) {
	int row = 0;
	int col = 0;
	while (row < SIZE) {
		col = 0;
		while (col < SIZE) {
			if (board.board[row][col] == 0){
                printf(".     ");
            } else {
                char toPrint[15];
                if (board.board[row][col] > 2*WIN_NUM) {
                    printf("\e[32;1m");
                    board.board[row][col] -= 2*WIN_NUM;
                    sprintf(toPrint, "%d", board.board[row][col]);
                    printf(toPrint);
                    printf("\e[39;49m");
                } else {
                    sprintf(toPrint, "%d", board.board[row][col]);
                    printf(toPrint);
                }
                int counter = strlen(toPrint);
                while (counter < 6) {
                    printf(" ");
                    counter++;
                }
            }
            col++;
		}
		printf("\n");
		row++;
	}
    return board;
}

game makeAction(game board, char move) {
	game newGame = board;
    if (move == 'a') {
        newGame = moveLeft(newGame);
        newGame = evalPairs(newGame);
        newGame = moveLeft(newGame);
        newGame = newTile(newGame);
    } else if (move == 's') {
        newGame = rotateClockwise(newGame);
        newGame = moveLeft(newGame);
        newGame = evalPairs(newGame);
        newGame = moveLeft(newGame);
        newGame = rotateClockwise(newGame);
        newGame = rotateClockwise(newGame);
        newGame = rotateClockwise(newGame);
        newGame = newTile(newGame);
    } else if (move == 'd') {
        newGame = rotateClockwise(newGame);
        newGame = rotateClockwise(newGame);
        newGame = moveLeft(newGame);
        newGame = evalPairs(newGame);
        newGame = moveLeft(newGame);
        newGame = rotateClockwise(newGame);
        newGame = rotateClockwise(newGame);
        newGame = newTile(newGame);
    } else if (move == 'w') {
        newGame = rotateClockwise(newGame);
        newGame = rotateClockwise(newGame);
        newGame = rotateClockwise(newGame);
        newGame = moveLeft(newGame);
        newGame = evalPairs(newGame);
        newGame = moveLeft(newGame);
        newGame = rotateClockwise(newGame);
        newGame = newTile(newGame);
    } else {
        printf("INVALID MOVE\n");
    }
	return newGame;

}

int findWin(game board) {
	int isWin = LOSE;
	int row = 0;
	int col = 0;
	while (row < SIZE) {
		col = 0;
		while (col < SIZE) {
			if (board.board[row][col] >= WIN_NUM && board.board[row][col] < 2*WIN_NUM) {
                isWin = WIN;
            } else if (board.board[row][col] == 0 && isWin == LOSE){
				isWin = NONE;
			} 
			col++;
		}
		row++;
	}

    if (isWin == LOSE && anyNext(board) == TRUE) {
        isWin = FULL;
    }
	return isWin;
}

game newTile(game board) {
	game newGame = board;
	if (findWin(newGame) != FULL) {
        int pos = rand()%(SIZE*SIZE);
        int col = pos%SIZE;
        int row = pos/SIZE;
        while (newGame.board[row][col] != 0) {
		pos = rand()%(SIZE*SIZE);
		col = pos%SIZE;
		row = pos/SIZE;
        }

        int chance = rand()%TOTAL_CHANCE;
        if (chance < TWO_CHANCE) {
            newGame.board[row][col] = 2+2*WIN_NUM;
        } else if (chance < TWO_CHANCE + THREE_CHANCE){
            newGame.board[row][col] = 3+2*WIN_NUM;
        } else if (chance < TWO_CHANCE+THREE_CHANCE+FIVE_CHANCE) {
            newGame.board[row][col] = 5+2*WIN_NUM;
        } else {
            newGame.board[row][col] = 7+2*WIN_NUM;
        }
    }

	return newGame;
}

game evalPairs(game board) {
    game newGame = board;
    int row = 0;
    while (row < SIZE) {
        int col = 1;
        int offset = 0;
        while(col < SIZE) {
            if ((board.board[row][col] != 0 && board.board[row][col-1] != 0) && ((board.board[row][col] % board.board[row][col-1] == 0) || 
                 (board.board[row][col-1] % board.board[row][col] == 0))) {
                newGame.board[row][col-1-offset] = board.board[row][col]+board.board[row][col-1];
                newGame.board[row][col] = 0;
                if (offset != 0) {
                    newGame.board[row][col-1] = 0;
                }
                col++;
                offset++;
            }
            col++;
        }
        row++;
    }
    return newGame;
}

int anyNext(game board) {
    int next = FALSE;
    int row = 0;
    while (row < SIZE) {
        int col = 0;
        while (col < SIZE) {
            int toCheck = board.board[row][col];
            if ((col > 0 && toCheck == board.board[row][col-1]) || (col < SIZE-1 && toCheck == board.board[row][col+1]) || 
                (row > 0 && toCheck == board.board[row-1][col]) || (row < SIZE-1 && toCheck == board.board[row+1][col])) {
                next = TRUE;
            }
            col++;
        }
        row++;
    }
    return next;
}
