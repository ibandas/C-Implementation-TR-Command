# IPD Homework 5: `tr`

## Purpose

The goal of this assignment is to get you programming in C with strings,
iteration, and dynamic memory allocation.


## Background

In this project, you will implement a clone of the standard Unix utility
[*tr*(1)](https://linux.die.net/man/1/tr), which is a *filter* that
performs transliteration. Given two equal-sized sequences of characters,
*from* and *to*, it transforms its input by replacing all occurrences of
characters appearing in *from* with the character in the corresponding
position in *to*. For example, we can use `tr` to replace each
occurrence of he letter *e* with the letter *a* in the message “Hello,
world!”:

```sh
$ echo 'Hello, world!' | ./tr e a
Hallo, world!
$
```

> *Note:* A **filter program** copies data from its **standard input stream** to its **standard output stream** while modifying the data in some way. For example the standard Unix command *grep*(1) prints only lines from its input that match a given pattern; *head*(1), given a number *n*, copies only the first *n* lines of its input and discards the rest.
>
> > *Note on note:* Command-line programs receive input in two different
> > ways:
> >
> >   - You can type a small number of strings after the name of the program when you start it. These **command-line arguments** are often used to refer to some files or give initial instructions. For *grep* it’s how you provide the pattern to match; for *head* it’s how you tell it how many lines you want.
> >
> >   - You can type a large amount of text into the program’s standard input while it’s running. You can also redirect the contents of a file or the output of another program into a program’s standard input rather than typing it yourself.
> >
> > Command-line programs provide their results by printing on two output streams, called standard output and **standard error,** and by returning a single integer result code when they exit. A result code of 0 means that the program was successful, and all other numbers mean it failed—different numbers let you distinguish different errors. Ordinarily both output streams are printed directly to your console, but you can also save them to files (separately or together), suppress them (one or both), or send them to other programs as input.

The *tr* program takes the *from* and *to* sets as command-line
arguments. Then it reads text from its standard input a line at a time
and writes each transformed line to its standard output.

In the simplest case, *from* and *to* are strings of the same length. We can use the `echo` command to generate one line of text and then use the shell’s pipe operator (`|`) to send that line to our `tr` program:

```sh
$ echo 'Hello, world!' | ./tr elo 310
H3110, w0r1d!
$ echo 'Hello, world!' | ./tr ',! ' ___
Hello__world_
$
```

> *Note:* Characters that have special meaning for the shell, such as
space, `!`, `*`, `?`, `$`, and `\`, need to be quoted in command-line
arguments. The rules altogether are complicated, but so long as you don’t need an actual ‘`'`’ character *in* your argument then you can reliably quote
with that. The rules for quoting with ‘`"`’ are more complicated, since
some characters are still special inside. You can also quote any single
character (except between `'`s) by preceding it with a single backslash
(`\`).


*Tr* also understands ranges of characters and some backslash escape
sequences:

```sh
$ echo 'Hello, world!' | ./tr a-z A-Z
HELLO, WORLD!
$ echo 'Hello, world!' | ./tr 'a-zA-Z!' 'A-Za-z?'
hELLO, WORLD?
$ alias rot13='./tr a-zA-Z n-za-mN-ZA-M'
$ echo 'Hello, world!' | rot13
Uryyb, jbeyq!
$ echo 'Hello, world!' | rot13 | rot13
Hello, world!
$ echo 'Hello, world!' | ./tr ' ' '\n'
Hello,
world!
$
```

> *Note:* The shell command `alias` lets you define a shorter name for a longer command. (If your shell is *tcsh*(1) then the syntax for `alias` is different, and you’ll need to omit the equals sign and quotation mark: `alias rot13 ./tr a-zA-Z n-za-mN-ZA-M`)


## Setup and orientation

Before you start, you will need to follow these [instructions for
setting up your development environment][setup].

You should also carefully read the [IPD Style Manual for
C and C++][style]. Every programming language has its own conventions
so when you start programming in a new language then you have to adapt
to its style.

Once you have everything set up, use Git to clone your homework
repository; then open the main repo directory that Git created using
CLion.

> *Note:* It’s important that you tell CLion to open the directory that contains `CMakeLists.txt`, rather than some directory above or below that in the directory tree. CLion also knows how to perform Git operations, and you can use its menus instead of the command line to clone, commit, push, pull, etc., if you prefer.

You should see that the starter code includes three `.c` files:

  - Most significant functionality will be defined in the
    “*translate* library,” `src/translate.c`.

  - Tests for those functions will be written in `test/test_translate.c`.

  - The `main()` function that implements the *tr* program will be defined in
    `src/tr.c`.

Function signatures for `src/translate.c` are provided for you in
`src/translate.h`; since we may run tests that interface with your code via
this header file, **you must not modify `src/translate.h` in any way.**
All of your code will be written in the three `.c` files listed above.


## Specifications

The project comprises two functional components, which are specified in
this section. First, though, we define *charsets* (character sets).


### Character sets

The *tr* program uses charsets to specify which characters to replace
and what to replace them with. The C type of a charset is just
`char*`—that is, a C string—but they can be represented in two forms
having different interpretations:

  -  A *literal* charset is just a sequence of characters, each standing
     for itself. For example, interpreted as a literal charset, the
     string `"a-e"` contains the three characters `'a'`, `'-'`, and
     `'e'` at indices 0, 1, and 2, respectively. In a literal charset,
     no character has special meaning.

  - An *unexpanded* charset may contain ranges, written
    <code><var>c</var>-<var>d</var></code>, and escape sequences,
    written <code>\\<var>c</var></code>:

      - The range <code><var>c</var>-<var>d</var></code> stands for the
        interval of characters from <code>'<var>c</var>'</code> to
        <code>'<var>d</var>'</code>, inclusive. (This means that if
        <code>'<var>c</var>' &gt; '<var>d</var>'</code> then the range
        is empty, and if <code>'<var>c</var>' == '<var>d</var>'</code>
        then the range contains only <code>'<var>c</var>'</code>.)

      - If the escape <code>\\<var>c</var></code> is valid [C string
        literal escape
        sequence](https://en.cppreference.com/w/c/language/escape), then
        it has the same meaning for  *tr* as in C; otherwise it just
        stands for character <code>'<var>c</var>'</code> itself. We have
        provided you a function mapping each character
        <code>'<var>c</var>'</code> to the meaning of
        <code>\\<var>c</var></code>, so you don't have to figure that
        part out.

Here is a table showing several unexpanded charsets along with
their literal expansions:

| **unexpanded**             | **literal**             |
| :------------------------- | :---------------------- |
| `"abc"`                    | `"abc"`                 |
| `"a-e"`                    | `"abcde"`               |
| `"a-e_"`                   | `"abcde_"`              |
| `"a-df-i"`                 | `"abcdfghi"`            |
| `"\\t"` (2 characters)     | `"\t"` (1 character)    |

The *tr* program takes charsets in unexpanded form and must expand them
to literal form before it can do its work. It is also important not to
expand more than once since expanding a literal charset may produce
a different charset.


### The *translate* library

The *translate* library is responsible for two main tasks:

  - expanding charsets from unexpanded to literal form, and

  - using a pair of literal charsets to translate a string.

It provides a function for each of these purposes that will be used in
`src/tr.c`. Additionally, the header file exposes two helper functions
to facilitate testing. Thus, `src/translate.c` defines four functions:

  - Function `expand_charset(const char*)` takes a charset in
    unexpanded form and expands it, returning it in literal form.

    The returned charset is allocated by *malloc*(3), which means that
    the caller is responsible for deallocating it with *free*(3) when
    finished with it.

    **Error case:** If `expand_charset()` is unable to allocate
    memory then it returns the special pointer value `NULL`.

  - Function `charset_length(const char*)` is a helper to
    `expand_charset()` that determines how long the literal result of
    expanding its argument will be.

  - Function `translate(char* s, const char* from, const char* to)`
    takes a string to modify (`s`) and two literal charsets (`from` and
    `to`). Each character in string `s` that appears in charset `from`
    is replaced by the character at the same index in charset `to`.

    To be precise: For each index `i` in `s`, if there is some `j` such
    that `s[i] == from[j]` (and there is no `k < j` such
    that `s[i] == from[k]`), then `s[i]` is replaced by `to[j]`.

    **Undefined behavior:**
    Function `translate()` has an *unchecked precondition* whose
    violation will result in undefined behavior. In particular, for it
    to work properly, `from` must not be a longer string than `to`.
    However, `translate()` **should not** check this condition, as
    ensuring it is the caller's responsibility.

  - Function `translate_char(char c, const char* from, const char* to)`
    is a helper to function `translate()`.
    It takes a character to translate (`c`) and two literal charsets
    (`from` and `to`). It returns the translation of character `c` as
    given by the two charsets.

    To be precise: If there is some `j` such that `c == from[j]` (and
    there is no `k < j` such that `c == from[k]`), then this function
    returns `to[j]`; but if there is no such `j` then it returns `c`
    unchanged.

    **Undefined behavior:**
    Function `translate_char()` has the same unchecked precondition
    as function `translate()`, with the same results if violated.
    (This is a natural consequence of `translate()` calling
    `translate_char()`.)


### The *tr* program

The *tr* program must be run with two command-line arguments. If run
with more or fewer than two, it prints the message

<pre>
Usage: <em>${TR}</em> FROM TO < INPUT_FILE
</pre>

to the standard error stream `stderr` (where *${TR}* is replaced by the
value `argv[0]` [the actual name that the program was called with]) and
then exits with error code 1.

The arguments `FROM` (`argv[1]`) and `TO` (`argv[2]`) are
unexpanded charsets, so *tr* must expand them to literal
charsets. If the lengths of the two literal charsets differ
(post-expansion, that is) then it prints the message

<pre>
<em>${TR}</em>: error: lengths of FROM and TO differ
</pre>

to `stderr` (where again *${TR}* is replaced by `argv[0]`) and then exits
with error code 2.

Now that argument checking has succeeded, *tr* begins filtering. For
each line read from the standard input, it translates the line according
to the literal expansions of `FROM` and `TO` and prints the result. When
there is no more input to process then the program terminates
successfully.


## Reference

### Accepting command-line arguments

When running a C program from the command line, the user can supply it
with *command-line arguments*, which the program's `main()` function
then receives as an array of strings. In particular, `main()` can be
declared to accept two function arguments, as follows:

```c
int main(int argc, char* argv[]);
```

Then `argc` will contain the number of command-line arguments (including
the name of the program itself in `argv[0]`), and `argv` will contain
the command line arguments themselves.

For example, if a C program is run like

```sh
$ my_prog foo bar bazzz
```

then `argc` is 4 and `argv` is the array

```c
    {
      "my_prog",
      "foo",
      "bar",
      "bazzz"
    }.
```


### Reading input a line at a time

The C programming language doesn't provide a safe or easy way to read
a line of input whose length is unknown before you read it. So we’ve
provided you a small library, *libipd*, with your Homework 5 starter
code. The `libipd.h` header declares a function `read_line()` that reads
a line from the standard input and returns it.

The `read_line` function returns a character array allocated by
*malloc*(3), which means that the caller is responsible for deallocating
it with *free*(3) when finished with it. See the next subsection for
more on this topic.

> *Note:* C provides *gets*(3), which is easy to use but [inherently unsafe](https://cwe.mitre.org/data/definitions/242.html), and *fgets*(3), which can be used safely but requires you to specify a limit on the length of the line.


### Managing memory with *malloc*(3) and *free*(3)

In BSL and ISL, all memory used by your programs is allocated and
deallocated automatically. But to work with strings in C, especially
strings whose length is not known when the program is written, we need
a different technique.

Function *malloc*() (from `<stdlib.h>`) takes the number of bytes
that you need and attempts to allocate that much memory. For example, we
can allocate enough memory for one `int` or for an array of `N` `int`s:

```c
int* just_one = malloc(sizeof(int));
int* several  = malloc(N * sizeof(int));
```

> *Note:* The result of *malloc*() has type `void*`, which is the type of a pointer whose referent type is unknown. In C (but not C++), `void*` converts automatically to and from any other pointer type.

If *malloc*() succeeds then it returns a pointer to the newly allocated
memory, which can be used to hold any type that fits. The memory this
pointer points to is uninitialized, so you must initialize it to avoid
undefined behavior. When you are done with this memory, you must
free it by passing the pointer to *free*().

> *Note:* Failure to free memory that your program no longer needs can lead to a *memory leak*, which causes your program to use more memory than it should, or even to run out. But worse things can happen than memory leaks: freeing a pointer twice, or dereferencing a pointer that has already been freed, causes undefined behavior.

If *malloc*() fails to find sufficient memory, which is always
a possibility, it returns the special pointer value `NULL`, which is
a valid pointer that points nowhere. Dereferencing `NULL` is undefined
behavior, but you can compare it using the `==` operator. Consequently,
every call to *malloc*() must be followed by a `NULL` check. We provide
this call to *malloc*() and the obligatory `NULL` check in
`src/translate.c`:

```c
char* result = malloc(charset_length(src) + 1);
char* dst    = result;

if (result == NULL) return NULL;
```

Two things to note about the above *malloc*() call:

  - We are allocating one more byte than the length that `src` will
    expand to because we need an extra byte to store the string's
    `'\0'` terminator.

  - There’s no need to multiply the desired number of `char`s by
    `sizeof(char)` because `sizeof(char)` is defined to be `1`.


### Testing, automatically and manually

You will need two techniques for testing your C code:

  - To test your C functions in `src/translate.c`, you’ll write more
    C functions in `test/test_translate.c` that call the functions we
    want to test and check their results. We’ll compile those test
    functions as their own C program whose purpose is just testing,
    and then you’ll run that program frequently.

  - To test your whole C program in `src/tr.c`, you will most likely
    have to run it manually and interactively. This may sound easier and
    more fun than writing test cases in C, but it’s a lot more work than
    writing automated tests, and the automated tests can help you get
    the important functions *right* before running the whole program is
    even possible.

#### Testing automatically

The provided file `test/translate.c` provides an initial setup for
automated testing: a `main` function that calls four other functions,
one for each function you need to test, and several helper functions for
making test cases nice to write. (Never underestimate the value of
making important tasks pleasant and easy!) The actual checks are written
using three “macros” that are provided by *libipd*:

  - <code>CHECK( <em>expr</em> );</code> checks that expression
    <code><em>expr</em></code> evaluates to something truthy (which in
    C includes not only the `bool` value `true`, but also every number but
    0 in every numeric type and all non-`NULL` pointers). It counts
    successes and failures, but even when the check fails the program
    continues with the next statement. (This is true for the other two
    check forms as well.)

  - <code>CHECK_INT( <em>expr1</em>, <em>expr2</em> );</code> evaluates
    expressions <code><em>expr1</em></code> and
    <code><em>expr2</em></code>, which must produce integers, and checks
    that they are equal.
    
    This is better than using <code>CHECK( <em>expr1</em> ==
    <em>expr2</em> );</code> because it prints both integers when the
    check fails.

  - <code>CHECK_STRING( <em>expr1</em>, <em>expr2</em> );</code>
    evaluates expressions <code><em>expr1</em></code> and
    <code><em>expr2</em></code>, which must produce C-style
    `'\0'`-terminated strings, and checks that they are equal.
    
    This is better than using <code>CHECK( strcmp(<em>expr1</em>,
    <em>expr2</em>) == 0 );</code> because it prints both strings when
    the check fails. It’s better than using <code>CHECK( <em>expr1</em>
    == <em>expr2</em> );</code> because it actually works. (See the next
    section for more on what’s wrong with comparing C strings using the
    `==` operator.)

To build and run your automated tests, select `test_translate` on the
list of *run configurations* in the CLion toolbar; then click the green,
triangular “play” button to its right.

These tests will also be run by GitHub Actions, our continuous
integration service, whenever you push your code to GitHub.

#### Testing manually

To test your main program in `src/tr.c`, you’ll need to build it using
CLion and then run it interactively in a terminal. 

 1. To build it, select `tr` from the list of run configurations on the
    CLion toolbar; then click the build button (the green hammer) to its
    left.

 2. To run it, first open a terminal in your project directory. The
    easiest way is to use CLion’s built-in terminal from the *View* >
    *Tool Windows* > *Terminal* menu. CLion/CMake will put your
    compiled program in its build directory, which by default is named
    `cmake-build-debug`, so if you don’t want to type
    `cmake-build-debug/tr` repeatedly, you should first change into that
    directory:
    
    ```sh
    [~/ipd/hw05]$ cd cmake-build-debug
    [~/ipd/hw05/cmake-build-debug]$ ./tr
    Usage: ./tr FROM TO < INPUT_FILE
    [~/ipd/hw05/cmake-build-debug]$ echo hallo | tr al ip
    hippo
    [~/ipd/hw05/cmake-build-debug]$ echo hallo | tr all ip
    ./tr: error: lengths of FROM and TO differ
    [~/ipd/hw05/cmake-build-debug]$
    ```

Note that the examples above and in the *Background* section involve
sending your `tr` program one line at a time. Be sure to test it
interactively, too, to make sure it handles multiple lines of input
correctly:

```sh
$ build/tr a-z A-Z
Be sure to test
BE SURE TO TEST
your program
YOUR PROGRAM
interactively.
INTERACTIVELY.
^D
$
```

> *Note:* The `^D` above indicates the keystroke *Control-D*, which on
Unix-like systems causes the program to detect end-of-file and exit. On
Windows you may need to press *Control-Z* instead.


### Working with C strings

When testing your functions, you might be tempted to write assertions
like this:

```c
CHECK( expand_charset("a-e") == "abcde" );
```

But there are three problems with this.

First, it leaks memory, because `expand_charset()` allocates memory and the
code above doesn't free it. To fix that, we need to store the result of
`expand_charset()` in a variable, which lets us refer to it twice, first to use
it and second to free it:

```c
char* actual_result = expand_charset("a-e");
CHECK( actual_result == "abcde" );
free(actual_result);
```

However, this still won't work, because when you use `==` to compare pointers
(which strings are in C), it compares *the addresses*, not the pointed-to
values. And the address returned by `expand_charset()` will never be the same
as the address of a string literal.

Instead, to compare strings, we need to use the
[*strcmp*(3)](https://linux.die.net/man/3/strcmp) function (from
`<string.h>`), which compares them character by character. You may
expect that *strcmp*() would return `true` for equal strings and `false`
for unequal strings, but actually it does something more useful:
`strcmp(s1, s2)` determines the lexicographical ordering for `s1` and
`s2`. If `s1` should come before `s2` when sorting then it returns
a negative `int`; if `s1` should come after `s2` then it returns
a positive `int`. If they are equal, it returns 0.

> *Note:* [Lexicographical order](https://en.wikipedia.org/wiki/Lexicographical_order) is a generalization of alphabetical order to sequences of non-letters (or more than just letters). *strcmp*() compares the numeric values of `char`s, which means that `'a' < 'b'` and `'A' < 'B'`, but also `'B' < 'a'` and `'$' < ','`.

Thus we could write:

```c
char* actual_result = expand_charset("a-e");
CHECK( strcmp(actual_result, "abcde") == 0 );
free(actual_result);
```

This almost works! In fact, it usually will work. But to be completely
correct, we need to deal with the possibility that `expand_charset()`
fails to allocate memory and returns `NULL`. In that case, *strcmp*()
will deference `NULL`, which is undefined behavior. Thus, we need to
ensure that `actual_result` is not `NULL` before we try to use the
string that it points to:

```c
char* actual_result = expand_charset("a-e");
assert( actual_result );
CHECK( strcmp(actual_result, "abcde") == 0 );
free(actual_result);
```

> *Note:* When `CHECK` fails it records a failed test but the program keeps going so you can see the results of more tests. So it would be bad to use `CHECK` for the obligatory null check on `expand_charset`’s result. Unlike `CHECK`, when `assert` fails it terminates the whole program, which is a better course of action than undefined behavior.

Finally, if you use `CHECK_STRING` then not only will it call `strcmp`
for you, but it prints the two strings if they differ. So use that:

```c
char* actual_result = expand_charset("a-e");
assert( actual_result );
CHECK_STRING( actual_result, "abcde" );
free(actual_result);
```

Here are some more functions from `<string.h>` that you may find
useful:

  - `char* strchr(const char* s, int c)` searches string `s` for the
    first occurrence of `(char)c`, returning a pointer to the occurrence
    if found or `NULL` if not.
    
    > *Note:* Why does *strchr*() take an `int` rather than a `char`? Many C functions take a character as type `int` for [obscure historical reasons](https://stratadoc.stratus.com/vos/15.1.1/r040-02/wwhelp/wwhimpl/common/html/wwhelp.htm?context=r040-02&file=ch6r040-02o.html).

  - `char* strcpy(char* dst, const char* src)` copies string pointed to
    by `src` into string pointed to by `dst` (which must have sufficient
    capacity, or you'll get UB).

  - `size_t strlen(const char*)` computes the length of a string (not
    including the `'\0'`).


## Hints

In this section, we provide suggestions, such as algorithms, for writing
the necessary functions. These hints are given in what we expect will be
the best order of implementation. It's a very good idea to test each
function as you write it, rather than testing them all at the end,
because you will find bugs sooner that way.

### Algorithm for the `charset_length()` function

The `charset_length()` function scans its argument string (an
unexpanded character set) while counting how many characters it will take
when expanded. Thus,  you need two variables: one to count, and one to
keep track of the position while scanning the string. Start the count at
0 and the position at the beginning of the argument string. Then iterate
and evaluate the following conditions for each iteration:

  - If the character at the current position is `'\0'`, then you've
    reached the end and should return the count.
    
    > *Note:* To scan a string you can use either an index `size_t i` or pointer `char* p`. If you hold onto the original string `s` then the two approaches are interchangeble, since `p == s + i`, or equivalently `i == p - s`

  - If the character at the *next* position is `'-'`, and the character
    at the position after that is not `'\0'`, then you've found a range.
    If we call the character before the hyphen `start` and the character
    after the hyphen `end`, then we can determine the length of the
    range by comparing the two characters: If `start > end` then the
    range is empty; otherwise the length of the range is
    `end - start + 1`. Add this to the count, and then advance the current
    position by 3 to get to the first character past the right side of the
    range.
    
    > *Note:* This implies that a hyphen at the beginning or end of the string, or immediately following the end of a character range, is interpreted literally rather than denoting a range.

   - If the character at the current position is `'\\'` (a single
     backslash) and the character at the next position is not `'\0'`,
     then you have found an escape sequence. Its expanded length is 1,
     so add that much to the count, and advance the current position by
     2 to get to the first character after the escape sequence.

     > *Note*: This case should be checked after the range case, which implies that the literal expansion of unexpanded charset `"\\-_"` is `"\\]^_"`, not `"-_"`.

  - Otherwise, the character at the current position will be copied
    as is, so increment the count by 1 and advance the current
    position to the next character.


### Algorithm for the `expand_charset()` function

Like `charset_length()`, the `expand_charset()` function scans its
argument string (an unexpanded character set), but instead of counting,
it copies the characters into a fresh string, expanding ranges and
escape characters into their literal meanings.

The first thing it must do is allocate memory for its result. We have
provided you code that calls `charset_length()` to find out how much
memory is needed, allocates the memory, and checks that the allocation
succeeded. Then the algorithm works by scanning the argument string
while storing characters into the result string. To do this, you will
likely need three variables: one to remember the start of the result
string in order to return it; one to keep track of your position in the
unexpanded character set being scanned (the source); and one to keep
track of your position in the result string being filled in (the
destination).

> *Note:* This function is probably the trickiest part of the whole homework. One way to develop your code would be to hold off writing this function and move forward, while temporarily considering all input charsets to be literal. It's not hard to add a call to `expand_charset()` to `src/tr.c`’s `main()` function once you get it working.

The control logic of the scanning-and-copying loop is the same as in the
`charset_length()` function, but the actions at each step differ:

  - If the character at the current source position is `'\0'`, then
    you've reached the end. Don't forget to store a `'\0'` at the
    destination position (which should be the end of the result string)
    before returning.

  - If the character at the *next* source position is `'-'`, and the
    character at the position after that is not `'\0'`, then you've
    found a range. If we call the character before the hyphen `start`
    and the character after the hyphen `end`, then we can generate the
    range by iteration, incrementing `start` until it passes `end`. That
    is, so long as `start <= end`, we want to store `start` to the
    destination position, advance the destination position, and
    increment `start`. Once we've fully expanded the range, we advance
    the source position past it (by adding 3).
    
    > *Note:* To avoid undefined behavior here, you should store `start` and `end` as `int`s, not `char`s. To understand why, consider what would happen if `end` were `CHAR_MAX`.

  - If the character at the current source position is `'\\'` and
    the character at the next source position is not `'\0'`, then you
    have found an escape sequence. Its expansion is given by
    `interpret_escape(c)` (provided in `src/translate.c`), where `c` is
    the character following the backlash. Store the expansion to the
    destination position, advance the destination position, and advance
    the source position past the escape sequence (by adding 2).

  - Otherwise, the character at the current position stands for itself,
    so store it at the current destination position and then advance
    both the source and destination positions by 1.
    
    > *Note:* The traditional C way to do this is `*dst++ = *src++;`.
    >
    > > *Sub-note:* If you’re using indices instead of pointers to track your reading and writing positions then it’s a bit less traditional, but you can write `d[j++] = s[i++];`.


### Algorithm for the `translate_char()` function

The `translate_char()` function takes a character to translate (`c`) and
two literal charsets (`from` and `to`). The idea is to scan charset `from`
searching for `c`. If we find `c` at some index `i` then return `to[i]`.
If we get to the end of `from` without finding `c` then return `c`
unchanged.


### Algorithm for the `translate()` function

The `translate()` function takes a string to translate in place (`s`)
and two literal charsets (`from` and `to`). The idea is to iterate
through each position in `s`, replacing each character with its
translation according to `translate_char()`.


### Algorithm for the *tr* program

The *tr* program has three phases: first it validates and interprets its
arguments, then it transforms its input to its output, and then it
cleans up its resources.

We've provided you with the first check, for the correct number of
arguments. This serves as an example of how to use *fprintf*(3) and
*stderr*(4) for printing error messages.

Next, use `expand_charset()` to expand both command-line arguments
`argv[1]` and `argv[2]` into literal charsets. Since `expand_charset()`
returns `NULL` if it cannot allocate memory, you need to `NULL`-check
both results; if it fails, print the error message (using `OOM_MESSAGE`
and `argv[0]`) and exit with error code 2.

> *Note:* Two calls to `expand_charset()` mean you will need exactly two calls to `free()` in order to clean up in the end.

If character set expansion succeeds but the charsets, once expanded,
don't have the same length, it is an error; print the specified error
message (`LENGTH_MESSAGE`) to `stderr` and exit with error code 2.

Now, if there are no errors then we are ready to iterate over the input
lines until `read_line()` returns `NULL`, translating each line and
printing the result. Since each input line read by `read_line()` is
allocated by `malloc()`, you need to free each line with `free()` when
you are done with it. This should be straightforward because you process
one line at a time and never need to hold onto one longer.


## Deliverables

For this homework you must:

  - Implement the specification for the *translate* library
    from the previous section in `src/translate.c`.

  - Implement the specification for the *tr* program from
    the previous section in `src/tr.c`.

  - Add more test cases to `test/test_translate.c` in
    order to test the four functions that you defined in
    `src/translate.c`.

  - Adhere to the [IPD Style Manual for C and C++][style].

The file `test/test_convert.c` already contains two tests cases for each
of the four functions, and helper functions to facilitate testing for
two of them. Because the functions you are implementing are complex and
have many corner cases, you need to add many more tests for each. Try to
cover all the possibilities, because for this week’s self evaluation we
will spot-check your test coverage by asking for just a few particular
test cases. You can’t anticipate which we’ll ask about, so you should
try to cover everything.

```sh
$ echo tbbq/yhpx/naq/unir/sha% | ./tr n-z/a-m% a-m\ n-z!
```

[style]: https://github.com/nu-ipd/notes/blob/master/style2.md

[setup]: https://github.com/nu-ipd/notes/blob/master/setup2.md

