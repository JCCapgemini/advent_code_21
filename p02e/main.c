#include<limits.h>
#include<stdio.h>

#define STR_MAX 100
int main(int argc, char** argv) {
    char str[STR_MAX] = "\0";
    int val = 0;

    int hor = 0;
    int depth = 0;
    int aim = 0;

    scanf("%100s %d", str,  &val);
    while (!feof(stdin)) {
        switch (str[0]) {
            case 'f': 
                depth += val * aim;
                hor += val;
                break;
            case 'u':
                aim -= val;
                break;
            case 'd':
                aim += val;
                break;
            default:return 1;
        }
        //printf("Hor %d Depth %d Aim %d\n", hor, depth, aim);
        scanf("%100s %d", str,  &val);
    }
    printf("%d\r", hor * depth);
    return 0;
}
