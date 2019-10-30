#include "translate.h"
#include "libipd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE_MESSAGE   "Usage: %s FROM TO < INPUT_FILE\n"
#define LENGTH_MESSAGE  "%s: error: lengths of FROM and TO differ\n"
#define OOM_MESSAGE     "%s: error: out of memory\n"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, USAGE_MESSAGE, argv[0]);
        exit(1);
    }

    // TODO: Your code goes here
}
