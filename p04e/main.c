#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<glib.h>
#include<ctype.h>

const int CLEARED_VAL = -1;
#define BOARD_SIZE 5
typedef int BOARD[BOARD_SIZE * BOARD_SIZE];

void print_board(BOARD* b) {
    for (int j=0; j<BOARD_SIZE; j++) {
        for(int z=0; z<BOARD_SIZE; z++){
            printf("%02d ", (*b)[j*BOARD_SIZE + z]);
        }
        printf("\n");
    }
}

void print_boards(GPtrArray const * const parr) {
    for (int i=0; i<parr->len; i++) {
        printf("\nBoard: %d\n", i);
        BOARD* cb = g_ptr_array_index(parr, i);
        print_board(cb);
    }
}

void clear_val_board(BOARD* b, int val) {
    for (int j=0; j<BOARD_SIZE; j++) {
        for(int z=0; z<BOARD_SIZE; z++){
            if ((*b)[j*BOARD_SIZE + z] == val) {
                (*b)[j*BOARD_SIZE + z] = CLEARED_VAL;
            }
        }
    }
}

int sum_not_clear_board(BOARD* b) {
    int res = 0;
    for (int j=0; j<BOARD_SIZE; j++) {
        for(int z=0; z<BOARD_SIZE; z++){
            if ((*b)[j*BOARD_SIZE + z] != CLEARED_VAL) {
                res += (*b)[j*BOARD_SIZE + z];
            }
        }
    }
    return res;
}

int check_win_board(BOARD* b) {
    //Check row
    for (int j=0; j<BOARD_SIZE; j++) {
        int valid = TRUE;
        for(int z=0; z<BOARD_SIZE; z++){
            if ((*b)[j*BOARD_SIZE + z] != CLEARED_VAL) {
                valid = FALSE;
                break;
            }
        }
        if (valid) {
            return TRUE;
        }
    }

    //Check cols
    for(int z=0; z<BOARD_SIZE; z++){
        int valid = TRUE;
        for (int j=0; j<BOARD_SIZE; j++) {
            if ((*b)[j*BOARD_SIZE + z] != CLEARED_VAL) {
                valid = FALSE;
                break;
            }
        }
        if (valid) {
            return TRUE;
        }
    }
    return FALSE;
}

int mark_boards(GPtrArray const * const parr, int val) {
    int result = -1;
    for (int i=0; i<parr->len; i++) {
        printf("\nBoard: %d\n", i);
        BOARD* cb = g_ptr_array_index(parr, i);
        clear_val_board(cb, val);
        if (check_win_board(cb) == TRUE) {
            result = i;
        }
        print_board(cb);
    }
    return result;
}

#define STR_MAX 1024
int main(int argc, char** argv) {
    char str[STR_MAX] = "\0";
    GArray* bingo_numbers = g_array_new(FALSE, FALSE, sizeof(int));
    GPtrArray* bingo_boards = g_ptr_array_new();

    char reading_nums = TRUE;

    BOARD* current_board = malloc(sizeof(BOARD));
    int board_row = 0;
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        printf("Input read! :: %s\n", str);
        if (reading_nums) {
            int start = 0;
            for (int i=0; str[i] != '\0'; i++) {
                if (!isdigit(str[i])) {
                    str[i] = '\0';
                    int tv = atoi(&str[start]);
                    g_array_append_val(bingo_numbers, tv);
                    start = i+1;
                    if (str[i] == '\n') {
                        printf("NewLineR\n");
                        reading_nums = FALSE;
                        break;
                    }
                }
            }
            board_row = 0;
            reading_nums = FALSE;
        } else {
            if (strlen(str)>2) {
                printf("Not!\n");
                int start = 0;
                int col = 0;
                for (int i=0; str[i] != '\0'; i++) {
                    if (!isdigit(str[i])) {
                        str[i] = '\0';
                        if (start != i) {
                            current_board[0][board_row * BOARD_SIZE + col++] = atoi(&str[start]);
                        }
                        start = i+1;
                    }
                }
                board_row++;
            } else {
                // Skip first due to empty
                if (board_row != 0) {
                    board_row = 0;
                    printf("Stored!\n");
                    print_board(current_board);
                    g_ptr_array_insert(bingo_boards, -1, current_board);
                    current_board = malloc(sizeof(BOARD));
                }
            }
        }
        fgets(str, STR_MAX, stdin);
    }
    // Store last one not triggered
    printf("Stored!\n");
    print_board(current_board);
    g_ptr_array_insert(bingo_boards, -1, current_board);
    printf("Numbers:\n");
    for (int i=0; i<bingo_numbers->len; i++) {
        printf("%d ", g_array_index(bingo_numbers, int, i));
    }
    printf("\n");
    print_boards(bingo_boards);


    for (int i=0; i<bingo_numbers->len; i++) {
        const int cur_num = g_array_index(bingo_numbers, int, i);
        printf("==========Bingo Num: %d\n", cur_num);
        const int win_board = mark_boards(bingo_boards, cur_num);
        if (win_board > -1) {
            for (int j=0; j<bingo_boards->len; j++) {
                if (check_win_board(g_ptr_array_index(bingo_boards, j)) == TRUE) {
                    printf("Board! %d Winval %d\n", j, cur_num);
                    if (bingo_boards->len == 1) {
                        printf("Last!\n");
                        printf("%d\n", sum_not_clear_board(g_ptr_array_index(bingo_boards, j)) * cur_num);
                        return 0;
                    }
                    free(g_ptr_array_index(bingo_boards, j));
                    g_ptr_array_remove(bingo_boards, g_ptr_array_index(bingo_boards, j));
                    j--;
                }
            }
        }
    }
    return 0;
}
