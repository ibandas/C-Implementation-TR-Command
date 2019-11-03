#include "translate.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// Helper function for interpreting escapes. Given the character that
// follows a backslash, returns the character that the combination
// should be replaced with. Recognizes the seven standard C escapes, and
// maps every other character to itself.
char interpret_escape(char c)
{
    switch (c) {
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'v': return '\v';
    default:  return c;
    }
}

// TODO: Add conditional for escapes
size_t charset_length(const char* src)
{
    size_t result = 0;
    const char* p = &src[0];
    const char* start;
    const char* end;
    while(*p != '\0'){
        if (*(p+1) == '-' && *(p+2) != '\0') {
            start = *p;
            end = *(p+2);
            if (start < end) {
                result = result + (end - start + 1);
            }
            else {
                result = result + 3;
            }
            p = p+3;
        }
        else {
            ++p;
            result++;
        }
    }

    return result;
}


char* expand_charset(const char* src)
{
    char* result = malloc(charset_length(src) + 1);
    char* dst    = result;

    if (result == NULL) return NULL;

    // TODO: Your code goes here

    *dst = '\0';

    return result;
}

char translate_char(char c, const char* from, const char* to)
{
    // TODO: Your code goes here

    return c;
}

void translate(char* s, const char* from, const char* to)
{
    // TODO: Your code goes here
}

