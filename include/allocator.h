#ifndef JRLIB_ALLOCATOR_H
#define JRLIB_ALLOCATOR_H

#include "utils.h"



typedef struct mem_allocator mem_allocator;

mem_allocator make_arena(usz capacity, usz elem_size);
void allocator_destroy(mem_allocator* allocator);
mem_allocator make_dyarena();

#endif 
