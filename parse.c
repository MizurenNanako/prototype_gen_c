#include "parse.h"
#include <ctype.h>
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
            if (ch == '\n')
            {
                ++t;
            }
            else
                spit_char(file);
            continue;
        }
        if (ch == '\n')
        {
            --t;
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
        if (ch == '#') // skip preprocessor
        {
            eat_line(file);
            continue;
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

enum symbol_t get_symbol(char *token)
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
