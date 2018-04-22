#ifndef CHESSGAME_ENGINE_H
#define CHESSGAME_ENGINE_H

typedef struct {
    int pieces[8][8]; //0-empty, 1-pawn, 2-knight, 3-bishop, 4-rook, 5-queen, 6-king
    int colors[8][8]; //1-white, 2-black
} ChessBoard;

typedef struct {
    ChessBoard board;
    int move; //1-white, 2-black
    int white000, white00, black000, black00; //are castlings possible
    int en_passant; //возможно ли взятие на проходе
    int en_passant_x;
} ChessGame;

typedef struct {
    int x, y;
} ChessPosition;

ChessGame game;

void fill_board() {
    int i, j;
    for (i = 0; i < 8; i++)
        for(j = 0; j < 8; j++) {
            game.board.pieces[i][j] = 0;
            game.board.colors[i][j] = 0;
        }
    game.board.pieces[0][0] = 4;
    game.board.pieces[1][0] = 2;
    game.board.pieces[2][0] = 3;
    game.board.pieces[3][0] = 5;
    game.board.pieces[4][0] = 6;
    game.board.pieces[5][0] = 3;
    game.board.pieces[6][0] = 2;
    game.board.pieces[7][0] = 4;
    game.board.pieces[0][7] = 4;
    game.board.pieces[1][7] = 2;
    game.board.pieces[2][7] = 3;
    game.board.pieces[3][7] = 5;
    game.board.pieces[4][7] = 6;
    game.board.pieces[5][7] = 3;
    game.board.pieces[6][7] = 2;
    game.board.pieces[7][7] = 4;
    for (i = 0; i < 8; i++) {
        game.board.pieces[i][1] = 1;
        game.board.pieces[i][6] = 1;
    }
    for (i = 0; i < 8; i++) {
        game.board.colors[i][0] = 1;
        game.board.colors[i][1] = 1;
        game.board.colors[i][6] = 2;
        game.board.colors[i][7] = 2;
    }
}

void start_new_game() {
    fill_board();
    game.move = 1;
    game.white000 = 1;
    game.white00 = 1;
    game.black000 = 1;
    game.black00 = 1;
    game.en_passant = 0;
}

int can_reach(int x1, int y1, int x2, int y2, int dx, int dy) {  //checks if a piece can reach (x2, y2) moving along a line
    if (x1 == x2 && y1 == y2)
        return 0;
    int tx = x1;
    int ty = y1;
    tx += dx;
    ty += dy;
    while (tx >= 0 && tx <= 7 && ty >= 0 && ty <= 7 && game.board.pieces[tx][ty] == 0) {
        if (tx == x2 && ty == y2)
            return 1;
        tx += dx;
        ty += dy;
    }
    if (tx == x2 && ty == y2)
        return 1;
    return 0;
}

int can_move(int x1, int y1, int x2, int y2) { //checks if a piece at (x1, y1) can move to (x2, y2)
    if (x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0 || x1 > 7 || x1 < 0 || y1 > 7 || y1 < 0)
        return 0;
    if (x2 == x1 && y2 == y1)
        return 0;
    int color1 = game.board.colors[x1][y1];
    int color2 = game.board.colors[x2][y2];
    if (color1 == 0)
        return 0;
    if (color1 == color2)
        return 0;
    int deltax = x2 - x1;
    int deltay = y2 - y1;
    switch (game.board.pieces[x1][y1]) {
        case 0:
            return 0;
        case 1:
            if (color1 == 1) {
                if (x2 == x1 && y2 == y1 + 1 && game.board.pieces[x2][y2] == 0)
                    return 1;
                if (x2 == x1 + 1 && y2 == y1 + 1 && game.board.pieces[x2][y2] != 0)
                    return 1;
                if (x2 == x1 - 1 && y2 == y1 + 1 && game.board.pieces[x2][y2] != 0)
                    return 1;
                if (x2 == x1 && y1 == 1 && y2 == 3 && game.board.pieces[x1][2] == 0 && game.board.pieces[x1][3] == 0)
                    return 1;
            }
            if (color1 == 2) {
                if (x2 == x1 && y2 == y1 - 1 && game.board.pieces[x2][y2] == 0)
                    return 1;
                if (x2 == x1 + 1 && y2 == y1 - 1 && game.board.pieces[x2][y2] != 0)
                    return 1;
                if (x2 == x1 - 1 && y2 == y1 - 1 && game.board.pieces[x2][y2] != 0)
                    return 1;
                if (x2 == x1 && y1 == 6 && y2 == 4 && game.board.pieces[x1][5] == 0 && game.board.pieces[x1][4] == 0)
                    return 1;
            }
            return 0;
        case 2:
            if ((deltax * deltax == 4 && deltay * deltay == 1) || (deltax * deltax == 1 && deltay * deltay == 4))
                return 1;
            return 0;
        case 3:
            if (can_reach(x1, y1, x2, y2, 1, 1) || can_reach(x1, y1, x2, y2, 1, -1) || can_reach(x1, y1, x2, y2,-1, 1) || can_reach(x1, y1, x2, y2, -1, -1))
                return 1;
            return 0;
        case 4:
            if (can_reach(x1, y1, x2, y2, 1, 0) || can_reach(x1, y1, x2, y2, -1, 0) || can_reach(x1, y1, x2, y2, 0, 1) || can_reach(x1, y1, x2, y2, 0, -1))
                return 1;
            return 0;
        case 5:
            if (can_reach(x1, y1, x2, y2, 1, 1) || can_reach(x1, y1, x2, y2, 1, -1) || can_reach(x1, y1, x2, y2,-1, 1) || can_reach(x1, y1, x2, y2, -1, -1))
                return 1;
            if (can_reach(x1, y1, x2, y2, 1, 0) || can_reach(x1, y1, x2, y2, -1, 0) || can_reach(x1, y1, x2, y2, 0, 1) || can_reach(x1, y1, x2, y2, 0, -1))
                return 1;
            return 0;
        case 6:
            if (deltax >= -1 && deltax <= 1 && deltay >= -1 && deltay <= 1)
                return 1;
            return 0;
    }
    return 0;
}

int is_check(int color) { //checks if a king of color colour is in check, 1 - yes 0 - no
    int i, j, xk = -1, yk = -1;
    for (i = 0; i <= 7; i++)
        for (j = 0; j <= 7; j++)
            if (game.board.pieces[i][j] == 6 && game.board.colors[i][j] == color) {
                xk = i;
                yk = j;
            }
    for (i = 0; i <= 7; i++)
        for (j = 0; j <= 7; j++)
            if (can_move(i, j, xk, yk))
                return 1;
    return 0;
}

int is_beaten(int x1, int y1, int color) { //checks if a position is under attack of pieces of specific color
    int i, j;
    for (i = 0; i <= 7; i++)
        for (j = 0; j <= 7; j++)
            if (game.board.pieces[x1][y1] == color && can_move(i, j, x1, y1))
                return 1;
    return 0;
}

int do_white_long_castle_if_possible() {
    if (!game.white000)
        return 0;
    if (is_beaten(4, 0, 2) || is_beaten(3, 0, 2) || is_beaten(2, 0, 2))
        return 0;
    if (game.board.pieces[0][0] == 4 && game.board.pieces[1][0] == 0 && game.board.pieces[2][0] == 0 && game.board.pieces[3][0] == 0 &&
            game.board.pieces[4][0] == 6 && game.board.colors[0][0] == 1 && game.board.colors[4][0] == 1) {
        game.board.pieces[2][0] = 6;
        game.board.pieces[3][0] = 4;
        game.board.colors[2][0] = 1;
        game.board.colors[3][0] = 1;
        game.board.pieces[0][0] = 0;
        game.board.pieces[4][0] = 0;
        return 1;
    }
    return 0;
}

int do_white_short_castle_if_possible() {
    if (!game.white00)
        return 0;
    if (is_beaten(4, 0, 2) || is_beaten(5, 0, 2) || is_beaten(6, 0, 2))
        return 0;
    if (game.board.pieces[7][0] == 4 && game.board.pieces[6][0] == 0 && game.board.pieces[5][0] == 0 &&
        game.board.pieces[4][0] == 6 && game.board.colors[7][0] == 1 && game.board.colors[4][0] == 1) {
        game.board.pieces[6][0] = 6;
        game.board.pieces[5][0] = 4;
        game.board.colors[6][0] = 1;
        game.board.colors[5][0] = 1;
        game.board.pieces[7][0] = 0;
        game.board.pieces[4][0] = 0;
        return 1;
    }
    return 0;
}

int do_black_long_castle_if_possible() {
    if (!game.black000)
        return 0;
    if (is_beaten(5, 7, 1) || is_beaten(4, 7, 1) || is_beaten(3, 7, 1))
        return 0;
    if (game.board.pieces[0][7] == 4 && game.board.pieces[1][7] == 0 && game.board.pieces[2][7] == 0 && game.board.pieces[3][7] == 0 &&
        game.board.pieces[4][7] == 6 && game.board.colors[0][7] == 2 && game.board.colors[4][7] == 2) {
        game.board.pieces[2][7] = 6;
        game.board.pieces[3][7] = 4;
        game.board.colors[2][7] = 2;
        game.board.colors[3][7] = 2;
        game.board.pieces[0][7] = 0;
        game.board.pieces[4][7] = 0;
        return 1;
    }
    return 0;
}

int do_black_short_castle_if_possible() {
    if (!game.black00)
        return 0;
    if (is_beaten(4, 7, 1) || is_beaten(5, 7, 1) || is_beaten(6, 7, 1))
        return 0;
    if (game.board.pieces[7][7] == 4 && game.board.pieces[6][7] == 0 && game.board.pieces[5][7] == 0 &&
        game.board.pieces[4][7] == 6 && game.board.colors[7][7] == 2 && game.board.colors[4][7] == 2) {
        game.board.pieces[6][7] = 6;
        game.board.pieces[5][7] = 4;
        game.board.colors[6][7] = 2;
        game.board.colors[5][7] = 2;
        game.board.pieces[7][7] = 0;
        game.board.pieces[4][7] = 0;
        return 1;
    }
    return 0;
}

int do_en_passant_if_possible(int x1, int y1, int x2, int y2) {
    int backup_pieces[8][8], backup_colors[8][8];
    int i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
            backup_pieces[i][j] = game.board.pieces[i][j];
            backup_colors[i][j] = game.board.colors[i][j];
        }

    if (!game.en_passant)
        return 0;
    if (game.en_passant_x != x2)
        return 0;

    if (game.board.pieces[x1][y1] != 1)
        return 0;
    if (game.board.colors[x1][y1] != game.move)
        return 0;
    if ((x1 - x2) * (x1 - x2) != 1)
        return 0;
    if (game.move == 1) {
        if (y2 - y1 != 1)
            return 0;
        if (y1 != 4)
            return 0;
        game.board.pieces[x1][y1] = 0;
        game.board.pieces[x2][y2] = 1;
        game.board.colors[x2][y2] = 1;
        game.board.pieces[x2][y2 - 1] = 0;

        if (is_check(game.move)) {
            for (i = 0; i < 8; i++)
                for (j = 0; j < 8; j++) {
                    game.board.pieces[i][j] = backup_pieces[i][j];
                    game.board.colors[i][j] = backup_colors[i][j];
                }
            return 0;
            } else {
            return 1;
        }
    }

    if (game.move == 2) {
        if (y2 - y1 != -1)
            return 0;
        if (y1 != 3)
            return 0;
        game.board.pieces[x1][y1] = 0;
        game.board.pieces[x2][y2] = 1;
        game.board.colors[x2][y2] = 2;
        game.board.pieces[x2][y2 + 1] = 0;

        if (is_check(game.move)) {
            for (i = 0; i < 8; i++)
                for (j = 0; j < 8; j++) {
                    game.board.pieces[i][j] = backup_pieces[i][j];
                    game.board.colors[i][j] = backup_colors[i][j];
                }
            return 0;
        } else {
            return 1;
        }
    }
    return 0;
}

int make_move(int x1, int y1, int x2, int y2) { //returns 1 if move is possible and done and 0 if move isn't possible
    //check for castles
    if (x1 == 4 && y1 == 0 && x2 == 2 && y2 == 0 && game.board.pieces[x1][y1] == 6 && game.board.colors[x1][y1] == game.move) {
        if (do_white_long_castle_if_possible()) {
            game.move = 3 - game.move;
            game.en_passant = 0;
            return 1;
        }
        return 0;
    }

    if (x1 == 4 && y1 == 0 && x2 == 6 && y2 == 0 && game.board.pieces[x1][y1] == 6 && game.board.colors[x1][y1] == game.move) {
        if (do_white_short_castle_if_possible()) {
            game.move = 3 - game.move;
            game.en_passant = 0;
            return 1;
        }
        return 0;
    }

    if (x1 == 4 && y1 == 7 && x2 == 2 && y2 == 7 && game.board.pieces[x1][y1] == 6 && game.board.colors[x1][y1] == game.move) {
        if (do_black_long_castle_if_possible()) {
            game.move = 3 - game.move;
            game.en_passant = 0;
            return 1;
        }
        return 0;
    }

    if (x1 == 4 && y1 == 7 && x2 == 6 && y2 == 7 && game.board.pieces[x1][y1] == 6 && game.board.colors[x1][y1] == game.move) {
        if (do_black_short_castle_if_possible()) {
            game.move = 3 - game.move;
            game.en_passant = 0;
            return 1;
        }
        return 0;
    }
    //проверка на взятие на проходе
    if (game.board.pieces[x1][y1] == 1 && x2 != x1 && game.board.pieces[x2][y2] == 0) {
        if (do_en_passant_if_possible(x1, y1, x2, y2)) {
            game.move = 3 - game.move;
            game.en_passant = 0;
            return 1;
        }
        return 0;
    }


    int backup_pieces[8][8], backup_colors[8][8];
    int i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
            backup_pieces[i][j] = game.board.pieces[i][j];
            backup_colors[i][j] = game.board.colors[i][j];
        }
    if (game.board.colors[x1][y1] == game.move && can_move(x1, y1, x2, y2)) {
        game.board.pieces[x2][y2] = game.board.pieces[x1][y1];
        game.board.colors[x2][y2] = game.board.colors[x1][y1];
        game.board.pieces[x1][y1] = 0;
        game.board.colors[x1][y1] = 0;
        if (is_check(game.move)) {
            for (i = 0; i < 8; i++)
                for (j = 0; j < 8; j++) {
                    game.board.pieces[i][j] = backup_pieces[i][j];
                    game.board.colors[i][j] = backup_colors[i][j];
                }
                return 0;
        } else {
            game.move = 3 - game.move;
            if (x1 == 4 && y1 == 0) {
                game.white000 = 0;
                game.white00 = 0;
            }
            if (x1 == 4 && y1 == 7) {
                game.black000 = 0;
                game.black00 = 0;
            }
            if ((x1 == 0 && y1 == 0) || (x2 == 0 && y2 == 0))
                game.white000 = 0;
            if ((x1 == 7 && y1 == 0) || (x2 == 7 && y2 == 0))
                game.white00 = 0;
            if ((x1 == 0 && y1 == 7) || (x2 == 0 && y2 == 7))
                game.black000 = 0;
            if ((x1 == 7 && y1 == 7) || (x2 == 7 && y2 == 7))
                game.black00 = 0;

            if (x1 == x2 && game.board.pieces[x2][y2] == 1 && (y2 - y1) * (y2 - y1) == 4) {
                game.en_passant = 1;
                game.en_passant_x = x1;
            }
            //проверка проведенной пешки
            if (game.board.pieces[x2][y2] == 1 && (y2 == 7 || y2 == 0)) {
                printf("2 - knight, 3 - bishop, 4 - rook, 5 - queen");
                long int answer = 0;
                while (!(answer <= 5 && answer >= 2)) {
                    printf("\nPick the piece: ");
                    scanf("%d", &answer);
                }
                game.board.pieces[x2][y2] = answer;
            }
            return 1;
        }
    } else return 0;
}

int make_str_move(char* str) {
    int x1, y1, x2, y2;
    x1 = str[0] - 'a';
    y1 = str[1] - '1';
    x2 = str[3] - 'a';
    y2 = str[4] - '1';
    return make_move(x1, y1, x2, y2);
}

int is_move_possible(int x1, int y1, int x2, int y2) {
    if (!can_move(x1, y1, x2, y2))
        return 0;
    if (game.board.colors[x1][y1] != game.move)
        return 0;
    int backup_pieces[8][8], backup_colors[8][8];
    int i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
            backup_pieces[i][j] = game.board.pieces[i][j];
            backup_colors[i][j] = game.board.colors[i][j];
        }
    game.board.pieces[x2][y2] = game.board.pieces[x1][y1];
    game.board.colors[x2][y2] = game.board.colors[x1][y1];
    game.board.pieces[x1][y1] = 0;
    game.board.colors[x1][y1] = 0;
    int res = is_check(game.move);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            game.board.pieces[i][j] = backup_pieces[i][j];
            game.board.colors[i][j] = backup_colors[i][j];
        }
    }
    return !res;
}

int is_mate() {
    int color = game.move;
    if (is_check(color)) {
        int x1, y1, x2, y2;
        for (x1 = 0; x1 < 8; x1++)
            for (y1 = 0; y1 < 8; y1++)
                for (x2 = 0; x2 < 8; x2++)
                    for (y2 = 0; y2 < 8; y2++)
                        if (is_move_possible(x1, y1, x2, y2))
                            return 0;
        return 1;
    }
    return 0;
}
#endif //CHESSGAME_ENGINE_H
