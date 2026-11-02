#ifndef JRLIB_LIST_H
#define JRLIB_LIST_H

#include "utils.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#define INLINE(T, x) (&(T){ (x) }) 

#define create_arraylist_type(T) \
    typedef struct { T* data; jr_allocator* allocator; usz length; usz capacity; } T##_arraylist;       \
    void T##_arraylist_append(void* arraylist, void* value) {                                           \
        T actual_value = *(T*)value;                                                                    \
        T##_arraylist* casted_list = (T##_arraylist*)arraylist;                                         \
        if (casted_list->length == casted_list->capacity) {                                             \
            if ((casted_list->allocator)->context_kind != DYNAMIC_ARENA) {                              \
                fprintf(stderr, "Error: To ensure list is dynamic, only use dynamic allocators");       \
                exit(1);                                                                                \
            }                                                                                           \
            void* temp = (casted_list->allocator)->allocate((casted_list->allocator)->context,          \
                                               sizeof(T) * casted_list->capacity * 2, _Alignof(T));     \
            if (temp == NULL) {                                                                         \
                fprintf(stderr, "Error: Could not expand list");                                        \
                exit(1);                                                                                \
            }                                                                                           \
            memcpy(temp, casted_list->data, casted_list->length * sizeof(T));                           \
            casted_list->data = temp;                                                                   \
        }                                                                                               \
                                                                                                        \
                                                                                                        \
        casted_list->data[casted_list->length] = actual_value;                                          \
        casted_list->length++;                                                                          \
        casted_list->capacity *= 2;                                                                     \
    }                                                                                                   \
    void* T##_arraylist_get(void* arraylist, usz index) {                                               \
        T##_arraylist* casted_list = (T##_arraylist*)arraylist;                                         \
        if (index > casted_list->length - 1) return NULL;                                               \
        return &casted_list->data[index];                                                               \
    }                                                                                                   \
    usz T##_arraylist_size(void* arraylist) {                                                           \
        T##_arraylist* casted_list = (T##_arraylist*)arraylist;                                         \
        return casted_list->length                                                                      \
    jr_list make_##T##_arraylist(jr_allocator* allocator) {                                             \
        void* temp_list = (allocator)->allocate((allocator)->context,                                   \
                                                sizeof(T##_arraylist), _Alignof(T##_arraylist));        \
        if (temp_list == NULL) {                                                                        \
            fprintf(stderr, "Error: Could not create arraylist");                                       \
            exit(1);                                                                                    \
        }                                                                                               \
        T##_arraylist* list = (T##_arraylist*)temp_list;                                                \
        void* temp_data = (allocator)->allocate((allocator)->context,                                   \
                                                sizeof(T) * 64, _Alignof(T));                           \
        list->data = temp_data;                                                                         \
        list->allocator = allocator;                                                                    \
        list->length = 0;                                                                               \
        list->capacity = 64;                                                                            \
        return (jr_list) { .context = list, .size = T##_arraylist_size, .get = T##_arraylist_get, .append = T##_arraylist_append }; \
    }                                                                                                   \
    }                                                                                                   \
    // void T##_arraylist_destroy(void* arraylist) {
    //     T##_arraylist* list = (T##_arraylist*)arraylist;
    //     list->allocator->
    // }
#define make_arraylist(T, allocator)                                                                    \
    make_##T##_arraylist(allocator)


typedef struct {
    void* context;
    usz(*size)(void* context);  
    void(*append)(void* context, void* value);
    void*(*get)(void* context, usz index);
    void(*destroy)(void* context);
} jr_list;

#endif
