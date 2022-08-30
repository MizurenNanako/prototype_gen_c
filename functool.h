#ifndef __functool_h
#define __functool_h

#define __COUNT_PARAM_HELPER(_10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _, ...) _

// This method cannot tell 1 or 0 param.
// max param : 10
#define __COUNT_PARAM(...) \
    __COUNT_PARAM_HELPER(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// example: __DEF_FUNC_TYPE(int,t1,int,int) => int(*t1)(int,int)
#define __DEF_FUNC_TYPE(ret_type, name, ...) \
    typedef ret_type (*name)(__VA_ARGS__);

// for invoking from void *
#define __force_invoke(func, func_type, ...) \
    ((func_type)func)(__VA_ARGS__)

/*      UINT64_MAX 18446744073709551615ULL */
#define P10_UINT64 10000000000000000000ULL /* 19 zeroes */
#define PRINT_U128_U_IMPL                            \
    static int print_u128_u(__uint128_t u128)        \
    {                                                \
        int rc;                                      \
        if (u128 > UINT64_MAX)                       \
        {                                            \
            __uint128_t leading = u128 / P10_UINT64; \
            uint64_t trailing = u128 % P10_UINT64;   \
            rc = print_u128_u(leading);              \
            rc += printf("%.19llu", trailing);       \
        }                                            \
        else                                         \
        {                                            \
            uint64_t u64 = u128;                     \
            rc = printf("%llu", u64);                \
        }                                            \
        return rc;                                   \
    }

#endif // __functool_h