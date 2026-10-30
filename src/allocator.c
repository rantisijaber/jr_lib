#include "allocator.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
// jr_allocator make_arena(usz capacity) {}

#define IS_POWER2(x) (( ((x) != 0)) && ( ((x) & ((x) - 1)) == 0))
#define DYNAMIC_ARENA           0



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
    allocator_kind kind;
    u8* data;
    usz pos;
    usz capacity;
} jr_arena;

jr_arena* arena_init(usz capacity);
void* arena_alloc(void* arena, usz size, usz alignemnt);
void arena_free(void* arena);
void arena_reset(void* arena);



jr_arena* arena_init(usz capacity) {
    jr_arena* arena = (jr_arena*)malloc(sizeof(jr_arena));
    if (capacity == DYNAMIC_ARENA) {
        arena->data = (u8*)malloc(128 * sizeof(u8));
        arena->capacity = 128;
    } else {
        arena->data = (u8*)malloc(capacity * sizeof(u8));
        arena->capacity = capacity;
    }
    arena->pos = 0;
    return arena;
}

void* arena_alloc_aligned(jr_arena* arena, usz size, usz alignment) {
    uptr current = (uptr)arena->data + (uptr)arena->pos;
    uptr offset = align_forward(current, alignment);
    offset -= (uptr)arena->data;
    
    uptr needed_cap = offset + size;
    if (needed_cap > arena->capacity) {
        if (arena->kind == DYNAMIC_ARENA) {
            while (arena->capacity < needed_cap) arena->capacity *= 2;
            void* temp = realloc(arena->data, arena->capacity);
            if (temp == NULL) {
                fprintf(stderr, "Error: Could not reallocate mem\n");
                exit(1);
            }
            arena->data = temp;
            return arena->data + offset;
        }
        return NULL;
    }

    arena->pos = offset + size;
    return arena->data + offset;
}

void* arena_alloc(void* arena, usz size, usz alignment) {
    jr_arena* casted_arena = (jr_arena*)arena;
    void* value = arena_alloc_aligned(casted_arena, size, alignment);
    if (value == NULL) {
        fprintf(stderr, "Error: Could not allocate more memory on arena. Fixed sized arena full\n");
        exit(1);
    }
    return value;
}

void arena_reset(void* arena) {
    ((jr_arena*)arena)->pos = 0;
}

void arena_destroy(void* arena) {
    jr_arena* casted_arena = (jr_arena*)arena;
    free(casted_arena->data);
    free(casted_arena);
}


jr_allocator make_dyarena(void) {
    jr_arena* arena = arena_init(DYNAMIC_ARENA);
    return (jr_allocator) {
        .context = arena,
        .context_kind = arena->kind,
        .allocate = arena_alloc,
        .destroy = arena_destroy,
        .reset = arena_reset
    };
}

jr_allocator make_arena(usz capacity) {
    jr_arena* jr_arena = arena_init(capacity);
    return (jr_allocator) {
        .context_kind = jr_arena->kind, 
        .context = jr_arena,
        .allocate = arena_alloc,
        .destroy = arena_destroy,
        .reset = arena_reset
    };
}

