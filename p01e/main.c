#include<limits.h>
#include<stdio.h>

#define WINDOW 3
int main(int argc, char** argv) {
    int acum[WINDOW] = {0};
    int acum_itr = 0;
    int last_acum = INT_MAX;
    int count_inc = 0;
    int cur_val = 0;

    for (int i=0;i<(WINDOW-1);i++){
        scanf("%d", &cur_val);
        acum[acum_itr] = cur_val;
        acum_itr = (acum_itr + 1);
    }

    scanf("%d", &cur_val);
    while (!feof(stdin)) {
        acum[acum_itr] = cur_val;
        acum_itr = (acum_itr + 1) * (acum_itr < (WINDOW-1));
        int acum_t = 0;
        for (int i=0;i<WINDOW;i++) { acum_t += acum[i]; };
        if (acum_t > last_acum) {
            count_inc++;
        }
        last_acum = acum_t;
        //printf("Val %d Acum %d Itr %d\n", cur_val, acum_t, acum_itr);
        scanf("%d", &cur_val);
    }
    printf("%d\r", count_inc);
    return 0;
}
