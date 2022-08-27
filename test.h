#include "stdafx.h"
#include <stdlib.h>
#include "options.h"
#include "stack.h"
#include "list.h"
#include "parse.h"
#include "functool.h"

#define something \
    newint(5);    \
    newint(6);

int *newint(int x)
{
    int *p = (int *)malloc(sizeof(int));
    *p = x;
    return p;
}

int test_list()
{
    struct list_t *l = list_create(NULL);
    list_push_back(l, "hello");
    list_push_back(l, "there");
    list_push_back(l, "right");
    list_push_back(l, "something");
    list_push_back(l, "addd");
    list_push_back(l, "asdfg");
    list_push_back(l, "qwe");
    list_push_back(l, "zxcvbnm");
    // while (l->size)
    // {
    //     printf("%s\n", list_front_p(l));
    //     list_pop_front(l);
    // }
    list_delete_at(l, 5);
    list_delete_at(l, 5);
    list_delete_at(l, 5);
    list_insert_before(l, 4, "five");
    for (size_t i = 0; i < l->size; ++i)
    {
        printf("%s\n", list_at_p(l, i));
    }
    list_free(l);
}

int test_get_token(char *prog_name, char *target_name)
{
    char tag[] = "?Ew+1$c#";
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