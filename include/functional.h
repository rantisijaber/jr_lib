#ifndef JRLIB_FUNCTIONAL_H
#define JRLIB_FUNCTIONAL_H

#include <stdbool.h>
#include "collections.h"
#include "utils.h"


#define predicate_lam(return_type, pre_name, element, pre_body) \
    return_type pre_name (element) pre_body 



typedef bool(*predicate_fn)(void* element);
typedef void(*consumer_fn)(void* element);
typedef void*(*supplier_fn)(void);
typedef void*(*function_fn)(void* element);

bool invoke_predicate(predicate_fn predicate, void* element);
void invoke_consumer(consumer_fn consumer, void* element);
void* invoke_supplier(supplier_fn supplier);
void* invoke_function(function_fn function, void* element);


#define invoke(x, y) _Generic((x),     \
    predicate_fn: invoke_predicate, \
    consumer_fn: invoke_consumer,   \
    supplier_fn: invoke_supplier,   \
    function_fn: invoke_function)(x, y)



typedef struct { predicate_fn pred; } jr_predicate;

typedef struct { consumer_fn cons; } jr_consumer;

typedef struct { supplier_fn sup; } jr_supplier;

typedef struct { function_fn func; } jr_function;



bool any_match(jr_collections* collection, jr_predicate predicate); 
bool all_match(jr_collections* collection, jr_predicate predicate);


void remove_if(jr_collections* collection, jr_predicate predicate);

void* for_each(jr_collections* collection, jr_function function);

#endif
