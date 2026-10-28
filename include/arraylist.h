#ifndef JRLIB_ARRAYLIST
#define JRLIB_ARRAYLIST

#include <stdint.h>
#include <stddef.h>

#if defined(ARRAYLIST_USE_NAIVE) && defined(ARRAYLIST_USE_PROPER)
# error "Cannot define both kinds of lists"
#elif !defined(ARRAYLIST_USE_NAIVE) && !defined(ARRAYLIST_USE_PROPER)
# define ARRAYLIST_USE_PROPER
#endif

#ifdef ARRAYLIST_USE_NAIVE

typedef struct {
    size_t length;
    size_t capacity;
    size_t item_size;
    void* data;
} array_list;





#else

typedef struct {

} array_list;

#endif

#endif

