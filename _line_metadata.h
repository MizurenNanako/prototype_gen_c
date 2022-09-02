#ifndef ___line_metadata_h
#define ___line_metadata_h

#include "stdafx.h"

enum lgc_line_type_t
{
    lgc_line_unknown, // something went wrong
    lgc_line_decl_func,
    lgc_line_decl_var,
    lgc_line_decl_enum,
    lgc_line_decl_struct,
    lgc_line_decl_union,
    lgc_line_decl_type, // typedef
    lgc_line_init_var,
    lgc_line_decl_and_init_var,
};

struct _line_metadata_t;

struct _line_metadata_t *_line_metadata_create();
void _line_metadata_free(struct _line_metadata_t *lmd);
void _line_metadata_append(struct _line_metadata_t *lmd,
                           const char *token_copy, size_t token_len);
const char *_line_metadata_idtoken(struct _line_metadata_t *lmd);
#endif // ___line_metadata_h