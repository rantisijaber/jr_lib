#ifndef JR_UTILS_H
#define JR_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef size_t usz;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define KiB(amount) ((usz)amount << 10)
#define MiB(amount) (KiB(amount) << 10)
#define GiB(amount) (MiB(amount) << 10)

typedef struct {
    void* internal_allocator;

    void*(*allocate)(void* internal_allocator, usz size);
    void(*destroy)(void* internal_allocator);
    void(*reset)(void* internal_allocator);
} jr_allocator;



typedef struct {
    u8* data;
    usz offset;
    usz capacity;
} jr_arena;


typedef struct {
    char* data;
    usz size;
} jr_string;

jr_string to_string(const char* cstring);
void string_set_at(char c, usz index);


typedef struct {
    const u8* data;
    usz size;
} jr_string_view;


jr_string_view to_sv(const char* cstring);
char* to_cstring(jr_string_view* sv);
void sv_chop_left(jr_string_view* sv);
void sv_chop_right(jr_string_view* sv);
void sv_trim(jr_string_view sv);



typedef struct {
    void* instance;

    usz(*get_size)(void* instance);
    void(*append)(void* instance, void* element);
    void*(*get)(void* instance, usz index);
    void(*destroy)(void* instance);
} jr_list;

#define make_list(kind, T, capacity)                   \
    (jr_list) {                                        \
            .instance = make_##T##_##kind##(capacity), \
            .get_size = T##_##kind##_size,             \
            .append = T##_##kind##_append,             \
            .get = T##_##kind##_get,                   \
            .destroy = T##_##kind##_destroy            \
        }                                              \


#define GEN_ARRAYLIST_OF(T)                                                   \
    typedef struct {                                                          \
        T* data;                                                              \
        usz size;                                                             \
        usz capacity;                                                         \
    } T##_arraylist;                                                          \
                                                                              \
    T##_arraylist* make_##T##_arraylist(usz capacity) {                       \
        T##_arraylist* out =                                                  \
            (T##_arraylist*)malloc(sizeof(T##_arraylist));                    \
        if (MAX_SIZE / sizeof(T) > capacity) {                                \
            fprintf(stderr, "Error: List too large");                         \
            exit(1);                                                          \
        }                                                                     \
        void* temp = malloc(sizeof(T) * capacity);                            \
        if (temp == NULL) {                                                   \
            fprintf(stderr, "Error: Could not allocate memory for new list"); \
            exit(1);                                                          \
                                                                              \
        }                                                                     \
        out->data = temp;                                                     \
        out->capacity = capacity;                                             \
        out->size = 0;                                                        \
        return out;                                                           \
    }                                                                         \
                                                                              \
    usz T##_arraylist_size(void* list) {                                      \
        return (T##_arraylist*(list))->size;                                  \
    }                                                                         \
                                                                              \
    void T##_arraylist_append(void* list, void* element) {                    \
        T##_arraylist* actual_list = (T##_arraylist*)list;                    \
        if (actual_list->size == actual_list->capacity) {                     \
            usz resized_capacity = actual_list->capacity * 2;                 \
            if (MAX_SIZE / sizeof(T) < resized_capacity) {                    \
                fprintf(stderr, "Error: List too large");                     \
                exit(1);                                                      \
            }                                                                 \
            void* temp = realloc(actual_list->data, resized_capacity);        \
            if (temp == NULL) {                                               \
                fprintf(stderr, "Error: Could not resize list");              \
                exit(1);                                                      \
            }                                                                 \
                                                                              \
            actual_list->data = temp;                                         \
            actual_list->capacity *= 2;                                       \
        }                                                                     \
        actual_list->data[actual_list->size++] = *(T*)element;                \
    }                                                                         \
                                                                              \
    void* T##_arraylist_get(void* list, usz index) {                          \
        T##_arraylist* actual_list = (T##_arraylist*)list;                    \
        if (index >= actual_list->size) {                                     \
            fprintf("Error: Accessing invalid index of list");                \
            exit(1);                                                          \
        }                                                                     \
                                                                              \
        return actual_list->data[index];                                      \
    }                                                                         \
                                                                              \
    void T##_arraylist_destroy(void* list) {                                  \
        T##_arraylist* actual_list = (T##_arraylist*)list;                    \
        free(actual_list->data);                                              \
        actual_list->size = 0;                                                \
        actual_list->capacity = 0;                                            \
        free(actual_list);                                                    \
    }                                                                         \


//functional stuff
typedef bool(*predicate_bare)(void* element);
typedef void(*consumer_bare)(void* element);
typedef void*(*function_bare)(void* element);
typedef void*(*supplier_bare)(void);


typedef bool(*bipredicate_bare)(void* element1, void* element2);
typedef void(*biconsumer_bare)(void* element1, void* element2);
typedef void*(*bifunction_bare)(void* element1, void* element2);


typedef struct {
    union {
        predicate_bare predicate;
        bipredicate_bare bipredicate;
    };
} jr_predicate;

typedef struct {
    union {
        consumer_bare consumer;
        bipredicate_bare biconsumer;
    };
} jr_consumer;

typedef struct {
    union {
        function_bare function;
        bifunction_bare bifunction;
    };
} jr_function;

typedef struct {
    supplier_bare supplier;
} jr_supplier;


void for_each_single(void* element, jr_consumer consumer); 
void for_each_bi(void* element1, void* element2, jr_consumer consumer);
bool any_match_single(void* element, jr_predicate predicate);
bool any_match_bi(void* element1, void* element2, jr_predicate predicate);
bool all_match_single(void* element, jr_predicate predicate);
bool all_match_bi(void* element1, void* element2, jr_predicate predicate);

#endif
