#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

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
        default: printf("Bad close braket\n"); exit(1); 
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

char recursion(char* str, int* pos) {
    // Prevent overrun
    printf("%c", str[*pos]);
    const char mychar = str[*pos];
    if(!isOpenBraket(mychar)) return mychar;
    const char lim = closedBraketPair(str[*pos]);
    while(str[*pos] != '\0' && str[++*pos] != '\0' && str[*pos] != lim) {
        // Only allow open
        if(isOpenBraket(str[*pos])) {
            char res = recursion(str, pos);
            // If return was non error
            if (res != '\0') return res;
        } else {
            // Bad exit, we found another close
            printf("%c", str[*pos]);
            return str[*pos];
        }
    }
    // Good exit, we found our pair
    printf("%c", str[*pos]);
    return '\0';
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
        while(str[pos] != '\0' && res == '\0') {
            printf("\nStart pos:%d\n", pos);
            res = recursion(str, &pos);
            if (str[pos] != '\0' && str[pos+1] != '\0') ++pos;
        }
        if (res!=0) {
            hits[(int)closeBrakets(res)]++;
        }
        printf("\n%lld %d %c\n", strlen(str), pos, res);
        assert(pos <= strlen(str));
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
