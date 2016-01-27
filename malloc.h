#ifndef MALLOC_H_INCLUDED
#define MALLOC_H_INCLUDED
#include <stddef.h>
#define true 1
#define false 0


#define my_malloc(size) (_is_malloc_init() ? heap_init_and_malloc(size) : _my_malloc(size))

struct node {
    size_t size;
    struct node *next;
    _Bool is_used;
};

/* declarations */
_Bool _is_malloc_init();
void *_my_malloc(size_t);
void *heap_init_and_malloc(size_t);
#endif /* MALLOC_H_INCLUDED */
