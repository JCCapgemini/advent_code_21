#include<stdio.h>
#include<glib.h>
#include<stdint.h>
#include<assert.h>

void sorted_insert(int* best, const int size, const int t) {
    int idx;
    for(int i=0; i<size; i++) {
        printf("%d ", best[i]);
    }
    printf("\n");
    for(idx=size-1; idx>=0; idx--) { if(best[idx]<t) break;}
    if (idx>=0) {
        for(int z=idx; z>0; z--) best[z-1] = best[z];
        best[idx] = t;
    }
}

char get_from_grid(GPtrArray* grid, int row, int col) {
    return ((char*)g_ptr_array_index(grid, row))[col];
}

int count_descendings(GPtrArray* grid, int row, int col) {
    int result = 0;

    char* my_char = &((char*)g_ptr_array_index(grid, row))[col];
    // If already counted
    if (*my_char == '#' || *my_char == '9') return 0;
    // Self
    result++;
    const int my_val = *my_char - '0';
    *my_char = '#';

    // Up
    if (row>0 && ((get_from_grid(grid, row-1, col) - '0') > my_val)) {
        result += count_descendings(grid, row-1, col);
    }
    //Left
    if (col>0 && ((get_from_grid(grid, row, col-1) - '0') > my_val)) {
        result += count_descendings(grid, row, col-1);
    }
    //Down
    if (row<(grid->len-1) && ((get_from_grid(grid, row+1, col) - '0') > my_val)) {
        result += count_descendings(grid, row+1, col);
    }
    //Right
    if (row<(strlen((char*)g_ptr_array_index(grid, row))-1) && ((get_from_grid(grid, row, col+1) - '0') > my_val)) {
        result += count_descendings(grid, row, col+1);
    }

    return result;
}

void print_grid(GPtrArray const* grid) {
    for(int i=0; i<grid->len; i++) {
        printf("%s\n", (char*)g_ptr_array_index(grid, i));
    }
}

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

#define BUF_SIZ 3
    int best[BUF_SIZ] = {0};

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
                print_grid(grid);
                sorted_insert(best, BUF_SIZ, count_descendings(grid, i, j));
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
            print_grid(grid);
            sorted_insert(best, BUF_SIZ, count_descendings(grid, row, j));
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
            print_grid(grid);
            sorted_insert(best, BUF_SIZ, count_descendings(grid, row, j));
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
            print_grid(grid);
            sorted_insert(best, BUF_SIZ, count_descendings(grid, i, col));
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
            print_grid(grid);
            sorted_insert(best, BUF_SIZ, count_descendings(grid, i, col));
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
        print_grid(grid);
        sorted_insert(best, BUF_SIZ, count_descendings(grid, row, col));
    }
    // Top right
    row = 0; col = strlen(g_ptr_array_index(grid, 0))-1;
    tv = ((char*)g_ptr_array_index(grid, row))[col] - '0';
    tv1 = ((char*)g_ptr_array_index(grid, row+1))[col] - '0';
    tv2 = ((char*)g_ptr_array_index(grid, row))[col-1] - '0';
    if((tv<tv1) && (tv<tv2)) {
        print_grid(grid);
        sorted_insert(best, BUF_SIZ, count_descendings(grid, row, col));
    }
    // Bottom Left
    row = grid->len-1; col = 0;
    tv = ((char*)g_ptr_array_index(grid, row))[col] - '0';
    tv1 = ((char*)g_ptr_array_index(grid, row-1))[col] - '0';
    tv2 = ((char*)g_ptr_array_index(grid, row))[col+1] - '0';
    if((tv<tv1) && (tv<tv2)) {
        print_grid(grid);
        sorted_insert(best, BUF_SIZ, count_descendings(grid, row, col));
    }
    // Bottom Right 
    row = grid->len-1; col = strlen(g_ptr_array_index(grid, 0))-1;
    tv = ((char*)g_ptr_array_index(grid, row))[col] - '0';
    tv1 = ((char*)g_ptr_array_index(grid, row-1))[col] - '0';
    tv2 = ((char*)g_ptr_array_index(grid, row))[col-1] - '0';
    if((tv<tv1) && (tv<tv2)) {
        print_grid(grid);
        sorted_insert(best, BUF_SIZ, count_descendings(grid, row, col));
    }
    print_grid(grid);

    int total = 1;
    for(int i=0; i<BUF_SIZ; i++) {
        total*=best[i];
    }

    printf("%d\n", total);
    return 0;
}
