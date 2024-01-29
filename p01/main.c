#include<limits.h>
#include<stdio.h>

int main(int argc, char** argv) {
    int last_val = INT_MAX;
    int cur_val = 0;
    int count_inc = 0;

    while (!feof(stdin)) {
        scanf("%d", &cur_val);
        if (cur_val > last_val) {
            count_inc++;
        }
        last_val = cur_val;
        //printf("CurVal %d\n", cur_val);
    }
    printf("%d\n", count_inc);
    return 0;
}
