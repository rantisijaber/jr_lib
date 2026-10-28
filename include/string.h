#ifndef JRLIB_STRING
#define JRLIB_STRING

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include "utils.h"

#define JR_STR_FMT(str) (int)(str).length, (str).data;
#define JR_STR_LITERAL(value) ((string) { .data = (value), .length = sizeof(value)}

typedef struct {
    usz length;
    u8* data;
} string;


typedef struct {
    usz length;
    const u8* data;
} string_view;

string* string_create(usz length);
void string_init(string* str, usz length);
void string_resize(string* str);
char* to_cstring(string_view string);
string concat(string_view sv1, string_view sv2);
string_view to_sv(string str);




#endif
