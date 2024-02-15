#ifndef PQ_C
#define PQ_C

#include<stdint.h>
#include<glib.h>

struct pq {
    GPtrArray* vector_storage;
    char (*cmp)(void*, void*);
};

struct pq* pq_new(char (*cmp)(void*, void*));
void pq_push(struct pq* pq, void* new_element);
void* pq_pop(struct pq* pq);

#endif
