#include <unistd.h>
#include "malloc.h"

#define LEAST_HEAP (1<<16)
#define MIN_ALLOC_SIZE (1<<4)
#define my_malloc(size) ((heap_begin == NULL) ? heap_init_and_malloc(size) : _malloc(size))


/* globals */
static void *heap_begin = NULL;
static struct node *head = NULL;
static struct node *tail = NULL;

/* declarations */
void *_my_malloc(size_t);
void *heap_init_and_malloc(size_t);
int grow_heap(size_t);

////

void *heap_init_and_malloc(size_t size) {
    heap_begin = sbrk(0); if (sbrk(LEAST_HEAP) == (void *) - 1) return NULL; head = heap_begin; 
    head->size = (LEAST_HEAP - sizeof(struct node)); //subtract the size of the node struct cause we can't allocate that
    head->next = NULL;
    head->is_used = false;

    tail = head;

    return _my_malloc(size);

}


/**
* traverse the linked list of nodes to find one that's big enough for the malloc request
* if none is available, grows the heap and try again
* if the node found is big enough, it is split in two nodes, one of which is allocated
*
* returns the pointer to the allocated memory, or NULL if something's wrong
*/
void *_my_malloc(size_t size) {
    struct node *n;
    n = head;

    while ( n != NULL || n->is_used || (n->size < size)) {
         n = n->next;
    }

    if (n == NULL) {
        if (grow_heap(size) == 1) {
             return NULL;
        } else {
            return _my_malloc(size);
        }
    }

    //we split the node if the resulting new node would be bigger than MIN_ALLOC_SIZE
    if (!n->is_used && n->size > size + sizeof(struct node) + MIN_ALLOC_SIZE) {
        struct node *new_node;
        new_node = n + size + sizeof(n);
        new_node->next = n->next;
        new_node->is_used = false;
        new_node->size = (n->size - size - sizeof(struct node));
        n->next = new_node;
        n->is_used = true;

        if (tail == n) {
            tail = new_node;
        }

        return n + sizeof(struct node);
    } else {
        n->is_used = true;
        return n + sizeof(struct node);
    }
}


/**
* grow the heap of at least size, usually much more
*
* return 1 on failure, 0 on success;
*/
int grow_heap(size_t size) {
    struct node *new;
    size_t grow_size;
    if (size < LEAST_HEAP) {
        grow_size = LEAST_HEAP;
    } else {
        grow_size = size<<4;
    }

    void *n = sbrk(grow_size);
    if (n == (void *) -1) {
        return 1;
    } 

    new = n;
    new->size = grow_size - sizeof(struct node);
    new->next = NULL;
    new->is_used = false;

    tail->next = new;
    tail = new;

    return 0;

}








