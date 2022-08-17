#ifndef __parse_h
#define __parse_h

#include "stdafx.h"
#include <stdio.h>

enum token_type_t
{
    nil = 0,
    keyword = 1,
    symbol = 2,
    userword = 3,
    num_literal = 4,
    str_literal = 5,
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
    arrow = -5, // ->
};

void spit_char(FILE *file);
void eat_line(FILE *file);
void spit_n_char(FILE *file, ulli n);
void get_token(FILE *file, char *buf, ulli bufsize);
enum token_type_t token_type(char *buf);

void parse(FILE *file);

#endif // __parse_h