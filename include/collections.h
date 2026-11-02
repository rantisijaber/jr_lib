#ifndef JRLIB_COLLECTIONS_H
#define JRLIB_COLLECTIONS_H

#include "list.h"


typedef enum {
    MAP, LIST
} collection_kind;

typedef struct {
    collection_kind kind;
    union {
        jr_list* list;
        // whatever is next
    } wrappers;
    void*(*get_element)(void* context);
} jr_collections;







#endif
