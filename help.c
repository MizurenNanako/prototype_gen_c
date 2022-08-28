#include "help.h"
#include <stdio.h>
#include <stdlib.h>

char *default_prog_name = "prototype_gen_c";

void print_help(char *prog_name)
{
    if (!prog_name)
    {
        prog_name = default_prog_name;
    }
    fprintf(stdout,
            "%s:\n"
            "prototype generator for c language.\n"
            "OPTIONS:\n"
            "\t-h\t--help      \t"
            "Show this message.\n"
            "\t-i\t--input <filename> <filename> ...\n\t\t\t\t"
            "Specify inputs; Use stdin without this tag.\n"
            "\t-r\t--recursive \t"
            "Enable recursion on folders.\n"
            "\t-s\t--summarize \t"
            "Output summarized to one file.\n"
            "\t-o\t--output <filename>\n\t\t\t\t"
            "Specify summarized output. (Default: stdout)\n"
            "\n"
            "NOTE THAT MULTIPLE OPTIONS IN ONE \'-\' "
            "IS NOT LEGAL!!!\n"
            "Error Example: -ri<name>"
            "\n",
            prog_name);
    exit(EXIT_SUCCESS);
}