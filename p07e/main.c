#define _GNU_SOURCE

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<glib.h>
#include<assert.h>

int calculate_distance(int distance) {
    return (distance * distance + distance) / 2;
}

#define STR_MAX 4096 
int main(int argc, char** argv) {
    GArray* crab_locations = g_array_new(FALSE, FALSE, sizeof(int));
    char str[STR_MAX] = "\0";
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        int start = 0;
        int v;
        for(int i=0; str[i] != '\0'; i++) {
            if (str[i] == ',') {
                str[i] = '\0';
                v = atoi(&str[start]);
                start = i+1;
                g_array_append_val(crab_locations, v);
            }
        }
        v = atoi(&str[start]);
        g_array_append_val(crab_locations, v);
        fgets(str, STR_MAX, stdin);
    }

    // Get limits
    printf("bucket status\n");
    int cc =  g_array_index(crab_locations, int, 0);
    printf("Loc: %d\n", cc);
    int min = cc;
    int max = cc;
    for(int i=1; i<crab_locations->len; i++) {
        cc =  g_array_index(crab_locations, int, i);
        printf("Loc: %d\n", cc);
        if (cc > max) {
            max = cc;
        }
        if (cc < min) {
            min = cc;
        }
    }

    // See the best result
    int best = -1;
    for(int i=min; i<=max; i++) {
        int cur = 0;
        for(int j=0; j<crab_locations->len; j++) {
            cc = g_array_index(crab_locations, int, j);
            cur += calculate_distance(abs(cc - i));
        }
        printf("Pos ref %d val %d\n", i, cur);
        if (best == -1 || best > cur) {
            best = cur;
        }
    }

    int total = best;
    printf("%d\n", total);
    return 0;
}
