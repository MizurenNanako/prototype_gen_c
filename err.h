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
                __LINE__, ##__VA_ARGS__); \
        exit(EXIT_FAILURE);               \
    } while (0)
#define err_nullptr \
    err("Null Pointer Exception.")
#define err_malloc \
    err("Failed on memory alloc.")
#endif // __err_h