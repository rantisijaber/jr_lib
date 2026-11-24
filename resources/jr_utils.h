#ifndef JR_UTILS_H
#define JR_UTILS_H

#include <cstdio>
#include <cstdlib>
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

#define KiB(amount) ((usz)(amount) << 10)
#define MiB(amount) (KiB(amount) << 10)
#define GiB(amount) (MiB(amount) << 10)



#define UNUSED(x) void(x)
#define ALIGNMENT sizeof(void *) * 2

#define SV_FORMAT(jr_string_view) "%.*s", (jr_string_view).size, (const char *)(jr_string_view).data


#define ERROR_CHECK(condition, message, action)                                              \
    do {                                                                                     \
        if (condition) {                                                                     \
            fprintf(stderr, "Error: %s {\n\tFile -> %s\n\tLine -> %d\n\tFunctin -> %s\n}\n", \
                        message, __FILE__, __LINE__, __func__);                              \
            action                                                                           \
        }                                                                                    \
    } while (0)                                                                              \


#define WARNING_CHECK(condition, message, action)                                              \
    do {                                                                                       \
        if (condition) {                                                                       \
            fprintf(stderr, "Warning: %s {\n\tFile -> %s\n\tLine ->%d\n\tFunction -> %s\n}\n", \
                    message, __FILE__, __LINE__, __func__);                                    \
            action                                                                             \
        }                                                                                      \
                                                                                               \
    }                                                                                          \



#define make_allocator(kind, capacity)              \
    (jr_allocator) {                                \
        .internal_allocator = make_##T##(capacity), \
        .allocate = T##_allocate,                   \
        .destroy = T##_destroy,                     \
        .reset = T##_reset                          \
    };                                              \


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

#define SV_LITERAL(s) (jr_string_view) { \
    .data = (const u8*)(s),         \
    .size = sizeof(s) - 1                \
}


typedef struct {
    const u8* data;
    usz size;
} jr_string_view;


jr_string_view to_sv(const char* cstring);
char* to_cstring(jr_string_view* sv);
void sv_chop_left(jr_string_view* sv);
void sv_chop_right(jr_string_view* sv);
void sv_trim(jr_string_view sv);
bool sv_equals(jr_string_view sv1, jr_string_view sv2);


typedef struct {
    void* instance;

    usz(*get_size)(void* instance);
    void(*append)(void* instance, void* element);
    void*(*get)(void* instance, usz index);
    void(*destroy)(void* instance);
} jr_list;

#define make_list(kind, T, NAME, capacity)                \
    (jr_list) {                                           \
            .instance = make_##NAME##_##kind##(capacity), \
            .get_size = NAME##_##kind##_size,             \
            .append = NAME##_##kind##_append,             \
            .get = NAME##_##kind##_get,                   \
            .destroy = NAME##_##kind##_destroy            \
        }

#define GEN_ARRAYLIST_OF(T, NAME)                                             \
    typedef struct {                                                          \
        T* data;                                                              \
        usz size;                                                             \
        usz capacity;                                                         \
    } NAME##_arraylist;                                                       \
                                                                              \
    NAME##_arraylist* make_##NAME##_arraylist(usz capacity) {                 \
        NAME##_arraylist* out =                                               \
            (NAME##_arraylist*)malloc(sizeof(NAME##_arraylist));              \
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
    usz NAME##_arraylist_size(void* list) {                                   \
        return (NAME##_arraylist*(list))->size;                               \
    }                                                                         \
                                                                              \
    void NAME##_arraylist_append(void* list, void* element) {                 \
        NAME##_arraylist* actual_list = (NAME##_arraylist *) list;            \
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
    void* NAME##_arraylist_get(void* list, usz index) {                       \
        NAME##_arraylist* actual_list = (NAME##_arraylist *) list;            \
        if (index >= actual_list->size) {                                     \
            fprintf("Error: Accessing invalid index of list");                \
            exit(1);                                                          \
        }                                                                     \
                                                                              \
        return actual_list->data[index];                                      \
    }                                                                         \
                                                                              \
    void NAME##_arraylist_destroy(void* list) {                               \
        NAME##_arraylist* actual_list = (NAME##_arraylist*)list;              \
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


void for_each_single(jr_list* list, jr_consumer consumer); 
void for_each_bi(jr_list* list, void* secondary, jr_consumer consumer);
bool any_match_single(jr_list* list, jr_predicate predicate);
bool any_match_bi(jr_list* list, void* secondary, jr_predicate predicate);
bool all_match_single(jr_list* list, jr_predicate predicate);
bool all_match_bi(jr_list* list, void* secondary, jr_predicate predicate);



typedef struct {
    void* instance;

    void(*push)(void* instance, void* element);
    void*(*peek)(void* instance);
    void*(*pop)(void* instance);
    bool(*is_empty)(void* instance);
} jr_stack;

                                                                                                   \
#define GEN_FSTACK_OF(T, NAME)                                                                     \
    typedef struct {                                                                               \
        T* data;                                                                                   \
        usz pos;                                                                                   \
        usz capacity;                                                                              \
    } NAME##_fstack;                                                                               \
                                                                                                   \
    NAME##_fstack make_##NAME##_fstack(usz init_capacity) {                                        \
        return (NAME##_fstack) {                                                                   \
            .data = (T *) malloc(sizeof(T) * init_capacity),                                       \
            .pos = 0,                                                                              \
            .capacitry = init_capacity                                                             \
        };                                                                                         \
    }                                                                                              \
    void NAME##_fstack_push(void* instance, void* element) {                                       \
        NAME##_fstack* stack = (NAME##_fstack *) instance;                                         \
        T casted_element = *(T *) element;                                                         \
        WARNING_CHECK(stack->pos == stack->capacity, "Cannot push to stack, stack full", return;); \
        stack->data[stack->pos++] = casted_element;                                                \
                                                                                                   \
    }                                                                                              \
                                                                                                   \
    void* NAME##_fstack_peek(void* instance) {                                                     \
        NAME##_fstack stack = (NAME##_fstack *) instance;                                          \
        WARNING_CHECK(stack->pos == 0, "Cannot peek, stack is empty", return NULL;);               \
        return stack->data[stack->pos];                                                            \
    }                                                                                              \
                                                                                                   \
    void* NAME##_fstack_pop(void* instance) {                                                      \
        NAME##_fstack stack = (NAME##_fstack *) instance;                                          \
        WARNING_CHECK(stack->pos == 0, "Cannot peek, stack is empty", return NULL;);               \
        return stack->data[stack->pos--];                                                          \
    }                                                                                              \
                                                                                                   \
    bool NAME##_fstack_is_empty(void* instance) {                                                  \
        return ((NAME##_fstack *) instance)->pos == 0;                                             \
    }                                                                                              \

#define GEN_RSTACK_OF(T, NAME)                                                             \
    typedef struct {                                                                       \
        T* data;                                                                           \
        usz pos;                                                                           \
        usz capacity;                                                                      \
    } NAME##_rstack;                                                                       \
                                                                                           \
                                                                                           \
    NAME##_rstack make_##NAME##_rstack(usz init_capacity) {                                \
        return (NAME##_rstack) {                                                           \
            .data = (T *) malloc(sizeof(T) * init_capacity),                               \
            .pos = 0,                                                                      \
            .capacitry = init_capacity                                                     \
        };                                                                                 \
    }                                                                                      \
                                                                                           \
    void NAME##_rstack_push(void* instance, void* element) {                               \
        NAME##_rstack* stack = (NAME##_rstack *) instance;                                 \
        T casted_element = *(T *) element;                                                 \
        if (stack->pos == stack->capacity) {                                               \
            void* temp = realloc(stack->data, sizeof(T) * stack->capacity * 2);            \
            ERROR_CHECK(temp == NULL, "Could not realloc for rstack", exit(EXIT_FAILURE)); \
            stack->data = temp;                                                            \
            stack->capacity *= 2;                                                          \
        }                                                                                  \
        stack->data[stack->pos++] = casted_element;                                        \
                                                                                           \
    }                                                                                      \
                                                                                           \
    void* NAME##_rstack_peek(void* instance) {                                             \
        NAME##_rstack stack = (NAME##_rstack *) instance;                                  \
        WARNING_CHECK(stack->pos == 0, "Cannot peek, stack is empty", return NULL;);       \
        return stack->data[stack->pos];                                                    \
    }                                                                                      \
                                                                                           \
    void* NAME##_rstack_pop(void* instance) {                                              \
        NAME##_rstack stack = (NAME##_rstack *) instance;                                  \
        WARNING_CHECK(stack->pos == 0, "Cannot peek, stack is empty", return NULL;);       \
        return stack->data[stack->pos--];                                                  \
    }                                                                                      \
                                                                                           \
    bool NAME##_rstack_is_empty(void* instance) {                                          \
        return ((NAME##_rstack *) instance)->pos == 0;                                     \
    }                                                                                      \
                                                                                           \

#define make_stack(kind, NAME, capacity)            \
    (jr_stack) {                                       \
        .instance =  make_##NAME##_##kind##(capacity), \
        .push = NAME##_##kind##_push,                  \
        .pop = NAME##_##kind##_pop,                    \
        .peek = NAME##_##kind##_peek,                  \
        .is_empty = NAME##_##kind##_is_empty           \
}

#endif
