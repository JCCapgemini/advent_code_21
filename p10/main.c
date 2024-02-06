#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<glib.h>

#define CLOSE_BRAKETS_SIZE 4
typedef enum {
    CloseRoundBraket,
    CloseSquareBraket,
    CloseCurlyBraket,
    CloseAngleBraket,
} CloseBrakets;

CloseBrakets closeBrakets(char c) {
    switch (c) {
        case ')': return CloseRoundBraket;
        case ']': return CloseSquareBraket;
        case '}': return CloseCurlyBraket;
        case '>': return CloseAngleBraket;
        default: printf("Bad close braket\n"); assert(FALSE); 
    }
}

char isOpenBraket(char c) {
    switch (c) {
        case '(':
        case '[':
        case '{':
        case '<': return 1;
        default: return 0; 
    }
}

char closedBraketPair(char c) {
    switch (c) {
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        case '<': return '>';
        default: exit(1); 
    }
}

char recursion(char* str, int* i) {
    GArray* arr_stack = g_array_new(FALSE, FALSE, sizeof(char));
    char cur_chr = '\0';
    for (*i=0; str[*i]!='\0'; (*i)++) {
        cur_chr = str[*i];
        printf("::%c::\n", cur_chr);
        if (isOpenBraket(cur_chr)) {
            // Is open
            g_array_append_val(arr_stack, cur_chr);
        } else {
            // Is close
            if (arr_stack->len > 0) {
                // Stack not empty
                const char match_char = g_array_index(arr_stack, char, arr_stack->len-1);
                g_array_remove_index(arr_stack, arr_stack->len-1);
                printf("Comp stack: %c actual: %c\n", match_char, cur_chr);
                if (closedBraketPair(match_char) != cur_chr) {
                    // Close bracket not match
                    printf("eary exit!\n");
                    break;
                }
            } else {
                // Stack empty
                printf("eary exit!\n");
                break;
            }
        }
    }

    if((strlen(str)) == *i) {
        cur_chr = '\0';
    } else {
        printf(" ret: %c\nStack: ", cur_chr);
        for(int j=0; j<arr_stack->len; j++) {
            printf("%c ", g_array_index(arr_stack, char, j));
        }
    }

    printf("\n");

    g_array_free(arr_stack, TRUE);
    return cur_chr;
}

#define STR_MAX 1000
int main(int argc, char** argv) {
    char str[STR_MAX] = {0};
    int hits[CLOSE_BRAKETS_SIZE] = {0};

    scanf("%100s", str);
    while (!feof(stdin)) {
        printf("\n\nLine:\n%s", str);
        if(strlen(str) == 0) continue;
        int pos = 0;
        char res = '\0';
        printf("\nStart pos:%d\n", pos);
        res = recursion(str, &pos);
        if (res!='\0') {
            printf("The braket was: %c\n", res);
            hits[(int)closeBrakets(res)]++;
        }
        printf("%lld %d %c\n", strlen(str), pos, res);
        assert(pos <= (strlen(str) +1));
        str[0] = '\0';
        scanf("%100s", str);
    }
    int total = hits[(int)CloseRoundBraket] * 3 +
        hits[(int)CloseSquareBraket] * 57 + 
        hits[(int)CloseCurlyBraket] * 1197 + 
        hits[(int)CloseAngleBraket] * 25137;

    printf("%d %d %d %d\n",
            (int)CloseRoundBraket,
            (int)CloseSquareBraket,
            (int)CloseCurlyBraket,
            (int)CloseAngleBraket
          );

    printf("%d %d %d %d\n",
            hits[(int)CloseRoundBraket],
            hits[(int)CloseSquareBraket],
            hits[(int)CloseCurlyBraket],
            hits[(int)CloseAngleBraket]
          );

    printf("%d\r", total);
    return 0;
}
