#include "functional.h"
#include "collection.h"





bool invoke_predicate(predicate_fn predicate, void* element) {
    return predicate(element);
}
void invoke_consumer(consumer_fn consumer, void* element) {
    return consumer(element);
}
void* invoke_supplier(supplier_fn supplier) {
    return supplier();
}
void* invoke_function(function_fn function, void* element) {
    return function(element);
}


// #define    

bool any_match(jr_collections* collection, jr_predicate predicate) {
    switch (collection->kind) {
        default: {
            usz length = collection->wrappers.list->size(collection->wrappers.list);
            for (usz i = 0; i < length; i++) {
                if (invoke(predicate.pred, 
                        collection->wrappers
                        .list->get(collection->wrappers.list, i))) {
                    return true; 
                }
            }
            break;
        }
    }
    return false;
}

bool all_match(jr_collections* collection, jr_predicate predicate) { 
    switch (collection->kind) {
        default: {
            usz length = collection->wrappers.list->size(collection->wrappers.list);
            for (usz i = 0; i < length; i++) {
                if (!invoke(predicate.pred, 
                        collection->wrappers
                        .list->get(collection->wrappers.list, i))) {
                    return false; 
                }
            }
            break;
        }
    }
    return true;
}



