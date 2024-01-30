#define _GNU_SOURCE

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<glib.h>
#include<ctype.h>
#include<stdint.h>
#include<assert.h>

struct Digit {
    char digit;
    char segment_count;
    char segment_mask;
};

char const segment_chars_to_mask(const char* const str) {
    char res = 0;
    for (int i=0; str[i] != '\0'; i++) {
        assert(str[i]-'a' < 8);
        res |= 1<<(str[i]-'a');
    }
    return res;
}

struct Digit* digit_of_mask_match(struct Digit* digits, char segment_count, char mask) {
    // Search for a digit with segment count and matching mask
    struct Digit* result = NULL;
    for(int i=0; i<10; i++) {
        struct Digit* cd = &digits[i];
        if (cd->segment_count == segment_count && cd->segment_mask != 0 && (cd->segment_mask & mask) == mask) {
            assert(result == NULL);
            result = cd;
        }
    }
    return result;
}

void resolve_all_digits(struct Digit digits[10]){
    struct Digit sol_digits[10] = {0};
    //Initialize all digit values to -1
    for(int i=0; i<10; i++) sol_digits[i].digit = -1;
    // Always same known: 1, 4, 7, 8
    for(int i=0; i<10; i++) {
        const int digit = digits[i].digit;
        if (digit != -1) {
            // Copy the known digit
            sol_digits[digit] = digits[i];
            digits[i].segment_mask = 0;
        }
    }
    // Search and set 9, it will be only 1 with perfect mask with 4
    struct Digit* cd = digit_of_mask_match(digits, 6, sol_digits[4].segment_mask);
    assert(cd != NULL);
    sol_digits[9] = *cd;
    sol_digits[9].digit = 9;
    cd->segment_mask = 0;
    // Search and set 3, it will be only 1 with perfect mask with 7
    cd = digit_of_mask_match(digits, 5, sol_digits[7].segment_mask);
    assert(cd != NULL);
    sol_digits[3] = *cd;
    sol_digits[3].digit = 3;
    cd->segment_mask = 0;
    // Search and set 0, it will be only 1 with perfect mask with 1
    cd = digit_of_mask_match(digits, 6, sol_digits[1].segment_mask);
    assert(cd != NULL);
    sol_digits[0] = *cd;
    sol_digits[0].digit = 0;
    cd->segment_mask = 0;
    // Search for 6, only remaining of 6 segments
    int loc = -1;
    for(loc=0; !(digits[loc].segment_count == 6 && digits[loc].segment_mask != 0) && loc<10; loc++)
        assert(loc < 10);
    cd = &digits[loc];
    assert(cd != NULL);
    sol_digits[6] = *cd;
    sol_digits[6].digit = 6;
    cd->segment_mask = 0;
    // Search and set 5, it will be only 1 with perfect mask with 6 & 9
    cd = digit_of_mask_match(digits, 5, sol_digits[6].segment_mask & sol_digits[9].segment_mask);
    assert(cd != NULL);
    sol_digits[5] = *cd;
    sol_digits[5].digit = 5;
    cd->segment_mask = 0;
    // Search for 2, only remaining of 5 segments
    for(loc=0; !(digits[loc].segment_count == 5 && digits[loc].segment_mask != 0) && loc<10; loc++)
        assert(loc < 10);
    cd = &digits[loc];
    assert(cd != NULL);
    sol_digits[2] = *cd;
    sol_digits[2].digit = 2;
    cd->segment_mask = 0;

    for(int i=0; i<10; i++) {
        printf("Digits val %d sc %d msk %x\n", digits[i].digit, digits[i].segment_count, digits[i].segment_mask);
    }

    for(int i=0; i<10; i++) {
        printf("Sol digits val %d sc %d msk %x\n", sol_digits[i].digit, sol_digits[i].segment_count, sol_digits[i].segment_mask);
        assert(sol_digits[i].digit == i);
        assert(sol_digits[i].segment_mask != 0);
        for(int j=0; j<10; j++) {
            // Force that all masks are unique, meaning all digits have single representation
            if (i!=j) assert(sol_digits[i].segment_mask != sol_digits[j].segment_mask);
        }
        digits[i] = sol_digits[i];
    }
    //printf("Early manual exit!\n");
    //exit(1);

}

#define STR_MAX 200
int main(int argc, char** argv) {
    uint64_t total = 0;
    char str[STR_MAX] = "\0";
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        printf("\nLNR!\n");
        struct Digit digits[10] = {0};
        char* v;
        int start = 0;
        int counter = 0;
        for(int i=0; counter < 10; i++) {
            if (str[i] == ' ' || str[i] == '\n') {
                struct Digit* cd = &digits[counter];
                str[i] = '\0';
                v = &str[start];
                printf("Str: %s\n", v);
                start = i+1;
                switch (strlen(v)) {
                    case 2: *cd = (struct Digit){.digit = 1, .segment_count = 2, .segment_mask = segment_chars_to_mask(v)}; break;
                    case 3: *cd = (struct Digit){.digit = 7, .segment_count = 3, .segment_mask = segment_chars_to_mask(v)}; break;
                    case 4: *cd = (struct Digit){.digit = 4, .segment_count = 4, .segment_mask = segment_chars_to_mask(v)}; break;
                    case 7: *cd = (struct Digit){.digit = 8, .segment_count = 7, .segment_mask = segment_chars_to_mask(v)}; break;
                    default: *cd = (struct Digit){.digit = -1, .segment_count = strlen(v), .segment_mask = segment_chars_to_mask(v)}; break;
                }
                counter++;
            }
        }
        resolve_all_digits(digits);
        for(; str[start] != '|'; start++);
        printf("The split!\n");
        start+=2;
        int remaining_count = 4;
        uint64_t curr_total = 0;
        for(int i=start; str[i] != '\0'; i++) {
            if (str[i] == ' ' || str[i] == '\n') {
                str[i] = '\0';
                v = &str[start];
                start = i+1;
                struct Digit td = {.digit = -1, .segment_count = strlen(v), .segment_mask = segment_chars_to_mask(v)};
                int cur_val = -1;
                for(int j=0; j<10; j++) {
                    if(digits[j].segment_mask == td.segment_mask) {
                        cur_val = digits[j].digit;
                        break;
                    }
                }
                assert(cur_val != -1);
                printf("Str: %s Val: %d\n", v, cur_val);
                curr_total += pow(10, --remaining_count) * cur_val;
            }
        }
        total += curr_total;
        printf("TTot: %lld\n", curr_total);
        fgets(str, STR_MAX, stdin);
    }

    printf("%lld\n", total);
    return 0;
}
