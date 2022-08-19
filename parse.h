#ifndef __parse_h
#define __parse_h

#include "stdafx.h"
#include <stdio.h>

enum token_type_t
{
    tok_unknown = 0, // something went wrong
    tok_eof,
    tok_word,
    tok_symbol,
    tok_literal_num,
    tok_literal_str,
    tok_literal_char,
};

enum symbol_t
{
    sym_unknown = 0, // something went wrong
    sym_b_1L = '(',  // parentheses
    sym_b_1R = ')',  // parentheses
    sym_b_2L = '[',  // brackets
    sym_b_2R = ']',  // brackets
    sym_b_3L = '{',  // braces
    sym_b_3R = '}',  // braces

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
    sym_arrow = -5,    // ->
    sym_decrease = -7, // --
};

void spit_char(FILE *file);
void spit_n_char(FILE *file, ulli n);
void eat_line(FILE *file); // literal line
enum token_type_t get_token(FILE *file, char *buf, ulli bufsize);
enum symbol_t get_symbol(char *token);

void parse(FILE *file);

#endif // __parse_h