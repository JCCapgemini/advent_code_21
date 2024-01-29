#include<limits.h>
#include<stdio.h>

#define STR_MAX 100
int main(int argc, char** argv) {
    char str[STR_MAX] = "\0";
    int val = 0;

    int hor = 0;
    int depth = 0;

    scanf("%100s %d", str,  &val);
    while (!feof(stdin)) {
        switch (str[0]) {
            case 'f': hor += val; break;
            case 'u': depth -= val; break;
            case 'd': depth += val; break;
            default:return 1;
        }
        scanf("%100s %d", str,  &val);
    }
    printf("%d\r", hor * depth);
    return 0;
}
