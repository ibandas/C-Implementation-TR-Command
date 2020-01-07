#include "translate.h"

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

size_t charset_length(const char* src)
{
    size_t result = 0;
    char* start;
    char* end;
    while(*src != '\0'){
        if (*(src+1) == '-' ) {
            if ( *(src+2) != '\0'){
                start = *src;
                end = *(src+2);
                if (start <= end) {
                    result = result + (end - start + 1);
                }
                src = src+3;
            }
            else{
                src = src + 2;
            }

        }
        else if (*src == '\\'){
            if ( *(src+1) != '\0'){
                result++;
                src = src+2;
            }
            else{
                src++;
            }
        }
        else {
            src++;
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

    int start, end;

    while(*src != '\0'){
        if (*(src+1) == '-') {
            if (*(src+2) != '\0'){
                start = *src;
                end = *(src+2);
                while (start <= end) {
                    *dst = (char)start;
                    dst++; start++;
                }
                src = src+3;
            }
            else{
                src = src + 2;
            }
        }
        else if (*src == '\\'){
            if (*(src+1) != '\0'){
                src++;
                *dst = interpret_escape(*src);
                dst = dst + 1;
            }
            src++;
        }
        else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    return result;
}

char translate_char(char c, const char* from, const char* to)
{
    size_t index = 0;
    while(*from != '\0'){
        if (*from == c){
            return to[index];
        }
        ++index;
        ++from;
    }
    return c;
}

void translate(char* s, const char* from, const char* to)
{
    size_t i = 0;
    while(s[i] != '\0') {
        s[i] = translate_char(s[i], from, to);
        i++;
    }
}

