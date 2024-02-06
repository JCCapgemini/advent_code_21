#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<stdint.h>
#include<glib.h>
#include<math.h>

int uint64_t_compare(gconstpointer a, gconstpointer b) {
    const uint64_t va = *(uint64_t*)a;
    const uint64_t vb = *(uint64_t*)b;
    if (va<vb){
        return -1;
    } else if(va>vb) {
        return 1;
    }
    return 0;
}

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

#define GArType int
uint64_t recursion(char* str, GArray* arr_pos) {
    char cur_chr = '\0';
    int i;
    for (i=0; str[i]!='\0'; (i)++) {
        cur_chr = str[i];
        //printf("::%c::\n", cur_chr);
        if (isOpenBraket(cur_chr)) {
            // Is open
            g_array_append_val(arr_pos, i);
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
                str[i] = ' ';
            } else {
                // Stack empty
                break;
            }
        }
    }

    printf("%s\n", str);

    uint64_t total = 0;
    if((strlen(str)) == i) {
        for(int i=arr_pos->len-1; i>=0; i--) {
            GArType p = g_array_index(arr_pos, GArType, i);
            char pc = str[p];
            char pm = closedBraketPair(pc);
            int inc = 0;
            switch (pm) {
                case ')': inc=1;break;
                case ']': inc=2;break;
                case '}': inc=3;break;
                case '>': inc=4;break;
                default: assert(FALSE);
            }
            total = total * 5 + inc;
        }
        printf("Number: %lld\n", total);
    } 

    return total;
}

#define STR_MAX 1000
int main(int argc, char** argv) {
    char str[STR_MAX] = {0};

    GArray* arr_totals = g_array_new(FALSE, FALSE, sizeof(uint64_t));
    GArray* arr_pos = g_array_new(FALSE, FALSE, sizeof(GArType));
    scanf("%1000s", str);
    while (!feof(stdin)) {
        printf("\n\nLine:\n%s\n", str);
        if(strlen(str) == 0) continue;
        arr_pos->len = 0;

        uint64_t result = recursion(str, arr_pos);
        if (result > 0) g_array_append_val(arr_totals, result);

        str[0] = '\0';
        scanf("%1000s", str);
    }
    g_array_free(arr_pos, TRUE);

    printf("Results:\n");
    g_array_sort(arr_totals, uint64_t_compare);
    for(int i=0; i<arr_totals->len; i++){
        uint64_t t = g_array_index(arr_totals, uint64_t, i);
        printf("%lld ", t);
    }
    printf("\n");

    int midPos = floor(((float)arr_totals->len)/2);
    uint64_t final_res = g_array_index(arr_totals, uint64_t, midPos);

    printf("%lld\n", final_res);

    g_array_free(arr_totals, TRUE);
    return 0;
}
