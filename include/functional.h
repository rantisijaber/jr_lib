#ifndef JRLIB_FUNCTIONAL_H
#define JRLIB_FUNCTIONAL_H

 

typedef struct {
    union {
        void*(*map)(void* context);
        void*(*filter)(bool(*predicate)());
    } fps;
} transformation;










#endif
