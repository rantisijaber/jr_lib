#include "list.h"

#define INTERNAL_arraylist(T) typedef struct { (T)* data, usz length, usz capacity } (T)_arraylist;



