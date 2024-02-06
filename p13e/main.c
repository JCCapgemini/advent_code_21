#include<assert.h>
#include<ctype.h>
#include<glib.h>
#include<limits.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>

#define STR_MAX 100
#define ARR_TYPE struct Position 
#define ARR_TYPE2 struct Fold 

struct Fold {
    char x_fold;
    int64_t val;
};

struct Position {
    int64_t x;
    int64_t y;
};

void setPositionX(struct Position* p, int64_t v) {
    p->x = v;
}

void setPositionY(struct Position* p, int64_t v) {
    p->y = v;
}

int64_t getPositionX(const struct Position* p) {
    return p->x;
}

int64_t getPositionY(const struct Position* p) {
    return p->y;
}

gint cmpPosition (gconstpointer a, gconstpointer b) {
    struct Position const * pa = a;
    struct Position const * pb = b;

    if (pa->x == pb->x) {
        if (pa->y < pb->y) return -1;
        else if (pa->y > pb->y) return 1;
    } else {
        if (pa->x < pb->x) return -1;
        else if (pa->x > pb->x) return 1;
    }
    return 0;
}

void remove_duplicates(GArray* points) {
    g_array_sort(points, cmpPosition);
    for(int i=1; i<points->len; i++) {
        struct Position* pb = &g_array_index(points, ARR_TYPE, i-1);
        struct Position* p = &g_array_index(points, ARR_TYPE, i);

        if (cmpPosition(pb, p) == 0) {
            g_array_remove_index(points, i);
            i--;
        }
    }
}

void execute_fold(GArray* points, struct Fold* f) {
    int64_t (*get) (const struct Position*) = NULL;
    void (*set) (struct Position*, int64_t) = NULL;
    if (f->x_fold) {
        get = &getPositionX;
        set = &setPositionX;
    } else {
        get = &getPositionY;
        set = &setPositionY;
    }

    for(int i=0; i<points->len; i++) {
        struct Position* p = &g_array_index(points, ARR_TYPE, i);
        int64_t v = get(p);
        //printf("OV - Point n: %d v: %lld\n", i, v);
        assert(v != f->val);
        if (v > f->val) {
            // Distance, we know positive
            int64_t d = v - f->val;
            // From fold we move distance negative
            set(p, f->val - d);
        }
        v = get(p);
        //printf("DV - Point n: %d v: %lld\n", i, v);
    }

    remove_duplicates(points);
}
int main(int argc, char** argv) {
    GArray* points = g_array_new(FALSE, FALSE, sizeof(ARR_TYPE));
    GArray* folds = g_array_new(FALSE, FALSE, sizeof(ARR_TYPE2));
    char str[STR_MAX] = "\0";

    // Read points!
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        printf("%s", str);
        if(str[0] == '\n') break;
        int i=0; while(str[++i] != ',');
        str[i] = '\0';
        struct Position p = {
            .x = atoi(str),
            .y = atoi(&str[i+1]) 
        };
        g_array_append_val(points, p);
        fgets(str, STR_MAX, stdin);
    }

    g_array_sort(points, cmpPosition);
    for(int i=0; i<points->len; i++){
        struct Position* p = &g_array_index(points, ARR_TYPE, i);
        printf("Point n: %d x: %lld y: %lld\n", i, p->x, p->y);
    }
    printf("\n\n");

    // Read folds!
    fgets(str, STR_MAX, stdin);
    while (!feof(stdin)) {
        printf("%s", &str[11]);
        struct Fold f = {
            .x_fold = str[11] == 'x' ? 1 : 0,
            .val = atoi(&str[13])
        };
        g_array_append_val(folds, f);
        fgets(str, STR_MAX, stdin);
    }

    for(int i=0; i<folds->len; i++){
        struct Fold* f = &g_array_index(folds, ARR_TYPE2, i);
        printf("Fold n: %d x: %d val: %lld\n", i, f->x_fold, f->val);
    }

    // Main logic
    printf("\n\nMain logic!\n\n");
    printf("Points: %d\n", points->len);
    for(int i=0; i<folds->len; i++) {
        execute_fold(points, &g_array_index(folds, ARR_TYPE2, i));
        printf("Points: %d\n", points->len);
    }

    // Display final status
    g_array_sort(points, cmpPosition);
    printf("Point list:\n");
    for(int i=0; i<points->len; i++){
        struct Position* p = &g_array_index(points, ARR_TYPE, i);
        printf("Point x: %lld y: %lld\n", p->x, p->y);
    }
    printf("Letters:\n");
    struct Position* last = &g_array_index(points, ARR_TYPE, points->len-1);
    const int max_width = last->x + 1;
    int64_t max_height = 0;
    for(int i=0; i<points->len; i++){
        int64_t cur = g_array_index(points, ARR_TYPE, i).y;
        if (cur>max_height) max_height = cur;
    }
    char* res_str = malloc(sizeof(char) * (max_width+1));
    res_str[max_width] = '\0';
    for(int j=0; j<=max_height; j++) {
        for(int i=0; i<max_width;i++) res_str[i] = ' ';
        for(int i=0; i<points->len; i++){
            struct Position* p = &g_array_index(points, ARR_TYPE, i);
            if (p->y == j) res_str[p->x] = '#';
        }
        printf("%s\n", res_str);
    }
    free(res_str);
    printf("%d\n", points->len);
    return 0;
}
