#ifndef __err_h
#define __err_h

#include <stdio.h>
#include <stdlib.h>
#define err(MSG, ...)                     \
    do                                    \
    {                                     \
        fprintf(stderr,                   \
                "Error: " MSG             \
                "\nin file: "__FILE__     \
                "(line %i)\n"             \
                "Exiting...\n",           \
                ##__VA_ARGS__, __LINE__); \
        exit(EXIT_FAILURE);               \
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