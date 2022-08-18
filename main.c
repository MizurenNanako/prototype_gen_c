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
    test_get_token(opt);
    opt_free(opt);
    return 0;
}
