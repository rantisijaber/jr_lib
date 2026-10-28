#include "allocator.h"
#include <stdlib.h>
// mem_allocator make_arena(usz capacity) {}

struct mem_allocator {
    void* context;
    void(*allocate)(void* context);
    void(*free)(void* context);
} ;


typedef struct {
    u8* data;
    usz pos;
    usz capacity;
    usz elem_size;
} jr_arena;

jr_arena* arena_init(usz capacity, usz elem_size);
void arena_alloc(void* arena);
void arena_free(void* arena);

mem_allocator make_arena(usz capacity, usz elem_size) {
    jr_arena* jr_arena = arena_init(capacity, elem_size);
    return (mem_allocator) {
        .context = jr_arena,
        .allocate = arena_alloc,
        .free = arena_free
    };
}


jr_arena* arena_init(usz capacity, usz elem_size) {
    jr_arena* arena = (jr_arena*)malloc(sizeof(jr_arena));
    arena->data = (u8*)malloc(capacity * elem_size);
    arena->pos = 0;
    arena->capacity = capacity;
    arena->elem_size = elem_size;
    return arena;
}

void arena_alloc(void* arena) {
    jr_arena* casted_arena = (jr_arena*)arena;
}

void arena_free(void* arena) {
    jr_arena* casted_arena = (jr_arena*)arena;
    free(casted_arena->data);
    free(casted_arena);
}
