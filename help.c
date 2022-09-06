#include "help.h"
#include <stdio.h>
#include <stdlib.h>

#define HELP_MSG ""                                           /**/ \
    GREEN "%s:\n"                                             /**/ \
    CYAN "prototype generator for c language.\n"              /**/ \
    MAGENTA "OPTIONS:\n"                                      /**/ \
    HCYAN "\t-h\t--help      \t"                              /**/ \
    HBLUE "Show this message.\n"                              /**/ \
    HCYAN "\t-i\t--input <filename> <filename> ...\n\t\t\t\t" /**/ \
    HBLUE "Specify inputs; Use stdin without this tag.\n"     /**/ \
    HCYAN "\t-r\t--recursive \t"                              /**/ \
    HBLUE "Enable recursion on folders.\n"                    /**/ \
    HCYAN "\t-o\t--output <filename>\n\t\t\t\t"               /**/ \
    HBLUE "Specify summarized output. (Default: stdout)\n"    /**/ \
    HCYAN "\t-b\t--nocolor <filename>\n\t\t\t\t"              /**/ \
    HBLUE "Specify summarized output. (Default: stdout)\n"    /**/ \
                 "\n"                                         /**/ \
    RED "NOTE THAT MULTIPLE OPTIONS IN ONE \'-\' "            /**/ \
                 "IS NOT LEGAL!!!\n"                          /**/ \
    HRED "Error Example: -ri<name>"                           /**/ \
    NON "\n"

char *default_prog_name = "prototype_gen_c";

void print_help(const char *prog_name, bool nocolor)
{
    if (!prog_name)
    {
        prog_name = default_prog_name;
    }
    if (nocolor)
#define NOCOLOR
#include "colordef.h"
        fprintf(stdout, HELP_MSG, prog_name);
    else
#undef NOCOLOR
#include "colordef.h"
        fprintf(stdout, HELP_MSG, prog_name);
    exit(EXIT_SUCCESS);
}
