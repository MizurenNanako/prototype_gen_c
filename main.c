#include "stdafx.h"
#include "options.h"
#include "stack.h"
#include "parse.h"
#include <stdio.h>
#include <string.h>
#include "err.h"
#include "list.h"
// #include "test.h"

int *newint(int x)
{
    int *p = (int *)malloc(sizeof(int));
    *p = x;
    return p;
}

int main(int argc, char **argv)
{
    // struct options_t *opt = opt_create(argc, argv);
    // if (opt->filenames_size != 1)
    // err("One file required");
    // test_get_token(opt->prog_name, opt->filenames[0]);
    struct list_t *l = list_create(free);
    list_push_back(l, newint(5));
    list_push_back(l, newint(-2));
    list_push_back(l, newint(1));
    list_push_back(l, newint(4));
    list_push_back(l, newint(3));
    while (l->size)
    {
        printf("%d\n", *list_front(l, int));
        list_pop_front(l);
    }
    // opt_free(opt);
    return 0;
}
