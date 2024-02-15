#include<limits.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<ctype.h>
#include<glib.h>
#include<assert.h>

#include "pq.h"

#define STR_MAX 110
#define ARR_TYPE uint64_t
#define ARR_TYPE_CHR char 

void print_grid(GPtrArray* grid) {
    // Print grid!
    printf("Grid:\n");
    for(int i=0; i<grid->len; i++) {
        GArray* row = g_ptr_array_index(grid, i);
        for(int j=0; j<row->len; j++) {
            printf("%d ", g_array_index(row, ARR_TYPE_CHR, j));
        }
        printf("\n");
    }
}

struct Position {
    int row;
    int col;
};

struct DijstraPosition {
    struct Position p;
    uint64_t cost;
};

char compare_struct_dijstra_position( void* a , void* b )
{
    const struct DijstraPosition* ai = ( const struct DijstraPosition* )a;
    const struct DijstraPosition* bi = ( const struct DijstraPosition* )b;

    if( ai->cost < bi->cost )
    {
        return -1;
    }
    else if( ai->cost > bi->cost )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int dijstra_position_equals(const void* lr, const void* rr) {
    const struct Position* l = &((const struct DijstraPosition*) lr)->p;
    const struct Position* r = &((const struct DijstraPosition*) rr)->p;
    int res = 0;
    if (l->row == r->row) {
        if (l->col == r->col) {
            res = 1;
        }
    }
    //printf("Equials R: %d\n", res);
    return res;
}

unsigned int dijstra_position_hash(const void *pr) {
    const struct Position* p = &((const struct DijstraPosition*) pr)->p;
    unsigned int res = p->row<<8;
    res |= p->col;
    //printf("Hash R: %d C: %d H: %d\n", p->row, p->col, res);
    return res;
}

GArray* grid_position_index_row(GPtrArray* grid, uint64_t row) {
    return g_ptr_array_index(grid, row);
}

ARR_TYPE_CHR grid_position_index(GPtrArray* grid, struct Position* p) {
    GArray* row = g_ptr_array_index(grid, p->row);
    return g_array_index(row, ARR_TYPE_CHR, p->col);
}

uint64_t dijstra(GPtrArray* grid) {
    struct pq* mpq = pq_new(compare_struct_dijstra_position);
    GHashTable* seen = g_hash_table_new(dijstra_position_hash, dijstra_position_equals);

    const uint64_t row_len = grid->len -1;
    const uint64_t col_len = grid_position_index_row(grid, 0)->len -1;

    // Add the two initial positions
    struct DijstraPosition* p = malloc(sizeof(struct DijstraPosition));
    p->p.row = 0;
    p->p.col = 1;
    p->cost = grid_position_index(grid, &p->p);
    pq_push(mpq, p);
    p = malloc(sizeof(struct DijstraPosition));
    p->p.row = 1;
    p->p.col = 0;
    p->cost = grid_position_index(grid, &p->p);
    pq_push(mpq, p);

    while (mpq->vector_storage->len > 0) {
        p = (struct DijstraPosition*) pq_pop(mpq);
        printf("Check: R: %d C: %d\n", p->p.row, p->p.col);
        // Remove element if already seen
        if (g_hash_table_contains(seen, &p->p)) {
            free(p);
            printf("Contained!\n");
            continue;
        }
        printf("Position: R: %d C: %d Cost: %lld\n", p->p.row, p->p.col, p->cost);

        // Reached the destination!
        if (p->p.row == row_len && p->p.col == col_len) {
            break;
        }

        // If can go down
        if (p->p.row < row_len) {
            struct DijstraPosition* tp = malloc(sizeof(struct DijstraPosition));
            tp->p.row = p->p.row + 1;
            tp->p.col = p->p.col;
            tp->cost = grid_position_index(grid, &tp->p) + p->cost;
            pq_push(mpq, tp);
        }

        // If can go right 
        if (p->p.col < col_len) {
            struct DijstraPosition* tp = malloc(sizeof(struct DijstraPosition));
            tp->p.row = p->p.row;
            tp->p.col = p->p.col + 1;
            tp->cost = grid_position_index(grid, &tp->p) + p->cost;
            pq_push(mpq, tp);
        }

        // Add element to seen set
        g_hash_table_add(seen, &p->p);
    }
    return p->cost;
}

int main(int argc, char** argv) {
    GPtrArray* grid = g_ptr_array_new();
    char str[STR_MAX] = "\0";
    char* istr = str;
    // Read all the grid
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        GArray* row = g_array_new(FALSE, FALSE, sizeof(ARR_TYPE_CHR));
        printf("IT: %s", str);
        istr = str-1;
        while(*(++istr) != '\n') {
            const char nv = *istr - '0';
            g_array_append_val(row, nv);
        }
        g_ptr_array_insert(grid, -1, row);
        fgets(str, STR_MAX, stdin);
    }

    print_grid(grid);

    uint64_t total = dijstra(grid);
    printf("Total\n");
    printf("%lld\n", total);

    return 0;
}
