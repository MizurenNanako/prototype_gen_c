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
void fake_free(void *p) {}

int main(int argc, char **argv)
{
    // struct options_t *opt = opt_create(argc, argv);
    // if (opt->filenames_size != 1)
    // err("One file required");
    // test_get_token(opt->prog_name, opt->filenames[0]);
    struct list_t *l = list_create(fake_free);
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
    for (size_t i = 0; i < l->size; ++i)
    {
        printf("%s\n", list_at_p(l, i));
    }
    printf("%s\n", list_at_p(l, 0));
    printf("%s\n", list_at_p(l, 3));
    printf("%s\n", list_at_p(l, 5));
    printf("%s\n", list_at_p(l, l->size - 1));
    list_free(l);
    // opt_free(opt);
    return 0;
}
