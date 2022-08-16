#ifndef __err_h
#define __err_h

#include <stdio.h>
#include <stdlib.h>
#define err(MSG, ...)                             \
    do                                            \
    {                                             \
        fprintf(stderr,                           \
                "Error: \n" MSG "\nExiting...\n", \
                ##__VA_ARGS__);                   \
        exit(EXIT_FAILURE);                       \
    } while (0)

#endif // __err_h