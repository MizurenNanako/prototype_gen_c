#include "stdafx.h"
#include "options.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>
#include "err.h"

int main(int argc, char **argv)
{
    struct options_t *opt = opt_create(argc, argv);
    test_stack(argc, argv);
    opt_free(opt);
    return 0;
}

int test_stack(int argc, char **argv)
{
    struct stack_t *s = sta_create();
    int tmp;
    char *tmpc;
    tmpc = (char *)malloc(36);
    tmp = 2, sta_push(s, &tmp);
    tmp = -1, sta_push(s, &tmp);
    tmp = 3, sta_push(s, &tmp);
    snprintf(tmpc, 36, "hello stack");
    sta_push_s(s, tmpc, 12);
    snprintf(tmpc, 36, "pop stack");
    sta_push_s(s, tmpc, 10);
    free(tmpc);
    tmpc = sta_top_s(s, 10);
    sta_pop_s(s, 10);
    puts(tmpc);
    tmpc = sta_top_s(s, 12);
    sta_pop_s(s, 12);
    puts(tmpc);
    tmp = sta_top_v(s, int);
    sta_pop(s, int);
    printf("%i\n", tmp);
    tmp = sta_top_v(s, int);
    sta_pop(s, int);
    printf("%i\n", tmp);
    tmp = sta_top_v(s, int);
    sta_pop(s, int);
    printf("%i\n", tmp);
    printf("size of stack: %i\n", s->size);
    sta_free(s);
}

int test_optparse(int argc, char **argv)
{
    struct options_t *opts = opt_create(argc, argv);

    // test opt parser
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
    // err();
    return 0;
}