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
#define GArType int
    GArray* arr_pos = g_array_new(FALSE, FALSE, sizeof(GArType));
    char cur_chr = '\0';
    for (*i=0; str[*i]!='\0'; (*i)++) {
        cur_chr = str[*i];
        //printf("::%c::\n", cur_chr);
        if (isOpenBraket(cur_chr)) {
            // Is open
            g_array_append_val(arr_pos, *i);
        } else {
            // Is close
            if (arr_pos->len > 0) {
                // Stack not empty
                const GArType match_pos = g_array_index(arr_pos, GArType, arr_pos->len-1);
                const char match_char = str[match_pos];
                g_array_remove_index(arr_pos, arr_pos->len-1);
                //printf("Comp stack: %c actual: %c\n", match_char, cur_chr);
                if (closedBraketPair(match_char) != cur_chr) {
                    // Close bracket not match
                    break;
                }
                // Close bracket match
                // Remove the character for debug
                str[match_pos] = ' ';
                str[*i] = ' ';
            } else {
                // Stack empty
                break;
            }
        }
    }

    if((strlen(str)) == *i) {
        cur_chr = '\0';
    } 
    //else {
    //    printf(" ret: %c\nStack: ", cur_chr);
    //    for(int j=0; j<arr_pos->len; j++) {
    //        printf("%c ", str[g_array_index(arr_pos, GArType, j)]);
    //    }
    //}

    printf("%s\n", str);

    g_array_free(arr_pos, TRUE);
    return cur_chr;
#undef GArType
}

#define STR_MAX 1000
int main(int argc, char** argv) {
    char str[STR_MAX] = {0};
    int hits[CLOSE_BRAKETS_SIZE] = {0};
    int lc = 0;

    scanf("%1000s", str);
    while (!feof(stdin)) {
        printf("\n\nLine:\n%s\n", str);
        if(strlen(str) == 0) continue;
        int pos = 0;
        char res = '\0';
        res = recursion(str, &pos);
        if (res!='\0') {
            printf("The braket was: %c\n", res);
            hits[(int)closeBrakets(res)]++;
        } else {
            printf("No problems %d\n", lc);
        }
        printf("%lld %d\n", strlen(str), pos);
        assert(pos <= (strlen(str) +1));
        str[0] = '\0';
        lc++;
        scanf("%1000s", str);
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
