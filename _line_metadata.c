#include "_line_metadata.h"
#include <string.h>
#include "list.h"
#include "err.h"

struct _line_metadata_t
{
    enum lgc_line_type_t line_type;
    struct list_t *tokens; // str list
    size_t id_token_pos;
};

struct _line_metadata_t *_line_metadata__create()
{
    struct _line_metadata_t *p =
        (struct _line_metadata_t *)
            malloc(sizeof(struct _line_metadata_t));
    if (!p)
        err_malloc;
    p->line_type = lgc_line_unknown;
    p->tokens = list_create(free);
    p->id_token_pos = ~0;
    return p;
}

void _line_metadata__free(struct _line_metadata_t *lmd)
{
    if (!lmd)
        err_nullptr;
    list_free(lmd->tokens);
    free(lmd);
}

void _line_metadata_append(struct _line_metadata_t *lmd,
                           const char *token_copy, size_t token_len)
{
    if (!lmd)
        err_nullptr;
    char *p = (char *)malloc(token_len * sizeof(char));
    if (!p)
        err_malloc;
    strncpy(p, token_copy, token_len);
    list_push_back(lmd->tokens, p);
}

void __calc_id_pos(struct _line_metadata_t *lmd) // todo : implant it
{
}

const char *_line_metadata_idtoken(struct _line_metadata_t *lmd)
{
    if (!lmd)
        err_nullptr;
    size_t itp = lmd->id_token_pos;
    if (~itp)
        __calc_id_pos(lmd);
    return list_at(lmd->tokens, itp, char);
}
