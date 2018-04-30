#include <stdio.h>
#include "engine.h"
#include "connection.h"
#include <stdlib.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void print_board_1() {
    system("clear");
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
    if (game.move == my_color)
        printf("Your turn!\n");
    else
        printf("Waiting for opponent...\n");
}

void print_board_2() {
    system("clear");
    int i, j;
    printf("\n");
    for (j = 0; j <= 7; j++) {
        printf("%d ", j + 1);
        for (i = 7; i >= 0; i--) {
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
    for (i = 7; i >= 0; i--)
        printf("%c  ", i + 'a');
    printf("\n\n");
    if (game.move == my_color)
        printf("Your turn!\n");
    else
        printf("Waiting for opponent...\n");
}

int main() {
    my_connect();
    start_new_game();

    char str[1000];

    if (connection_type == 1) {
        printf("Select your colour(b/w): ");
        scanf("%s", str);
        if (str[0] == 'b') {
            my_color = 2;
        } else my_color = 1;
        if (my_color == 1) {
            my_send("2");
        } else my_send("1");
        start_new_game();
    }

    if (connection_type == 2) {
        my_receive();
        if (buffer[0] == '1')
            my_color = 1;
        else
            my_color = 2;
        start_new_game();
    }

    int x1, x2, y1, y2;
    while (1) {
        if (my_color == 1)
            print_board_1();
        else
            print_board_2();
        if (game.move == my_color) {
            scanf("%s", str);
            make_str_move(str);
            make_position();
            my_send(position);
        } else {
            my_receive();
            read_position(buffer);
        }

        if (is_mate()) {
            printf("Mate! ");
            if (game.move == 1)
                printf("Black wins!");
            else
                printf("White wins!");
            return 0;
        }
        if (is_stalemate()) {
            printf("Stalemate!");
            return 0;
        }
    }
    return 0;
}