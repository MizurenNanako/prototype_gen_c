#include "options.h"
#include <stdlib.h>
#include <string.h>
#include "err.h"
#include "help.h"

// this stack memory will be used
// if program path is .lt. PROG_PATH_LEN chars
#define PROG_PATH_LEN 256
char prog_path_stkmem[PROG_PATH_LEN];

const char opts[][3][32] = {
    {"-h", "--help", "\0"},
    {"-r", "--recursive", "\1"},
    {"-i", "--input", "\2"},
    {"-o", "--output", "\3"},
};

#define OPTSIZE sizeof(opts) / sizeof(opts[0])

ulli __f0(struct options_t *opt, ulli i, const char **v)
{
    print_help(opt->prog_name);
    return i;
}
ulli __f1(struct options_t *opt, ulli i, const char **v)
{
    opt->recursive = true;
    return i;
}
ulli __f2(struct options_t *opt, ulli i, const char **v)
{
    int k, q;
    if (v[i + 1] && v[i + 1][0] == '-')
        return 0;
    if (opt->filenames_size)
        q = opt->filenames_size; // reserve
    else
        q = 0;
    for (k = i + 1; v[k] && v[k][0] != '-'; ++k)
        ++opt->filenames_size;
    const char **tmp = (const char **)
        malloc(opt->filenames_size * sizeof(char *));
    if (!tmp)
        err_malloc;
    if (q)
        memcpy(tmp, opt->filenames, q * sizeof(char *));
    for (k = i + 1; v[k] && v[k][0] != '-'; ++k, ++q)
    {
        tmp[q] = v[k];
    }
    if (opt->filenames)
        free(opt->filenames);
    opt->filenames = tmp;
    return k - 1;
}
ulli __f3(struct options_t *opt, ulli i, const char **v)
{
    if (v[i + 1] && v[i + 1][0] != '-')
    {
        opt->output_name = v[i + 1];
        return i + 1;
    }
    return 0;
}

ulli (*optf[])(struct options_t *opt, ulli i, const char **v) = {
    __f0,
    __f1,
    __f2,
    __f3,
};

int opt_parse(struct options_t *opt, int c, const char **v);

struct options_t *opt_create(int argc, const char **argv)
{
    // initialize
    struct options_t
        *r = (struct options_t *)
            malloc(sizeof(struct options_t));
    if (!r)
        err_malloc;
    r->filenames = NULL;
    r->filenames_size = 0;
    r->prog_name = NULL;
    r->prog_path = NULL;
    r->ishelp = false;
    r->recursive = false;
    opt_parse(r, argc, argv);
    return r;
}

void opt_free(struct options_t *src)
{
    /* No Need To Free
    ulli n = src->filenames_size;
    while (~(--n))
    {
        free(src->filenames[n]);
    }*/
    free(src->filenames);
    if (src->prog_path && src->prog_path != prog_path_stkmem)
        free((void *)src->prog_path); // cast to non-const
    free(src);
    src = NULL;
}

int opt_parse(struct options_t *opt, int c, const char **v)
{
    // tmp variable
    ulli i, j, t, s;
    // get program name
    for (t = strlen(v[0]) - 1;
         t >= 0 &&
#if defined(_WIN32) || defined(WIN32)
         v[0][t] != '\\'; // for windows
#else
         v[0][t] != '/'; // for *nix
#endif
         --t)
        ;
    ++t;
    opt->prog_name = v[0] + t;
    if (t > PROG_PATH_LEN)
    {
        char *p = (char *)malloc(t * sizeof(char));
        strncpy(p, v[0], t);
        p[t] = 0;
        opt->prog_path = p;
    }
    else
    {
        strncpy(prog_path_stkmem, v[0], t);
        prog_path_stkmem[t] = 0;
        opt->prog_path = prog_path_stkmem;
    }
    // get options
    for (i = 1; i < c; ++i)
    {
        t = 1;
        // parse short & long option
        for (j = 0; j < OPTSIZE; ++j)
        {
            if (
                (opts[j][0] &&
                 strcmp(v[i], opts[j][0]) == 0) ||
                (opts[j][1] &&
                 strcmp(v[i], opts[j][1]) == 0))
                t = 0;
            if (!t)
            {
                // s set to 0 when error
                s = optf[(opts[j][2][0])](opt, i, v);
                break;
            }
        }
        if (t || !s)
            err("Wrong option $%i: %s\n"
                "Try --help",
                i, v[i]);
        i = s;
    }
}
