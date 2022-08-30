#include "stdafx.h"
#include "options.h"
#include "stack.h"
#include "parse.h"
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "err.h"
#include "functool.h"
#include "test.h"

PRINT_U128_U_IMPL

int run0(const struct options_t *opt)
{
    FILE *out;
    if (!opt->output_name)
    {
        out = stdout;
        puts("Output to: stdout");
    }
    else if (!(out = fopen(opt->output_name, "w")))
        err("Failed to open %s", opt->output_name);
    if (opt->filenames_size < 1)
        err("Atlease One file required");

    for (size_t i = 0; i < opt->filenames_size; ++i)
    {
        FILE *in = fopen(opt->filenames[i], "r");
        if (!in)
            err("Failed to open %s", opt->filenames[i]);
        fprintf(out, "// From file: %s\n", opt->filenames[i]);
        parse_def(in, out);
        fputc('\n', out);
        fclose(in);
    }
}

// test prep & keyword parser
int run1(const struct options_t *opt)
{
    for (size_t i = 0; i < opt->filenames_size; ++i)
    {
        const char *h = opt->filenames[i];
        printf("%lu\n",
               hash_murmurhash3_x86_32(h, strlen(h), 0));
        print_u128_u(hash_murmurhash3_x64_128(h, strlen(h), 0));
        fputc('\n', stdout);
    }
}

int (*run)(const struct options_t *opt) = run1;

int main(int argc, const char **argv)
{
    struct options_t *opt = opt_create(argc, argv);
    run(opt);
    opt_free(opt);
    return 0;
}
