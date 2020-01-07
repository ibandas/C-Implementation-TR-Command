#include "translate.h"
#include "libipd.h"

#include <stdlib.h>
#include <string.h>

/////
///// HELPER FUNCTION SIGNATURES
///// (definitions are at the bottom of the file)
/////

// Clones a string into fresh, `malloc`ed memory. Some C implementations
// provide a function `strdup` to do this, but it's non-standard. This
// is useful because `translate` modifies its first argument in-place,
// but we write our tests using string literals, which cannot be
// modified.
static char* clone_string(const char* src);

// Calls `expand_charset`, but asserts that the result is non-NULL.
// This is useful because dereferencing NULL is undefined behavior,
// and calling `expand_charset_checked` instead of `expand_charset`
// means we can avoid checking the result each time in our tests.
static char* expand_charset_checked(const char* input);


/////
///// TEST CASES FOR charset_length
/////

static void test_charset_length()
{
    CHECK_INT( charset_length("abc"), 3 );
    CHECK_INT( charset_length("a-z"), 26 );
    CHECK_INT( charset_length("a-"), 0 );
    CHECK_INT( charset_length("b-a"), 0 );
    CHECK_INT( charset_length("a-cd\\"), 4);
}


/////
///// TEST CASES FOR expand_charset
/////

// Runs an expand_charset() test case. Asserts that
// `expand_charset(input)` will return `expected`.
static void assert_expand(const char* input, const char* expected)
{
    char* actual = expand_charset_checked(input);
    CHECK_STRING( actual, expected );
    free(actual);
}

static void test_expand_charset()
{
    assert_expand( "abc", "abc" );
    assert_expand( "a-e", "abcde" );
    assert_expand( "a-d\\b", "abcd\b" );
    assert_expand( "af-b", "a" );
    assert_expand( "ab-", "a" );
    assert_expand( "a\\", "a" );
}


/////
///// TEST CASES FOR translate_char
/////

static void test_translate_char()
{
    CHECK_CHAR( translate_char('a', "xyz", "XYZ"), 'a' );
    CHECK_CHAR( translate_char('y', "xyz", "XYZ"), 'Y' );
    CHECK_CHAR( translate_char('x', "xyz", "XYZ"), 'X' );
    CHECK_CHAR( translate_char('z', "xyz", "XYZ"), 'Z' );
}


/////
///// TEST CASES FOR translate
/////

// Runs an translate() test case. Asserts that `translate(input, from0,
// to0, expected)` changes `input` to `expected`. (But uses a copy of
// `input`, since `input` might be a string literal, and changing those
// is UB.)
static void assert_tr(const char* input,
                      const char* from0,
                      const char* to0,
                      const char* expected)
{
    char* from   = expand_charset_checked(from0);
    char* to     = expand_charset_checked(to0);
    char* actual = clone_string(input);

    translate(actual, from, to);
    CHECK_STRING( actual, expected );

    free(from);
    free(to);
    free(actual);
}

void test_translate()
{
    assert_tr( "Hello, world!", ",! ",    "___",    "Hello__world_" );
    assert_tr( "hELLO, WORLD!", "a-zA-Z", "A-Za-z", "Hello, world!" );
    assert_tr( "Hello, world!", "a-e", "f-j", "Hjllo, worli!" );
}


/////
///// main
/////

int main()
{
    test_translate_char();
    test_charset_length();
    test_expand_charset();
    test_translate();
}


/////
///// Helper function definitions
/////

static char* clone_string(const char* src)
{
    size_t size = strlen(src) + 1;

    char* dst = malloc(strlen(src) + 1);
    if ( ! dst ) {
        perror("clone_string");
        exit(2);
    }

    memcpy(dst, src, size);

    return dst;
}

static char* expand_charset_checked(const char* input)
{
    char* res = expand_charset(input);
    if ( ! res ) {
        perror("expand_charset");
        exit(3);
    }

    return res;
}

