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
    else {
        char* from = expand_charset(argv[1]);
        char* to = expand_charset(argv[2]);
        if (from == NULL || to == NULL){
            fprintf(stderr, OOM_MESSAGE, argv[0]);
        }
        else if (strlen(from) != strlen(to)){
            fprintf(stderr, LENGTH_MESSAGE, argv[0]);
        }
        else {
            while(true){
                char *s = read_line();
                translate(s, from, to);
                printf("%s", s);
                printf("\n");
                free(s);
            }
        }
        free(from);
        free(to);
    }
    return 0;
}
