#ifndef __err_h
#define __err_h

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define NOCOLOR
#endif
#include "colordef.h"

#define err(MSG, ...)                               \
    do                                              \
    {                                               \
        fprintf(stderr,                             \
                RED "Error: "                /**/   \
                MAGENTA MSG "\n"             /**/   \
                YELLOW "in function: "       /**/   \
                HCYAN "%s\n"                 /**/   \
                YELLOW "in file: "           /**/   \
                HBLUE __FILE__ "(line %i)\n" /**/   \
                RED "Exiting...\n"           /**/   \
                NON,                                \
                ##__VA_ARGS__, __func__, __LINE__); \
        exit(EXIT_FAILURE);                         \
    } while (0)
#define err_nullptr \
    err("Null Pointer Exception.")
#define err_malloc \
    err("Failed on memory alloc.")
#define __double_quote(str) #str
#define err_out_of_range(var) \
    err("Variable (" __double_quote(var) ") out of range.")
#define err_buffer_overflow(var) \
    err("Buffer (" __double_quote(var) ") overflowed.")
#endif // __err_h