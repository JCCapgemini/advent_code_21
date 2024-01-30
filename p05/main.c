#define _GNU_SOURCE

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<glib.h>
#include<assert.h>

struct Position {
    int posx;
    int posy;
};

guint position_hash(const void* v) {
    const struct Position* const val = v;
    guint res = g_int_hash(&val->posx);
    res ^= g_int_hash(&val->posy);
    return res;
}

gboolean position_equals(const void* pa, const void* pb) {
    struct Position const *const a = pa;
    struct Position const *const b = pb;
    if (a->posx != b->posx)
        return FALSE;
    if (a->posy != b->posy)
        return FALSE;
    return TRUE;
}

struct Line {
    struct Position start;
    struct Position dest;
};

void increment_or_insert(GHashTable* ht, struct Position* p){
    int* res = g_hash_table_lookup(ht, p);
    if (res != NULL) {
        (*res)++;
    } else {
        struct Position* np = malloc(sizeof(struct Position));
        np->posx = p->posx;
        np->posy = p->posy;
        res = malloc(sizeof(int));
        *res = 1;
        g_hash_table_insert(ht, np, res);
    }
}

#define STR_MAX 100
int main(int argc, char** argv) {
    GArray* line_array = g_array_new(FALSE, FALSE, sizeof(struct Line));
    char str[STR_MAX] = "\0";
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        printf("Line: %s", str);
        char** splits = g_strsplit(str, " -> ", 2);
        //printf("Split 1: %s -- %s\n", splits[0], splits[1]);
        char** t_splits_s = g_strsplit(splits[0], ",", 2);
        //printf("TSplits %s -- %s\n", t_splits_s[0], t_splits_s[1]);
        char** t_splits_d = g_strsplit(splits[1], ",", 2);
        //printf("TSplitd: %s -- %s\n", t_splits_d[0], t_splits_d[1]);
        struct Line l = {
            .start = { .posx = atoi(t_splits_s[0]), .posy = atoi(t_splits_s[1])},
            .dest = { .posx = atoi(t_splits_d[0]), .posy = atoi(t_splits_d[1])}
        };
        g_array_append_val(line_array, l);

        fgets(str, STR_MAX, stdin);
    }

    GHashTable* ht = g_hash_table_new(position_hash, position_equals);
    printf("\nProcessed:\n");
    for (int i=0; i<line_array->len; i++) {
        struct Line l = g_array_index(line_array, struct Line, i);
        if ((l.start.posx - l.dest.posx) != 0) {
            if(l.start.posy != l.dest.posy) {
                continue;
            }
            int start = MIN(l.start.posx, l.dest.posx);
            int dest = MAX(l.start.posx, l.dest.posx);
            for(int i=start; i<=dest; i++) {
                struct Position p = {.posx = i, .posy = l.start.posy};
                increment_or_insert(ht, &p);
            }
        } else {
            int start = MIN(l.start.posy, l.dest.posy);
            int dest = MAX(l.start.posy, l.dest.posy);
            for(int i=start; i<=dest; i++) {
                struct Position p = {.posx = l.start.posx, .posy = i};
                increment_or_insert(ht, &p);
            }
        }
        printf("Sx %d Sy %d -> Dx %d Dy %d\n", l.start.posx, l.start.posy, l.dest.posx, l.dest.posy);
    }

    int totals = 0;
    printf("\nValues:\n");
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init (&iter, ht);
    while (g_hash_table_iter_next (&iter, &key, &value)) {
        struct Position* p = key;
        int val = *((int*)value);
        //printf("Pos: x %d y %d Hits: %d\n", p->posx, p->posy, val);
        if (val>1) {
            totals++;
        }
    }
    printf("%d\n", totals);
    return 0;
}
