#include "parse.h"
#include <ctype.h>
#include <string.h>
#include "err.h"

void spit_char(FILE *file)
{
    fseek(file, -1, SEEK_CUR);
}

void spit_n_char(FILE *file, ulli n)
{
    fseek(file, -n, SEEK_CUR);
}

void eat_line(FILE *file)
{
    int ch, t;
    t = 1;
    while (t)
    {
        ch = fgetc(file);
        if (feof(file)) // stop at eof
            break;
        if (isblank(ch)) // skip blank
            continue;
        if (ch == '\\')
        {
            do
            {
                ch = fgetc(file);
            } while (isblank(ch));
            if (ch != '\n')
                spit_char(file);
            continue;
        }
        if (ch == '\n')
        {
            t = 0;
            continue;
        }
    }
}

enum token_type_t get_token(FILE *file, char *buf, ulli bufsize)
{
    char *p, *end;
    int ch, t, s[5];
    enum token_type_t token_type = tok_unknown;
    p = buf;
    end = buf + bufsize;
    while (p < end)
    {
        ch = fgetc(file);
        if (feof(file)) // stop at eof
        {
            token_type = tok_eof;
            break;
        }
        if (isblank(ch)) // skip blank
            continue;
        if (ch == '/') // test if is comment
        {
            // this is special!
            ch = fgetc(file);
            if (ch == '/') // comment line
            {
                eat_line(file);
                continue;
            }
            else if (ch == '*') // comment block
            {
                t = 1;
                while (t)
                {
                    ch = fgetc(file);
                    if (feof(file)) // stop at eof
                        break;
                    if (ch == '*')
                    {
                        ch = fgetc(file);
                        if (ch == '/')
                            t = 0;
                        else
                            spit_char(file);
                        continue;
                    }
                }
                continue;
            }
            else
            {
                spit_char(file);
                *(p++) = '/';
                token_type = tok_symbol;
                break;
            }
        }
        if (ch == '#') // get preprocessor
        {
            t = 1;
            while (t)
            {
                ch = fgetc(file);
                if (feof(file)) // stop at eof
                    break;
                if (isblank(ch)) // skip blank
                {
                    *(p++) = ch;
                    ch = fgetc(file);
                    if (isblank(ch))
                    {
                        do
                        {
                            ch = fgetc(file);
                        } while (isblank(ch));
                    }
                    spit_char(file);
                    continue;
                }
                if (ch == '\\')
                {
                    do
                    {
                        ch = fgetc(file);
                    } while (isblank(ch));
                    if (ch == '\n')
                    {
                        --p;
                    }
                    else
                        spit_char(file);
                    continue;
                }
                if (ch == '\n')
                {
                    t = 0;
                    continue;
                }
                *(p++) = ch;
            }
            token_type = tok_preprocessor;
            break;
        }
        if (ch == '\'') // must be char literal
        {
            t = 1;
            while (p < end && t)
            {
                *(p++) = ch;
                ch = fgetc(file);
                if (ch == '\\')
                {
                    *(p++) = ch;
                    ch = fgetc(file);
                }
                else if (ch == '\'')
                {
                    t = 0;
                }
            }
            *(p++) = ch;
            token_type = tok_literal_char;
            break;
        }
        if (ch == '\"') // must be str literal
        {
            t = 1;
            while (p < end && t)
            {
                *(p++) = ch;
                ch = fgetc(file);
                if (ch == '\\')
                {
                    *(p++) = ch;
                    ch = fgetc(file);
                }
                else if (ch == '\"')
                {
                    t = 0;
                }
            }
            *(p++) = ch;
            token_type = tok_literal_str;
            break;
        }
        if (isalpha(ch) || ch == '_' ||
            ch == '$') // maybe keyword, type
        {
            while (p < end &&
                   (isalnum(ch) || ch == '_' || ch == '$'))
            // get whole word
            {
                *(p++) = ch;
                ch = fgetc(file);
            }
            spit_char(file);
            token_type = tok_word;
            break;
        }
        if (ch == '.') // this is special (...)
        {
            *(p++) = '.';
            ch = fgetc(file);
            if (ch == '.')
            {
                ch = fgetc(file);
                if (ch == '.')
                {
                    *(p++) = '.';
                    *(p++) = '.';
                    token_type = tok_symbol;
                    break;
                }
                else
                {
                    spit_char(file);
                    spit_char(file);
                    token_type = tok_symbol;
                    break;
                }
            }
            else
            {
                spit_char(file);
                token_type = tok_symbol;
                break;
            }
        }
        if (isdigit(ch)) // must be number
        {
            t = 1; // dot mark
            while (p < end && (isdigit(ch) || ch == '.'))
            {
                if (t && ch == '.')
                {
                    t = 0;
                    *(p++) = ch;
                }
                else if (isdigit(ch))
                    *(p++) = ch;
                ch = fgetc(file);
            }
            spit_char(file);
            token_type = tok_literal_num;
            break;
        }
        if (ispunct(ch)) // !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
        {
            // in fact:
            // !"$%&'()*+,-:;<=>?@[\]^`{|}~
            *(p++) = ch;
            switch (ch)
            {
            case '<':
            case '=':
            case '>':
                t = 1;
                s[0] = '=';
                break;
            case '+':
                t = 1;
                s[0] = '+';
                break;
            case '-':
                t = 1;
                s[0] = '>';
                s[1] = '-';
                break;
            default:
                // !"$%&'()*,:;?@[\]^`{|}~
                t = 0;
                break;
            }
            while (t--)
            {
                ch = fgetc(file);
                if (ch == s[t])
                    *(p++) = ch;
                else
                    spit_char(file);
            }
            token_type = tok_symbol;
            break;
        }
    }
    if (p >= end)
        err_out_of_range(p);
    *p = 0; // seal
    return token_type;
}

enum symbol_type_t get_symbol(char *token)
{
    if (!token || !token[0])
        err_nullptr;
    switch (token[0])
    // !%&'()*+,-./:;<=>?@[\]^`{|}~
    {
        // brackets
    case '(':
        return sym_b_1L;
    case ')':
        return sym_b_1R;
    case '[':
        return sym_b_2L;
    case ']':
        return sym_b_2R;
    case '{':
        return sym_b_3L;
    case '}':
        return sym_b_3R;

        // single
    case '~':
        return sym_tilde;
    case '/':
        return sym_slash;
    case '\\':
        return sym_backslash;
    case '@':
        return sym_at;
    case ':':
        return sym_colon;
    case ';':
        return sym_semicolon;
    case '*':
        return sym_asterisk;
    case '`':
        return sym_grave;
    case ',':
        return sym_comma;
    case '?':
        return sym_question_mark;
    case '&':
        return sym_ampersand;
    case '%':
        return sym_percentage;
    case '|':
        return sym_pipe;
    case '^':
        return sym_circumflex;
    case '\'':
        return sym_apostrophe;

        // special
    case '!':
        if (token[1])
        {
            if (token[1] == '=')
                return sym_neq;
            return sym_unknown;
        }
        return sym_exclamation_mark;
    case '+':
        if (token[1])
        {
            if (token[1] == '+')
                return sym_increase;
            return sym_unknown;
        }
        return sym_plus;
    case '=':
        if (token[1])
        {
            if (token[1] == '=')
                return sym_eq;
            return sym_unknown;
        }
        return sym_assign;
    case '<':
        if (token[1])
        {
            if (token[1] == '=')
                return sym_leq;
            return sym_unknown;
        }
        return sym_le;
    case '>':
        if (token[1])
        {
            if (token[1] == '=')
                return sym_geq;
            return sym_unknown;
        }
        return sym_ge;
    case '-':
        if (token[1])
        {
            if (token[1] == '-')
                return sym_decrease;
            if (token[1] == '>')
                return sym_arrow;
            return sym_unknown;
        }
        return sym_ge;
    case '.':
        if (token[1] && token[2])
        {
            if (token[1] == '.' &&
                token[2] == '.')
                return sym_veridic_param;
            return sym_unknown;
        }
        return sym_dot;

        // something went wrong
    default:
        return sym_unknown;
    }
}

enum keyword_type_t get_keyword(char *token)
{
    if (!token || !token[0])
        err_nullptr;
    switch (token[0])
    {
    case 'a': // auto
        if (!strcmp(token + 1, "uto"))
            return kwd_auto;
        return kwd_unknown;
    case 'b': // break
        if (!strcmp(token + 1, "reak"))
            return kwd_break;
        return kwd_unknown;
    case 'c': // case char const continue
        switch (token[1])
        {
        case 'a':
            if (!strcmp(token + 2, "se"))
                return kwd_case;
            return kwd_unknown;
        case 'h':
            if (!strcmp(token + 2, "ar"))
                return kwd_char;
            return kwd_unknown;
        case 'o':
            if (token[2] == 'n')
                if (!strcmp(token + 3, "st"))
                    return kwd_const;
                else if (!strcmp(token + 3, "tinue"))
                    return kwd_continue;
            return kwd_unknown;
        }
        return kwd_unknown;
    case 'd': // default do double
        switch (token[1])
        {
        case 'e':
            if (!strcmp(token + 2, "fault"))
                return kwd_default;
            return kwd_unknown;
        case 'o':
            if (!token[2])
                return kwd_do;
            if (!strcmp(token + 2, "uble"))
                return kwd_double;
            return kwd_unknown;
        }
        return kwd_unknown;
    case 'e': // else enum extern
        switch (token[1])
        {
        case 'l':
            if (!strcmp(token + 2, "se"))
                return kwd_else;
            return kwd_unknown;
        case 'n':
            if (!strcmp(token + 2, "um"))
                return kwd_enum;
            return kwd_unknown;
        case 'x':
            if (!strcmp(token + 2, "tern"))
                return kwd_extern;
            return kwd_unknown;
        }
        return kwd_unknown;
    case 'f': // float for
        switch (token[1])
        {
        case 'l':
            if (!strcmp(token + 2, "oat"))
                return kwd_float;
            return kwd_unknown;
        case 'o':
            if (!strcmp(token + 2, "r"))
                return kwd_for;
            return kwd_unknown;
        }
        return kwd_unknown;
    case 'g': // goto
        if (!strcmp(token + 1, "oto"))
            return kwd_goto;
        return kwd_unknown;
    case 'i': // if inline int
        switch (token[1])
        {
        case 'f':
            if (!token[2])
                return kwd_if;
            return kwd_unknown;
        case 'n':
            if (!strcmp(token + 2, "line"))
                return kwd_inline;
            else if (!strcmp(token + 2, "t"))
                return kwd_int;
            return kwd_unknown;
        }
        return kwd_unknown;
    case 'l': // long
        if (!strcmp(token + 1, "ong"))
            return kwd_long;
        return kwd_unknown;
    case 'r': // register restrict return
        if (token[1] == 'e')
        {
            switch (token[2])
            {
            case 'g':
                if (!strcmp(token + 3, "ister"))
                    return kwd_register;
                return kwd_unknown;
            case 's':
                if (!strcmp(token + 3, "trict"))
                    return kwd_restrict;
                return kwd_unknown;
            case 't':
                if (!strcmp(token + 3, "urn"))
                    return kwd_return;
                return kwd_unknown;
            }
        }
        return kwd_unknown;
    case 's': // short signed sizeof static struct switch
        switch (token[1])
        {
        case 'h':
            if (!strcmp(token + 2, "ort"))
                return kwd_short;
            return kwd_unknown;
        case 'i':
            switch (token[2])
            {
            case 'g':
                if (!strcmp(token + 3, "ned"))
                    return kwd_signed;
                return kwd_unknown;
            case 'z':
                if (!strcmp(token + 3, "eof"))
                    return kwd_sizeof;
                return kwd_unknown;
            }
            return kwd_unknown;
        case 't':
            switch (token[2])
            {
            case 'a':
                if (!strcmp(token + 3, "tic"))
                    return kwd_static;
                return kwd_unknown;
            case 'r':
                if (!strcmp(token + 3, "uct"))
                    return kwd_struct;
                return kwd_unknown;
            }
            return kwd_unknown;
        case 'w':
            if (!strcmp(token + 2, "itch"))
                return kwd_switch;
            return kwd_unknown;
        }
        return kwd_unknown;
    case 't': // typedef
        if (!strcmp(token + 1, "ypedef"))
            return kwd_typedef;
        return kwd_unknown;
    case 'u': // union unsigned
        if (token[1] == 'n')
            if (!strcmp(token + 2, "ion"))
                return kwd_union;
            else if (!strcmp(token + 2, "signed"))
                return kwd_unsigned;
        return kwd_unknown;
    case 'v': // void volatile
        if (token[1] == 'o')
            if (!strcmp(token + 2, "id"))
                return kwd_void;
            else if (!strcmp(token + 2, "latile"))
                return kwd_volatile;
        return kwd_unknown;
    case 'w': // while
        if (!strcmp(token + 1, "hile"))
            return kwd_while;
        return kwd_unknown;
    case '_':
        // _Alignas _Alignof _Atomic _Bool _Complex _Generic
        // _Imaginary _Noreturn _Static_assert _Thread_local
        switch (token[1])
        {
        case 'A': // _Alignas _Alignof _Atomic
            switch (token[2])
            {
            case 'l':
                if (token[3] == 'i' && token[4] == 'g' && token[5] == 'n')
                {
                    if (!strcmp(token + 6, "as"))
                        return kwd__Alignas;
                    else if (!strcmp(token + 6, "of"))
                        return kwd__Alignof;
                }
                return kwd_unknown;
            case 't':
                if (!strcmp(token + 3, "omic"))
                    return kwd__Atomic;
                return kwd_unknown;
            }
            return kwd_unknown;
        case 'B': // _Bool
            if (!strcmp(token + 2, "ool"))
                return kwd__Bool;
            return kwd_unknown;
        case 'C': // _Complex
            if (!strcmp(token + 2, "omplex"))
                return kwd__Complex;
            return kwd_unknown;
        case 'G': // _Generic
            if (!strcmp(token + 2, "eneric"))
                return kwd__Generic;
            return kwd_unknown;
        case 'I': // _Imaginary
            if (!strcmp(token + 2, "maginary"))
                return kwd__Imaginary;
            return kwd_unknown;
        case 'N': // _Noreturn
            if (!strcmp(token + 2, "oreturn"))
                return kwd__Noreturn;
            return kwd_unknown;
        case 'S': // _Static_assert
            if (!strcmp(token + 2, "tatic_assert"))
                return kwd__Static_assert;
            return kwd_unknown;
        case 'T': // _Thread_local
            if (!strcmp(token + 2, "hread_local"))
                return kwd__Thread_local;
            return kwd_unknown;
        }
        return kwd_unknown;
    }
    return kwd_unknown;
}

void parse_def(FILE *f_in, FILE *f_out)
{
    char buffer[SHORTBUFSIZ];
    char prepare[LINEBUFSIZ];
    enum token_type_t tok_type, last_tok_type;
    enum symbol_type_t sym_type, last_sym_type;
    int parse_level;
    bool eol;
    enum
    {
        def_not,
        def_func,
        def_data,
        def_var,
        def_wait_semicolon,
    } def_type;
    buffer[0] = prepare[0] = 0;
    last_tok_type = tok_unknown;
    last_sym_type = sym_unknown;
    parse_level = 0;
    eol = false;
    def_type = def_not;
    while (tok_type = get_token(f_in, buffer, SHORTBUFSIZ))
    {
        if (tok_type == tok_symbol)
        {
            sym_type = get_symbol(buffer);
            switch (sym_type)
            {
            case sym_b_1R:
                def_type = def_func;
                break;
            case sym_b_3L:
                ++parse_level;
                break;
            case sym_b_3R:
                --parse_level;
                /* buffer[0] = ';';
                buffer[1] = '\n';
                buffer[2] = 0; */
                if (def_type != def_func &&
                    def_type != def_wait_semicolon)
                    def_type = def_data;
                eol = true;
                break;
            case sym_asterisk:
                buffer[0] = ' ';
                buffer[1] = '*';
                buffer[2] = 0;
                break;
            case sym_comma:
                buffer[0] = ',';
                buffer[1] = ' ';
                buffer[2] = 0;
                break;
            case sym_semicolon:
                /* if (last_sym_type == sym_b_3R)
                    buffer[0] = 0;
                buffer[1] = '\n';
                buffer[2] = 0; */
                if (def_type == def_wait_semicolon)
                    def_type = def_not;
                eol = true;
                break;
            case sym_assign:
                /* buffer[1] = buffer[0];
                buffer[0] = ' ';
                buffer[2] = ' ';
                buffer[3] = 0; */
                /* buffer[0] = 0; */
                def_type = def_var;
                eol = true;
                break;
            }
        }
        if (parse_level)
            continue;
        if (!eol)
            switch (tok_type)
            {
            case tok_word:
                if (last_tok_type == tok_type)
                    // fputc(' ', f_out);
                    strcat(prepare, " ");
                // fprintf(f_out, "%s", buffer);
                strcat(prepare, buffer);
                break;
            case tok_symbol:
                // fprintf(f_out, "%s", buffer);
                strcat(prepare, buffer);
                break;
            }
        else
        {
            if (def_type == def_var)
            {
                fprintf(f_out, "%s %s;\n", "extern", prepare);
                def_type = def_wait_semicolon;
            }
            else if (def_type != def_wait_semicolon &&
                     def_type != def_not)
            {
                fprintf(f_out, "%s;\n", prepare);
                def_type = def_not;
            }
            prepare[0] = 0;
            eol = false;
        }
        last_tok_type = tok_type;
        last_sym_type = sym_type;
    }
}
