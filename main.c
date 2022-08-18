#include "stdafx.h"
#include "options.h"
#include "stack.h"
#include "parse.h"
#include <stdio.h>
#include <string.h>
#include "err.h"

#include "test.out"
char tag[] = "?Ew+1$";

int main(int argc, char **argv)
{
    struct options_t *opt = opt_create(argc, argv);
    FILE *in = fopen("./main.c", "r");
    int l = 0;
    while (!feof(in))
    {
        char buf[255];
        int t = get_token(in, buf, 255);
        printf("%c:%s\t", tag[t], buf);
        if (++l > 15)
        {
            printf("\n");
            l = 0;
        }
    }
    printf("\tprog:%s\n", opt->prog_name);
    fclose(in);
    opt_free(opt);
    return 0;
}
