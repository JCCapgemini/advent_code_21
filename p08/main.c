#define _GNU_SOURCE

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<glib.h>
#include<assert.h>

#define STR_MAX 200
int main(int argc, char** argv) {
    int total = 0;
    char str[STR_MAX] = "\0";
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        char* v;
        int start = 0;
        for(start=0; str[start] != '|'; start++);
        start++;
        for(int i=start; str[i] != '\0'; i++) {
            if (str[i] == ' ' || str[i] == '\n') {
                str[i] = '\0';
                v = &str[start];
                //printf("Str: %s\n", v);
                start = i+1;
                switch (strlen(v)) {
                    case 2:
                    case 3:
                    case 4:
                    case 7:  total++; break;
                    default: break;
                }
                //printf("Tot: %d\n", total);
            }
        }
        printf("Tot: %d\n", total);
        fgets(str, STR_MAX, stdin);
    }

    printf("%d\n", total);
    return 0;
}
