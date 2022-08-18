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
        if (ch == '#') // skip preprocessor
        {
            eat_line(file);
            continue;
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
        if (isalpha(ch) || ch == '_') // maybe keyword, type
        {
            while (p < end && (isalnum(ch) || ch == '_'))
            // get whole word
            {
                *(p++) = ch;
                ch = fgetc(file);
            }
            spit_char(file);
            token_type = tok_word;
            break;
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
            // !"$%&'()*+,-./:;<=>?@[\]^`{|}~
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
                // !"$%&'()*,./:;?@[\]^`{|}~
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
