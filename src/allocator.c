#include "allocator.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
// mem_allocator make_arena(usz capacity) {}




#define IS_POWER2(x) (( ((x) != 0)) && ( ((x) & ((x) - 1)) == 0))

uptr align_forward(uptr ptr, usz alignment) {
    if (!IS_POWER2(alignment)) return 0;
    uptr ptr_alignment = (uptr)alignment;
    uptr rem = ptr & (ptr_alignment - 1);
    if (rem) {
        ptr += (ptr_alignment - rem);
    }

    return ptr;

}

typedef struct {
    u8* data;
    usz pos;
    usz capacity;
} jr_arena;

jr_arena* arena_init(usz capacity);
void* arena_alloc(void* arena, usz size, usz alignemnt);
void arena_free(void* arena);

mem_allocator make_arena(usz capacity) {
    jr_arena* jr_arena = arena_init(capacity);
    return (mem_allocator) {
        .context = jr_arena,
        .allocate = arena_alloc,
        .free = arena_free
    };
}


jr_arena* arena_init(usz capacity) {
    jr_arena* arena = (jr_arena*)malloc(sizeof(jr_arena));
    arena->data = (u8*)malloc(capacity * sizeof(u8));
    arena->pos = 0;
    arena->capacity = capacity;
    return arena;
}

void* arena_alloc_aligned(jr_arena* arena, usz size, usz alignment) {
    uptr current = (uptr)arena->data + (uptr)arena->pos;
    uptr offset = align_forward(current, alignment);
    offset -= (uptr)arena->data;

    if (offset + size > arena->capacity) {
        return NULL;
    }

    arena->pos = offset + size;
    return (void*)arena->data + offset;
}

void* arena_alloc(void* arena, usz size, usz alignment) {
    jr_arena* casted_arena = (jr_arena*)arena;
    void* value = arena_alloc_aligned(casted_arena, size, alignment);
    if (value == NULL) {
        fprintf(stderr, "Error: Could not allocate more data on arena. Fixed sized arena full\n");
        exit(1);
    }
    return value;
}

void arena_free(void* arena) {
    jr_arena* casted_arena = (jr_arena*)arena;
    free(casted_arena->data);
    free(casted_arena);
}
