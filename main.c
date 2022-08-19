#include "stdafx.h"
#include "options.h"
#include "stack.h"
#include "parse.h"
#include <stdio.h>
#include <string.h>
#include "err.h"

#include "test.h"

int main(int argc, char **argv)
{
    struct options_t *opt = opt_create(argc, argv);
    if (opt->filenames_size != 1)
        err("One file required"); 
    test_get_token(opt->prog_name, opt->filenames[0]);
    opt_free(opt);
    return 0;
}
