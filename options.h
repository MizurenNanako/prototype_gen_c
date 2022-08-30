#ifndef __options_h
#define __options_h

#include "stdafx.h"

struct options_t
{
    ulli filenames_size;
    const char **filenames;
    const char *prog_name;
    const char *output_name;
    bool ishelp;
    bool recursive;
    bool summarize;
};

struct options_t *opt_create(int argc, const char **argv);
void opt_free(struct options_t *src);

#endif //__options_h