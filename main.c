#include "stdafx.h"
#include "options.h"
#include "stack.h"
#include "parse.h"
#include <stdio.h>
#include <string.h>
#include "err.h"

int main(int argc, char **argv)
{
    struct options_t *opt = opt_create(argc, argv);
    FILE *in = fopen("./stack.c", "r");
    while (!feof(in))
    {
        char buf[255];
        get_token(in, buf, 255);
        printf("%s\t", buf);
    }
    printf("\n");
    fclose(in);
    opt_free(opt);
    return 0;
}

#include "test.out"
