#include "stdafx.h"
#include "options.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    struct options_t *opts = opt_create(argc, argv);

    // test
    puts(opts->prog_name);
    for (int i = 0; i < opts->filenames_size; ++i)
    {
        printf("input: %s\n", opts->filenames[i]);
    }
    if (opts->output_name)
        printf("output: %s\n", opts->output_name);
    else
        puts("No output name");
    if (opts->recursive)
        puts("recursive");
    else
        puts("non-recursive");
    if (opts->summarize)
        puts("summarize");
    else
        puts("non-summarize");

    opt_free(opts);
    return 0;
}