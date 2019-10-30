#pragma once

#include <stdbool.h>

// CHECK(A) checks that A evaluates to true.
#define CHECK(A)           really_check((A),#A,__FILE__,__LINE__)

// CHECK_INT(A, B) checks that A and B evaluate to the same int.
#define CHECK_INT(A,B)     really_check_int((A),(B),#A,#B,__FILE__,__LINE__)

// CHECK_CHAR(A, B) checks that A and B evaluate to the same char.
#define CHECK_CHAR(A,B)    really_check_char((A),(B),#A,#B,__FILE__,__LINE__)

// CHECK_STRING(A, B) checks that A and B evaluate to the same string
// (as a char*).
#define CHECK_STRING(A,B)  really_check_string((A),(B),#A,#B,__FILE__,__LINE__)

// Helper function used by `CHECK` macro above.
void really_check(
        bool condition,
        const char* assertion,
        const char* file,
        int line);

// Helper function used by `CHECK_INT` macro above.
void really_check_int(
        long have,
        long want,
        const char* expr_have,
        const char* expr_want,
        const char* file,
        int line);

// Helper function used by `CHECK_STRING` macro above.
void really_check_char(
        char have,
        char want,
        const char* expr_have,
        const char* expr_want,
        const char* file,
        int line);

// Helper function used by `CHECK_STRING` macro above.
void really_check_string(
        const char* have,
        const char* want,
        const char* expr_have,
        const char* expr_want,
        const char* file,
        int line);

