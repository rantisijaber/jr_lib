#include "jr_utils.h"
#include <stdint.h>
#include <stdlib.h>


uintptr_t align_forward(uintptr_t ptr, usz align) {
    uintptr_t result = ptr & ((uintptr_t) align - 1);
    if (result != 0) {
        return ptr + (uintptr_t) align - result;
    }
    
    return ptr;
}


jr_arena* make_arena(usz capacity) {
    jr_arena* out = (jr_arena *)malloc(sizeof(jr_arena));
    out->data = (u8 *)malloc(capacity);
    out->capacity = capacity;
    out->offset = 0;
    return out; 
}

void* arena_allocate(void* arena, usz size) {
    jr_arena* casted_arena = (jr_arena *) arena;
    uintptr_t curr = (uintptr_t) casted_arena->data + (uintptr_t) casted_arena->offset;
    uintptr_t n_offset = align_forward(curr, ALIGNMENT) - curr;

    if (n_offset + size <= casted_arena->capacity) {
        casted_arena->offset = n_offset + size;
        return &casted_arena->data[n_offset];
    }

    return NULL;
}

void arena_destroy(void* arena) {
    jr_arena* casted_arena = (jr_arena *) arena;
    free(casted_arena->data);
    casted_arena->capacity = 0;
    casted_arena->offset = 0;
}

void arena_reset(void* arena) {
    jr_arena* casted_arena = (jr_arena *) arena;
    casted_arena->offset = 0;
}


void sv_chop_left(jr_string_view* sv) {
    if (sv->size == 0) return;
    sv->data++;
    sv->size--;
}

void sv_chop_right(jr_string_view* sv) {
    if (sv->size == 0) return;
    sv->size--;
}

jr_string_view to_sv(const char* cstring) {
    return (jr_string_view) {
        .data = (const u8 *) cstring,
        .size = strlen(cstring)
    };
}

char* to_cstring(jr_string_view* sv) {
    char* result = malloc(sv->size + 1);
    if (result == NULL) return NULL;
    
    memcpy(result, (char *) sv->data, sv->size);
    result[sv->size] = '\0';
    return result;    
}

bool sv_equals(jr_string_view sv1, jr_string_view sv2) {
    if (sv1.size != sv2.size) return false;
    if (sv1.size == 0) return true;
    
    return memcmp(sv1.data, sv2.data, sv1.size) == 0;
}


void for_each_single(jr_list* list, jr_consumer consumer) {
    usz size = list->get_size(list->instance);
    for (usz i = 0; i < size; i++) {
        consumer.consumer(list->get(list->instance, i));
    }
}


void for_each_bi(jr_list* list, void* secondary, jr_consumer consumer) {
    usz size = list->get_size(list->instance);
    for (usz i = 0; i < size; i++) {
        consumer.biconsumer(list->get(list->instance, i), secondary);
    }
}

bool any_match_single(jr_list* list, jr_predicate predicate) {
    usz size = list->get_size(list->instance);
    for (usz i = 0; i < size; i++) {
        if (predicate.predicate(list->get(list->instance, i))) return true;
    }
    return false;
}

bool any_match_bi(jr_list* list, void* secondary, jr_predicate predicate) {
    usz size = list->get_size(list->instance);
    for (usz i = 0; i < size; i++) {
        if (predicate.bipredicate(list->get(list->instance, i), secondary)) return true;
    }
    return false;
}
bool all_match_single(jr_list* list, jr_predicate predicate) {
    usz size = list->get_size(list->instance);
    for (usz i = 0; i < size; i++) {
        if (!predicate.predicate(list->get(list->instance, i))) return false;
    }
    return true; 
}
bool all_match_bi(jr_list* list, void* secondary, jr_predicate predicate) {
    usz size = list->get_size(list->instance);
    for (usz i = 0; i < size; i++) {
        if (!predicate.bipredicate(list->get(list->instance, i), secondary)) return false;
    }
    return true;    
}
