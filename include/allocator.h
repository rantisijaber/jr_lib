#ifndef JRLIB_ALLOCATOR_H
#define JRLIB_ALLOCATOR_H

#include "utils.h"



typedef struct mem_allocator {
    void* context;
    void*(*allocate)(void* context, usz size, usz alignment);
    void(*free)(void* context);
} mem_allocator;


mem_allocator make_arena(usz capacity);
void allocator_destroy(mem_allocator* allocator);
// mem_allocator make_dyarena();

#endif 
