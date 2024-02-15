#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<assert.h>
#include "pq.h"

struct MyPotato {
    float t1;
    float t2;
    uint64_t p;
};

char compareMyPotato(void* vl, void* vr) {
    struct MyPotato *l, *r;
    assert(vl != NULL);
    assert(vr != NULL);
    l = (struct MyPotato*) vl;
    r = (struct MyPotato*) vr;
    assert(l != NULL);
    assert(r != NULL);
    //printf("L: %lld R: %lld\n", l->p, r->p);
    char result = 0;
    if (l->p < r->p) {
        result = -1;
    } else if (l->p > r->p) {
        result = 1;
    }
    return result;
}

void print_ptr_vec(GPtrArray* parray) {
    printf("Array:");
    for(int i=0; i<parray->len; i++) {
        struct MyPotato* t = (struct MyPotato*) g_ptr_array_index(parray, i);
        printf(" %lld", t->p);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    struct pq* mpq = pq_new(compareMyPotato);
    srand(0);
    printf("Hello!\n");
    for(int i = 1000; i>0; i--) {
        struct MyPotato* p = malloc(sizeof(struct MyPotato));
        assert(p != NULL);
        p->p = rand();
        pq_push(mpq, (void*)p);
        printf("Pushed: %lld\n", p->p);
    }

    struct MyPotato* t = NULL;
    struct MyPotato* tl = NULL;
    print_ptr_vec(mpq->vector_storage);
    tl = pq_pop(mpq);
    t = pq_pop(mpq);
    printf("\n\nPoppings!\n");
    do {
        printf("Ordered pop: %lld\n", t->p);
        assert(compareMyPotato(tl, t) <=0);
        tl = t;
        t = pq_pop(mpq);
    } while(t != NULL);
}
