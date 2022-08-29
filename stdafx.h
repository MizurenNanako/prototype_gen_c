#ifndef __stdafx_h
#define __stdafx_h

#include <stddef.h>

typedef char bool;
#define true 1
#define false 0

typedef unsigned long long int ulli;
typedef unsigned char byte;

#define __absolute_dest(a, b) \
    (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

#endif //__stdafx_h
