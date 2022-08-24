#include "stdafx.h"
#include <stdlib.h>
#include "options.h"
#include "stack.h"
#include "parse.h"
#include "functool.h"

int test_get_token(char *prog_name, char *target_name)
{
    char tag[] = "?Ew+1$c";
    FILE *in = fopen(target_name, "r");
    if (!in)
        err("Failed to open %s", target_name);
    FILE *out = fopen("./output.txt", "w");
    while (!feof(in))
    {
        char buf[255];
        int t = get_token(in, buf, 255);
        fprintf(out, "type:%c\tinfo:%s\t", tag[t], buf);
        if (t == tok_symbol)
            fprintf(out, "\tsym:%i", get_symbol(buf));
        if (t == tok_literal_char)
            fprintf(out, "value:%i", buf[0]);
        fprintf(out, "\n");
    }
    printf("program name:%s\n", prog_name);
    fclose(in);
    fclose(out);
}

int test_stack()
{
    struct stack_t *s = stack_create();
    int tmp;
    char *tmpc;
    tmpc = (char *)malloc(36);
    tmp = 2, stack_push(s, &tmp);
    tmp = -1, stack_push(s, &tmp);
    tmp = 3, stack_push(s, &tmp);
    snprintf(tmpc, 36, "hello stack");
    stack_push_s(s, tmpc, 12);
    snprintf(tmpc, 36, "pop stack");
    stack_push_s(s, tmpc, 10);
    free(tmpc);
    tmpc = (char *)stack_top_s(s, 10);
    stack_pop_s(s, 10);
    puts(tmpc);
    tmpc = (char *)stack_top_s(s, 12);
    stack_pop_s(s, 12);
    puts(tmpc);
    tmp = stack_top_v(s, int);
    stack_pop(s, int);
    printf("%i\n", tmp);
    tmp = stack_top_v(s, int);
    stack_pop(s, int);
    printf("%i\n", tmp);
    tmp = stack_top_v(s, int);
    stack_pop(s, int);
    printf("%i\n", tmp);
    printf("size of stack: %i\n", s->size);
    stack_free(s);
}

int test_optparse(struct options_t *opts)
{
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
}