#include<limits.h>
#include<stdio.h>
#include<stdint.h>
#include<glib.h>

//#define MIN(a,b) (((a)<(b))?(a):(b))
//#define MAX(a,b) (((a)>(b))?(a):(b))

#define STR_MAX 100
#define ARR_TYPE uint8_t 

void print_grid(GPtrArray* grid) {
    printf("Grid:\n");
    for(int i=0; i<grid->len; i++) {
        GArray* row = (GArray*) g_ptr_array_index(grid, i);
        for(int j=0; j<row->len;j++) {
            printf("%d ", g_array_index(row, ARR_TYPE, j));
        }
        printf("\n");
    }
}

void clear_grids(GPtrArray* grid, GPtrArray* sgrid) {
    for(int i=0; i<grid->len; i++) {
        GArray* row = (GArray*) g_ptr_array_index(grid, i);
        GArray* srow = (GArray*) g_ptr_array_index(sgrid, i);
        for(int j=0; j<row->len;j++) {
            ARR_TYPE* sv = &g_array_index(srow, ARR_TYPE, j);
            if(*sv) {
                *sv = 0;
                g_array_index(row, ARR_TYPE, j) = 0;
            }
        }
    }
}

int propagate_light(GPtrArray* grid, GPtrArray* sgrid, int r, int c) {
    int total = 0;
    ARR_TYPE* seen = &g_array_index((GArray*) g_ptr_array_index(sgrid, r), ARR_TYPE, c);
    if (!*seen) {
        ARR_TYPE* cv = &g_array_index((GArray*) g_ptr_array_index(grid, r), ARR_TYPE, c);
        if (*cv>9) {
            total++;
            *seen = 1;
            const int top = MAX(0, r-1);
            const int bot = MIN(grid->len-1, r+1);
            const int left = MAX(0, c-1);
            const int right = MIN(((GArray*) g_ptr_array_index(sgrid, r))->len-1, c+1);

            for(int i=top; i<=bot; i++) {
                for(int j=left; j<=right; j++) {
                    ARR_TYPE* ov = &g_array_index((GArray*) g_ptr_array_index(grid, i), ARR_TYPE, j);
                    ARR_TYPE* os = &g_array_index((GArray*) g_ptr_array_index(sgrid, i), ARR_TYPE, j);
                    *ov+=1;

                    if(*ov>9 && !*os) {
                        total += propagate_light(grid, sgrid, i, j);
                    }
                }
            }
        }
    }
    return total;
}

int iterate_grid(GPtrArray* grid, GPtrArray* sgrid) {
    int total = 0;
    clear_grids(grid, sgrid);

    for(int i=0; i<grid->len; i++) {
        GArray* row = (GArray*) g_ptr_array_index(grid, i);
        for(int j=0; j<row->len;j++) {
            g_array_index(row, ARR_TYPE, j)++;
            total += propagate_light(grid, sgrid, i, j);
        }
    }

    return total;
}

int main(int argc, char** argv) {
    GPtrArray* grid = g_ptr_array_new();
    GPtrArray* sgrid = g_ptr_array_new();
    char str[STR_MAX] = "\0";
    scanf("%100s", str);
    while (!feof(stdin)) {
        GArray* row = g_array_new(FALSE, FALSE, sizeof(ARR_TYPE));
        GArray* srow = g_array_new(FALSE, FALSE, sizeof(ARR_TYPE));
        for(int i=0; str[i] != '\0'; i++) {
            const ARR_TYPE t = str[i] - '0';
            g_array_append_val(row, t);
            const ARR_TYPE v0 = 0;
            g_array_append_val(srow, v0);
        }
        g_ptr_array_insert(grid, -1, row);
        g_ptr_array_insert(sgrid, -1, srow);
        scanf("%100s", str);
    }


    int total = 0;
    for(int i=0; i<100; i++) {
        printf("Iter: %d\n", i);
        print_grid(grid);
        print_grid(sgrid);
        total += iterate_grid(grid, sgrid);
    }
    printf("Final:\n");
    print_grid(grid);
    print_grid(sgrid);

    printf("Tot\n");
    printf("%d\n", total);

    return 0;
}
