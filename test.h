#include "stdafx.h"
#include <stdlib.h>
#include "options.h"
#include "stack.h"
#include "list.h"
#include "hash.h"
#include "parse.h"
#include "functool.h"

#define something \
    newint(5);    \
    newint(6);

int a[] = {1, 2, 3, 4};
char b[][30] = {
    "hello",
    "test",
};
int c[][5] = {
    {1, 2, 3, 4, 5},
    {0, 9, 8, 7, 6},
};

int *newint(int x)
{
    int *p = (int *)malloc(sizeof(int));
    *p = x;
    return p;
}

int test_get_$(struct options_t *opt);

int test_get_$(struct options_t *opt)
{
    FILE *out = stdout;
    char tag[] = "Ew+1$c#";
    if (opt->filenames_size < 1)
        err("Atlease One file required");
    for (size_t i = 0; i < opt->filenames_size; ++i)
    {
        FILE *in = fopen(opt->filenames[i], "r");
        if (!in)
            err("Failed to open %s", opt->filenames[i]);
        fprintf(out, "// From file: %s\n", opt->filenames[i]);
        while (!feof(in))
        {
            char buf[255];
            int t = get_token(in, buf, 255);
            fprintf(out, "type: %c\tinfo: %s\t", tag[t], buf);
            switch (t)
            {
            case tok_symbol:
                fprintf(out, "\tsym: %i", get_symbol(buf));
                break;
            case tok_word:
                fprintf(out, "\ttype: %i", get_keyword(buf));
                break;
            case tok_literal_char:
                fprintf(out, "\tvalue: %i", buf[0]);
                break;
            case tok_preprocessor:
                fprintf(out, "\ttype: %i", get_prep_direc(buf));
                break;
            }
            fprintf(out, "\n");
        }
        fputc('\n', out);
        fclose(in);
    }
}

PRINT_U128_U_IMPL;

int test_murmurhash3(struct options_t *opt)
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
    for (size_t i = 0; i < list_size(l); ++i)
    {
        printf("%s\n", list_at_p(l, i));
    }
    list_free(l);
}

int test_get_token(char *prog_name, char *target_name)
{
    char tag[] = "Ekw+1$c#";
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
    printf("size of stack: %i\n", stack_size(s));
    stack_free(s);
}

int test_optparse(const struct options_t *opts)
{
    // test opt parser
    puts(opts->prog_name);
    puts(opts->prog_path);
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
}