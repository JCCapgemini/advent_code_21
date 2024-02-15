#include "pq.h"
#include<stdio.h>
#include<math.h>
#include<assert.h>

struct pq* pq_new(char (*cmp)(void*, void*)) {
    struct pq* mpq = malloc(sizeof(struct pq));
    mpq->vector_storage = g_ptr_array_new();
    mpq->cmp = cmp;
    return mpq;
}

void pq_swap(GPtrArray* parray, int pnode, int cnode) {
    void** ptr_pnode = &g_ptr_array_index(parray, pnode);
    void** ptr_cnode = &g_ptr_array_index(parray, cnode);
    void* ptr_tnode = *ptr_pnode;
    *ptr_pnode = *ptr_cnode;
    *ptr_cnode = ptr_tnode;
}

int pq_parent(int pos) {
    return floor((((float)pos-1))/2);
}

void pq_push(struct pq* pq, void* new_element) {
    GPtrArray* parray = pq->vector_storage;
    int idx = pq->vector_storage->len;
    int pidx = -1;
    g_ptr_array_insert(pq->vector_storage, -1, new_element);
    if (pq->vector_storage->len > 1) {
        printf("Pos %d\n", idx);
        pidx = pq_parent(idx);
        assert(idx >= 0);
        assert(pidx >= 0);
        void* pnode = g_ptr_array_index(parray, pidx);
        void* cnode = g_ptr_array_index(parray, idx);
        // Swap while in range
        while (pidx >= 0 && pq->cmp(pnode, cnode) > 0) {
            pq_swap(parray, pidx, idx);
            idx = pidx;
            assert(idx >= 0);
            assert(pidx >= 0);
            pidx = pq_parent(idx);
            pnode = g_ptr_array_index(parray, pidx);
            cnode = g_ptr_array_index(parray, idx);
        }
    }
    printf("PIdx: %d CIdx: %d\n", pidx, idx);


}

int pq_left_child(int pos) {
    return pos*2+1;
}

int pq_right_child(int pos) {
    return pos*2+2;
}

void* pq_pop(struct pq* pq) {
    void* result = NULL;
    GPtrArray* parray = pq->vector_storage;
    if(parray->len > 1) {
        result = g_ptr_array_index(parray, 0);
        pq_swap(parray, 0, parray->len-1);
        g_ptr_array_remove_index(parray, parray->len-1);
        const int len = parray->len;

        int node = 0;
        int left_child = pq_left_child(node);
        int right_child = pq_right_child(node);

        void* current_node = g_ptr_array_index(parray, node);
        void* left_child_node = g_ptr_array_index(parray, left_child);
        void* right_child_node = g_ptr_array_index(parray, right_child);

        while(left_child < len && right_child < len) {
            char left_child_cmp = pq->cmp(current_node, left_child_node);
            char right_child_cmp = pq->cmp(current_node, right_child_node);
            if(!(left_child_cmp > 0 || right_child_cmp > 0)) {
                break;
            }
            if (pq->cmp(left_child_node, right_child_node) < 0) {
                pq_swap(parray, node, left_child);
                node = left_child;
            } else {
                pq_swap(parray, node, right_child);
                node = right_child;
            }

            left_child = pq_left_child(node);
            right_child = pq_right_child(node);

            current_node = g_ptr_array_index(parray, node);
            left_child_node = g_ptr_array_index(parray, left_child);
            right_child_node = g_ptr_array_index(parray, right_child);
        }

        if  (left_child >= len && right_child >= len)  {
        } else if (left_child < len && pq->cmp(current_node, left_child_node)) {
            char left_child_cmp = pq->cmp(current_node, left_child_node);
            if (left_child_cmp>0) {
                pq_swap(parray, node, left_child);
                node = left_child;
            }
        } else if (right_child < len && pq->cmp(current_node, right_child_node)) {
            char right_child_cmp = pq->cmp(current_node, right_child_node);
            if (right_child_cmp>0) {
                pq_swap(parray, node, right_child);
                node = right_child;
            }
        }
    } else if(parray->len == 1) {
        result = g_ptr_array_index(parray, 0);
        g_ptr_array_remove_index(parray, 0);
    }
    return result;
}
