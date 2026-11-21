#include "jr_utils.h"
#include <stdlib.h>

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
        .data = (const u8*)cstring,
        .size = strlen(cstring)
    };
}

char* to_cstring(jr_string_view* sv) {
    char* result = malloc(sv->size + 1);
    if (result == NULL) return NULL;
    
    memcpy(result, (char*)sv->data, sv->size);
    result[sv->size] = '\0';
    return result;    
}



