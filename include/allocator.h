#ifndef JRLIB_ALLOCATOR_H
#define JRLIB_ALLOCATOR_H

#include "utils.h"

typedef enum { DYNAMIC_ARENA, FIXED_ARENA } allocator_kind;

typedef struct jr_allocator {
    allocator_kind context_kind;
    void* context;
    void*(*allocate)(void* context, usz size, usz alignment);
    void(*reset)(void* context);
    void(*destroy)(void* context);
} jr_allocator;


jr_allocator make_arena(usz capacity);
jr_allocator make_dyarena(void);

#endif 
