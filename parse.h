#ifndef __parse_h
#define __parse_h

#include "stdafx.h"
#include <stdio.h>
#include "_line_metadata.h"

#define SHORTBUFSIZ 255
#define LINEBUFSIZ 4196

enum token_type_t
{
    tok_unknown = -1, // something went wrong
    tok_eof = 0,
    tok_word,
    tok_symbol,
    tok_literal_num,
    tok_literal_str,
    tok_literal_char,
    tok_preprocessor,
};

enum symbol_type_t
{
    sym_unknown = -1, // something went wrong
    sym_b_1L = '(',   // parentheses
    sym_b_1R = ')',   // parentheses
    sym_b_2L = '[',   // brackets
    sym_b_2R = ']',   // brackets
    sym_b_3L = '{',   // braces
    sym_b_3R = '}',   // braces

    sym_comma = ',',
    sym_dot = '.',
    sym_slash = '/',
    sym_colon = ':',
    sym_semicolon = ';',
    sym_backslash = '\\',
    sym_tilde = '~',
    sym_grave = '`',
    sym_apostrophe = '\'',
    sym_question_mark = '?',
    sym_asterisk = '*',
    sym_ampersand = '&',
    sym_sharp = '#',
    sym_dollor = '$',
    sym_at = '@',
    sym_percentage = '%',
    sym_pipe = '|',
    sym_circumflex = '^',
    // special
    sym_assign = '=',
    sym_eq = -1, // ==
    sym_exclamation_mark = '!',
    sym_neq = -2, // !=
    sym_le = '<',
    sym_leq = -3, //<=
    sym_ge = '>',
    sym_geq = -4, // >=
    sym_plus = '+',
    sym_increase = -6, // ++
    sym_dash = '-',
    sym_arrow = -5,         // ->
    sym_decrease = -7,      // --
    sym_veridic_param = -8, // ...
};

enum keyword_type_t
{
    kwd_unknown = -1, // something went wrong
    kwd_auto,
    kwd_break,
    kwd_case,
    kwd_char,
    kwd_const,
    kwd_continue,
    kwd_default,
    kwd_do,
    kwd_double,
    kwd_else,
    kwd_enum,
    kwd_extern,
    kwd_float,
    kwd_for,
    kwd_goto,
    kwd_if,
    kwd_inline, // C99
    kwd_int,
    kwd_long,
    kwd_register,
    kwd_restrict, // C99
    kwd_return,
    kwd_short,
    kwd_signed,
    kwd_sizeof,
    kwd_static,
    kwd_struct,
    kwd_switch,
    kwd_typedef,
    kwd_union,
    kwd_unsigned,
    kwd_void,
    kwd_volatile,
    kwd_while,
    // extend
    kwd__Alignas,       // C11
    kwd__Alignof,       // C11
    kwd__Atomic,        // C11
    kwd__Bool,          // C99
    kwd__Complex,       // C99
    kwd__Generic,       // C11
    kwd__Imaginary,     // C99
    kwd__Noreturn,      // C11
    kwd__Static_assert, // C11
    kwd__Thread_local,  // C11
};

enum prep_direc_type_t
{
    prep_direc_unknown = -1,
    prep_direc_define,
    prep_direc_elif,
    prep_direc_else,
    prep_direc_endif,
    prep_direc_error,
    prep_direc_if,
    prep_direc_ifdef,
    prep_direc_ifndef,
    prep_direc_include,
    prep_direc_pragma,
    prep_direc_undef,
};

void spit_char(FILE *file);
void spit_n_char(FILE *file, size_t n);
void eat_line(FILE *file); // literal line
enum token_type_t get_token(FILE *file, char *buf, size_t bufsize);
enum symbol_type_t get_symbol(const char *token);
enum keyword_type_t get_keyword(const char *token);
enum prep_direc_type_t get_prep_direc(const char *token);
enum lgc_line_type_t get_logical_line(FILE *file, char *buf, size_t bufsize);

void parse_def(FILE *file_in, FILE *file_out);

#endif // __parse_h