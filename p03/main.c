#include<limits.h>
#include<stdio.h>

#define STR_MAX 100
int main(int argc, char** argv) {
    char str[STR_MAX] = "\0";
    int counts[STR_MAX] = {0};
    int ncounts[STR_MAX] = {0};
    int count = 0;
    int width = 0;

    scanf("%100s", str);
    while (!feof(stdin)) {
        count++;
        for(int i=0; str[i] != '\0'; i++){
            switch(str[i]){
                case '0': ncounts[i]++; break;
                case '1': counts[i]++; break;
                default: return 1;
            };
            width = width * (width>=i) | i * (width<i);
        }
        scanf("%100s", str);
    }
    //printf("Width %d\n", width);
    int result = 0;
    int negated = 0;
    // Width is inclusive due to being the max index allowed
    for(int i=width; i >= 0 ; i--){
        //printf("%d", counts[i] > ncounts[i]);
        // We swap it around because I store it MSB to LSB
        if (counts[i] > ncounts[i]) {
            result |= 1<<(width-i);
        } else {
            negated |= 1<<(width-i);
        }
    }
    //printf("\n");
    //printf("Res %d Neg %d\n", result, negated);
    printf("%d\r", result * negated);
    return 0;
}
