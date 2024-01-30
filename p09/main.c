#include<stdio.h>
#include<glib.h>
#include<stdint.h>
#include<assert.h>

#define STR_MAX 1024
int main(int argc, char** argv) {
    GPtrArray* grid = g_ptr_array_new();
    char str[STR_MAX] = "\0";
    scanf("%1024s", str);
    while (!feof(stdin)) {
        printf("Str: %s\n", str);
        char* tr = malloc(sizeof(char) * (strlen(str)+1));
        strcpy(tr, str);
        g_ptr_array_insert(grid, -1, tr);
        scanf("%1024s", str);
    }

    int total = 0;

    // Normal loop, no edges
    for(int i=1; i<(grid->len-1); i++) {
        for(int j=1; j<(strlen(g_ptr_array_index(grid, i))-1); j++) {
            char cv = ((char*)g_ptr_array_index(grid, i))[j] - '0';
            char up, down, left, right;
            up = ((char*)g_ptr_array_index(grid, i-1))[j] - '0';
            down = ((char*)g_ptr_array_index(grid, i+1))[j] - '0';
            left = ((char*)g_ptr_array_index(grid, i))[j-1] - '0';
            right = ((char*)g_ptr_array_index(grid, i))[j+1] - '0';
            if((cv<up) && (cv<down) && (cv<left) && (cv<right)) {
                total += cv+1;
            }
        }
    }
    // Top
    int row = 0;
    for(int j=1; j<(strlen(g_ptr_array_index(grid, row))-1); j++) {
        char cv = ((char*)g_ptr_array_index(grid, row))[j] - '0';
        char down, left, right;
        down = ((char*)g_ptr_array_index(grid, row+1))[j] - '0';
        left = ((char*)g_ptr_array_index(grid, row))[j-1] - '0';
        right = ((char*)g_ptr_array_index(grid, row))[j+1] - '0';
        if((cv<down) && (cv<left) && (cv<right)) {
            total += cv+1;
        }
    }
    // Bottom
    row = grid->len-1;
    for(int j=1; j<(strlen(g_ptr_array_index(grid, row))-1); j++) {
        char cv = ((char*)g_ptr_array_index(grid, row))[j] - '0';
        char up, left, right;
        up = ((char*)g_ptr_array_index(grid, row-1))[j] - '0';
        left = ((char*)g_ptr_array_index(grid, row))[j-1] - '0';
        right = ((char*)g_ptr_array_index(grid, row))[j+1] - '0';
        if((cv<up) && (cv<left) && (cv<right)) {
            total += cv+1;
        }
    }
    // Left
    int col = 0;
    for(int i=1; i<(grid->len-1); i++) {
        char cv = ((char*)g_ptr_array_index(grid, i))[col] - '0';
        char up, down, right;
        up = ((char*)g_ptr_array_index(grid, i-1))[col] - '0';
        down = ((char*)g_ptr_array_index(grid, i+1))[col] - '0';
        right = ((char*)g_ptr_array_index(grid, i))[col+1] - '0';
        if((cv<up) && (cv<down) && (cv<right)) {
            total += cv+1;
        }
    }
    // Right
    col = strlen(g_ptr_array_index(grid, 0))-1;
    for(int i=1; i<(grid->len-1); i++) {
        char cv = ((char*)g_ptr_array_index(grid, i))[col] - '0';
        char up, down, left;
        up = ((char*)g_ptr_array_index(grid, i-1))[col] - '0';
        down = ((char*)g_ptr_array_index(grid, i+1))[col] - '0';
        left = ((char*)g_ptr_array_index(grid, i))[col-1] - '0';
        if((cv<up) && (cv<down) && (cv<left) ) {
            total += cv+1;
        }
    }
    // Corners
    // Top Left
    row = 0; col = 0;
    int tv, tv1, tv2;
    tv = ((char*)g_ptr_array_index(grid, row))[col] - '0';
    tv1 = ((char*)g_ptr_array_index(grid, row+1))[col] - '0';
    tv2 = ((char*)g_ptr_array_index(grid, row))[col+1] - '0';
    if((tv<tv1) && (tv<tv2)) {
        total += tv+1;
    }
    // Top right
    row = 0; col = strlen(g_ptr_array_index(grid, 0))-1;
    tv = ((char*)g_ptr_array_index(grid, row))[col] - '0';
    tv1 = ((char*)g_ptr_array_index(grid, row+1))[col] - '0';
    tv2 = ((char*)g_ptr_array_index(grid, row))[col-1] - '0';
    if((tv<tv1) && (tv<tv2)) {
        total += tv+1;
    }
    // Bottom Left
    row = grid->len-1; col = 0;
    tv = ((char*)g_ptr_array_index(grid, row))[col] - '0';
    tv1 = ((char*)g_ptr_array_index(grid, row-1))[col] - '0';
    tv2 = ((char*)g_ptr_array_index(grid, row))[col+1] - '0';
    if((tv<tv1) && (tv<tv2)) {
        total += tv+1;
    }
    // Bottom Right 
    row = grid->len-1; col = strlen(g_ptr_array_index(grid, 0))-1;
    tv = ((char*)g_ptr_array_index(grid, row))[col] - '0';
    tv1 = ((char*)g_ptr_array_index(grid, row-1))[col] - '0';
    tv2 = ((char*)g_ptr_array_index(grid, row))[col-1] - '0';
    if((tv<tv1) && (tv<tv2)) {
        total += tv+1;
    }
    printf("%d\n", total);
    return 0;
}
