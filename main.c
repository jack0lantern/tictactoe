#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 30
#define BOARD_SIZE 5 //for a 3x3 only

const int SIDE_LEN = BOARD_SIZE/2 + 1;

void trim_newline(char *str){ //abdc\n\0 --> abdc\0\0
//    printf("%s", str);
    while((*str != '\n')&(*str != '\0'))
       str += 1;
    *str = '\0';
}

void getplayername(char *player, int pnum){
    printf("Ok player %d, enter your name.\n", pnum);
    fgets(player, MAX_NAME_LEN, stdin);
    trim_newline(player);
    printf("Hello %s!\n", player);
}

void initialize_board(char board[BOARD_SIZE][BOARD_SIZE + 1]){
    int r = 0;
    int c = 0;

    for(r = 0; r < BOARD_SIZE; r++){
        for(c = 0; c < BOARD_SIZE; c++){
            if(r % 2 == 1 && c % 2 == 1)
                board[r][c] = '+';
            else if (r % 2 == 1)
                board[r][c] = '-';
            else if(c % 2 == 1)
                board[r][c] = '|';
            else
                board[r][c] = '1' + SIDE_LEN*r/2 + c/2;
            //printf("%c", board[r][c]);
        }
        board[r][BOARD_SIZE] = '\n';
        //board[r][BOARD_SIZE + 1] = '\0';
        //printf("%c", board[r][c]);
    }
    board[r - 1][BOARD_SIZE] = '\0';
}

void initialize_board_record(int board_rec[SIDE_LEN][SIDE_LEN]) {
    int r;
    int c;
    for(r = 0; r < SIDE_LEN; r++)
        for(c = 0; c < SIDE_LEN; c++)
            board_rec[r][c] = 0;
}

int check_win(int board_rec[SIDE_LEN][SIDE_LEN]) {
    int r;
    int c;
    int init;
    int test;
    for(r = 0; r < SIDE_LEN; r++) { // check horiz's
        init = board_rec[r][0];
        test = board_rec[r][0];
        for(c = 0; c < SIDE_LEN; c++) {
            test = ~((test == board_rec[r][c]) - 1) & board_rec[r][c];
        }   // test should be the init value if matches all down, 0 ow
        if((init == test) && test)
            return test;
    }

    //printf("first check: passed horiz");
    for(c = 0; c < SIDE_LEN; c++) { // check vert's
        init = board_rec[0][c];
        test = board_rec[0][c];
        for(r = 0; r < SIDE_LEN; r++) {
            test = ~((test == board_rec[r][c]) - 1) & board_rec[r][c];
        }
        if((init == test) && test)
            return test;
    }

    //printf("second check: passed verts");
    init = board_rec[0][0];
    test = board_rec[0][0];
    for(r = 0; r < SIDE_LEN; r++) // check \\ diag
        test = ~((test == board_rec[r][r]) - 1) & board_rec[r][r];

    if((init == test) && test)
            return test;

    //printf("third check: passed \\ diags");
    init = board_rec[0][SIDE_LEN - 1];
    test = board_rec[0][SIDE_LEN - 1];
    for(r = 0, c = SIDE_LEN - 1; r < SIDE_LEN; r++, c--) { // check // diag
        test = ~((test == board_rec[r][c]) - 1) & board_rec[r][c];
    }
    if(init == test)
        return test;
    //printf("last check: passed // diags");
    return 0;
}

int make_move(char board[BOARD_SIZE][BOARD_SIZE + 1], int board_rec[SIDE_LEN][SIDE_LEN]
              , int r, int c, char piece) {
        if(board_rec[r][c] || !((0 <= r) & (r <= (SIDE_LEN - 1)) & (0 <= c) & (c <= (SIDE_LEN - 1)))) {
            printf("Invalid move. \n");
            return 1;
        }
        board[2 * r][2 * c] = piece;
        if(piece == 'X')
            board_rec[r][c] = 1;        // X associated with 1
        else if(piece == 'O')
            board_rec[r][c] = -1;       // O with -1
        else
            board_rec[r][c] = 12345678;
        printf("\n%s\n\n", *board);
        return 0;
}

int main()
{
    char player1[MAX_NAME_LEN];// = "p1";
    char player2[MAX_NAME_LEN];// = "p2";
    char board[BOARD_SIZE][BOARD_SIZE + 1];
    int board_record[SIDE_LEN][SIDE_LEN];
    char *move = malloc(sizeof(*move));
    int movecount = 0;
    int r;
    int c;
    int win;

    //collect names
    printf("LET'S PLAY TIC TAC TOE Y'ALL\n");
    printf("Only the first character of whatever you enter will be read. e.g. 1234 is");
    printf(" interpreted as 1.\n\n");

    getplayername(player1, 1);
    getplayername(player2, 2);

    initialize_board(board);
    initialize_board_record(board_record);

    while(!strcmp(player1, player2)){ // TODO have users reenter names
        printf("Names must not match, ya ducks\n Player 2 pick a different name.\n");
        //getplayername(player2, 2);
        trim_newline(player1);
        printf("%s", player1);
    }

    do {
        do {
            printf("Make your move, %s\n", player1);
            *move = fgetc(stdin);
            r = (*move - '1') / SIDE_LEN; // ASSUME move is a char 1-9
            c = (*move - '1') % SIDE_LEN;
            if(*move != '\n')
                fgetc(stdin);   // The enter is considered a '\n' char in fgetc
        } while(make_move(board, board_record, r, c, 'X'));
        movecount++;
        if((win = check_win(board_record)) || movecount >= 9)
            break;

        do {
            printf("Make your move, %s\n", player2);
            *move = fgetc(stdin);
            r = (*move - '1') / SIDE_LEN; // ASSUME move is a char 1-9
            c = (*move - '1') % SIDE_LEN;
            if(*move != '\n')
                fgetc(stdin);   // The enter is considered a '\n' char in fgetc
        } while(make_move(board, board_record, r, c, 'O'));
        movecount++;
    } while(!(win = check_win(board_record)) && (movecount < SIDE_LEN*SIDE_LEN));

    if(win == 1)
        printf("%s wins!!", player1);
    else if(win == -1)
        printf("%s wins!!", player2);
    else
        printf("Ya drawed!!");

    free(move);
    //free(board);
    return 0;
}
