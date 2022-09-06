// #ifndef __colordef_h
// #define __colordef_h

// foreground color
#ifdef NOCOLOR
// basic color
#define BLACK
#define RED
#define GREEN
#define YELLOW
#define BLUE
#define MAGENTA
#define CYAN
#define WHITE
// bright color
#define GRAY
#define HRED
#define HGREEN
#define HYELLOW
#define HBLUE
#define HMAGENTA
#define HCYAN
#define HWHITE
// non
#define NON
#else
// basic color
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
// bright color
#define GRAY "\033[90m"
#define HRED "\033[91m"
#define HGREEN "\033[92m"
#define HYELLOW "\033[93m"
#define HBLUE "\033[94m"
#define HMAGENTA "\033[95m"
#define HCYAN "\033[96m"
#define HWHITE "\033[97m"
// non
#define NON "\033[m"
#endif

// #endif // __colordef_h