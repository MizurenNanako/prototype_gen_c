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
};

enum symbol_t
{
    b_1L = '(', // parentheses
    b_1R = ')', // parentheses
    b_2L = '[', // brackets
    b_2R = ']', // brackets
    b_3L = '{', // braces
    b_3R = '}', // braces
    b_4L = '<', // angle brackets
    b_4R = '>', // angle brackets
    assign = '=',
    eq = -1,  // ==
    neq = -2, // !=
    le = '<',
    leq = -3, //<=
    ge = '>',
    geq = -4, // >=
    tilde = '~',
    dot = '.',
    comma = ',',
    colon = ':',
    semicolon = ';',
    backslash = '\\',
    slash = '/',
    dash = '-',
    apostrophe = '`',
    question_mark = '?',
    exclamation_mark = '!',
    asterisk = '*',
    cross = '+',
    ampersand = '&',
    sharp = '#',
    dollor = '$',
    at = '@',
    percentage = '%',
    arrow = -5,    // ->
    increase = -6, // ++
    decrease = -7, // --
};

void spit_char(FILE *file);
void spit_n_char(FILE *file, ulli n);
void eat_line(FILE *file); // literal line
enum token_type_t get_token(FILE *file, char *buf, ulli bufsize);
enum symbol_t get_symbol(char *token);

void parse(FILE *file);

#endif // __parse_h