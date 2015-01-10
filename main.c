#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 30
#define BOARD_SIZE 5 //for a 3x3 only

const int SIDE_LEN = BOARD_SIZE/2 + 1;

void trim_newline(char *str){ //abdc\n\0 --> abdc\0\0
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

/* initialize_board: set an array (the board) to this:
 * 1|2|3
 * -+-+-
 * 4|5|6
 * -+-+-
 * 7|8|9
 */
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

/* initialize_board_record: takes a board record[][], sets errthing to 0
 */
void initialize_board_record(int board_rec[SIDE_LEN][SIDE_LEN]) {
    int r;
    int c;
    for(r = 0; r < SIDE_LEN; r++)
        for(c = 0; c < SIDE_LEN; c++)
            board_rec[r][c] = 0;
}

/* check_win: checks each win pattern for a 3x3 board.
 * @params: board_rec: a board record which keeps track of who moved where
 * returns 0 if no wins, otherwise returns a number corresponding to a player
 */
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
              , char move, char piece) {
        int r = (move - '1') / SIDE_LEN;
        int c = (move - '1') % SIDE_LEN;

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

void clear_stream(char in) {
    char target;
    while(in != '\n' && (target = getchar()) != '\n' && target != EOF){}
}

void play_self(char board[BOARD_SIZE][BOARD_SIZE + 1], int board_rec[SIDE_LEN][SIDE_LEN]) {
    printf("\n%s\n", *board);
}

int main()
{
    char player1[MAX_NAME_LEN]; // = "p1";
    char player2[MAX_NAME_LEN]; // = "p2";
    char board[BOARD_SIZE][BOARD_SIZE + 1];
    int board_record[SIDE_LEN][SIDE_LEN];
    char move;
    char y;
    int num_players;
    int movecount;
    int win;

    //collect names
    printf("LET'S PLAY TIC TAC TOE Y'ALL\n");

    do{
        do{
            initialize_board(board);
            initialize_board_record(board_record);
            movecount = 0;

            printf("How many players?\n");  // get number of players
            num_players = getchar() - '0';
            clear_stream(num_players);
            if(num_players < 0 || num_players > 9)
                printf("Invalid number of players.\n");
        } while(num_players < 0 || num_players > 9);
        if(!num_players)
            goto end;

        getplayername(player1, 1);
        getplayername(player2, 2);

        printf("\nOnly the first character of whatever you enter will be read. e.g. 1234 is");
        printf(" interpreted as 1.\n\n");

        while(!strcmp(player1, player2)){
            printf("Names must not match, ya ducks\n Player 2 pick a different name.\n");
            getplayername(player2, 2);
            trim_newline(player2);
        }

        do {
            do {
                printf("Make your move, %s\n", player1);

                move = getchar();
                clear_stream(move);

            } while(make_move(board, board_record, move, 'X'));
            movecount++;
            if((win = check_win(board_record)) || movecount >= 9)
                break;

            do {
                printf("Make your move, %s\n", player2);

                move = getchar();
                clear_stream(move);

            } while(make_move(board, board_record, move, 'O'));
            movecount++;
        } while(!(win = check_win(board_record)) && (movecount < SIDE_LEN*SIDE_LEN));

        if(win == 1)
            printf("%s wins!!", player1);
        else if(win == -1)
            printf("%s wins!!", player2);
        else
            printf("Ya drawed!!");

        end:
        if(!num_players) {
            play_self(board, board_record);
            printf("A strange game. The only way to win is to not play. ");
        }
        printf("\nPlay again? Enter y for yes, anything else for no.\n");
        y = getchar();
        clear_stream(y);
    } while(y == 'y');
    return 0;
}
