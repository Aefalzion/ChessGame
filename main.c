#include <stdio.h>
#include <conio.h>
#include "engine.h"
#include <stdlib.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void print_board() {
    int i, j;
    printf("\n");
    for (j = 7; j >= 0; j--) {
        printf("%d ", j + 1);
        for (i = 0; i < 8; i++) {
            if (game.board.pieces[i][j] == 0)
                printf("--");
            else {
                if (game.board.colors[i][j] == 1)
                    printf(   "w%d"   , game.board.pieces[i][j]);
                else
                    printf(   "b%d"   , game.board.pieces[i][j]);
            }
            printf(" ");
        }
        printf("\n\n");
    }
    printf("  ");
    for (i = 0; i <= 7; i++)
        printf("%c  ", i + 'a');
    printf("\n\n");
}

int main() {
    start_new_game();
    int x1, x2, y1, y2;
    char str[10];
    while (1) {
        print_board();
        scanf("%s", str);
        if (make_str_move(str)) {
            printf("1\n");
        } else {
            printf("0\n");
        }
        system("cls");
        if (is_mate()) {
            printf("Mate! ");
            if (game.move == 1)
                printf("Black wins!");
            else
                printf("White wins!");
            start_new_game();
        }
    }
    return 0;
}