#include "parse.h"
#include <ctype.h>
#include "err.h"

void spit_char(FILE *file)
{
    fseek(file, -1, SEEK_CUR);
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

void get_token(FILE *file, char *buf, ulli bufsize)
{
    char *p, *end;
    int ch, t;
    p = buf;
    end = buf + bufsize;
    while (p < end)
    {
        ch = fgetc(file);
        if (feof(file)) // stop at eof
            break;
        if (isblank(ch)) // skip blank
            continue;
        if (ch == '#')
        {
            eat_line(file);
            continue;
        }
        if (isalpha(ch) || ch == '_')
        // maybe keyword, type
        {
            while (p < end && (isalnum(ch) || ch == '_'))
            // get whole word
            {
                *(p++) = ch;
                ch = fgetc(file);
            }
            spit_char(file);
            break;
        }
        if (isdigit(ch))
        // must be number
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
            break;
        }
        if (ispunct(ch))
        // !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
        {
            *(p++) = ch;
            switch (ch)
            {
            case '<':
            case '=':
            case '>':
                t = '=';
                break;
            case '-':
                t = '>';
                break;
            default:
                t = 0;
                break;
            }
            if (t)
            {
                ch = fgetc(file);
                if (ch == t)
                    *(p++) = ch;
                else
                    spit_char(file);
            }
            break;
        }
    }
    if (p >= end)
        err_out_of_range(p);
    *p = 0; // seal
}