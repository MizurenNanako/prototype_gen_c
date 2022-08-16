#ifndef __options_h
#define __options_h

#include "stdafx.h"

struct options_t
{
    ulli filenames_size;
    char **filenames;
    char *prog_name;
    char *output_name;
    bool ishelp;
    bool recursive;
    bool summarize;
};

struct options_t *opt_create(int argc, char **argv);
void opt_free(struct options_t *src);

#endif //__options_h