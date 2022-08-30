#include "hash.h"
#include "err.h"

// private data/method
uint32_t __crc32_table[256] = {0, 0};
uint64_t __crc64_table_ECMA[256] = {0, 0};
void __init_crc32_table(void)
{
    uint32_t c;
    uint32_t i, j;
    for (i = 0; i < 256; ++i)
    {
        c = (uint32_t)i;
        for (j = 0; j < 8; ++j)
        {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        __crc32_table[i] = c;
    }
}
void __init_crc64_ECMA_table(void)
{
    uint64_t c;
    uint64_t i, j;
    for (i = 0; i < 256; ++i)
    {
        c = (uint64_t)i;
        for (j = 0; j < 8; ++j)
        {
            if (c & 1)
                c = UINT64_C(0xC96C5795D7870F42) ^ (c >> 1);
            else
                c = c >> 1;
        }
        __crc64_table_ECMA[i] = c;
    }
}
// end private

uint64_t hash_polynomial_rolling_53(const unsigned char *str)
{
    if (!str)
        err_nullptr;
    ulli r = 0ULL;
    ulli p = 1ULL;
    while (*str)
    {
        r += *(str++) * p;
        p *= 53ULL;
    }
    return r;
}

uint32_t hash_cyclic_redundancy_check_32(const unsigned char *str)
{
    if (!str)
        err_nullptr;
    if (!__crc32_table[1])
        __init_crc32_table();
    uint32_t crc = 0;
    while (*str)
        crc = __crc32_table[(crc ^ *(str++)) & 0xff] ^ (crc >> 8);
    return crc;
}

uint64_t hash_cyclic_redundancy_check_64_ECMA(const unsigned char *str)
{
    if (!str)
        err_nullptr;
    if (!__crc64_table_ECMA[1])
        __init_crc64_ECMA_table();
    uint64_t crc = 0;
    while (*str)
    {
        crc = __crc64_table_ECMA[(crc ^ *str) & 0xff] ^ (crc >> 8);
        ++str;
    }
    return crc;
}

uint32_t hash_elf_unix(const unsigned char *str)
{
    uint32_t h = 0, high;
    while (*str)
    {
        h = (h << 4) + *str++;
        if (high = h & 0xF0000000)
            h ^= high >> 24;
        h &= ~high;
    }
    return h;
}

uint64_t hash_djb2(const unsigned char *str)
{
    uint64_t hash = 5381;
    while (*str)
        hash = ((hash << 5) + hash) + *(str++);
    return hash;
}

uint64_t hash_bkdr(const unsigned char *str)
{
    uint64_t seed = 0x00000083ULL;
    uint64_t hash = 0;
    while (*str)
        hash = hash * seed + *(str++);
    return hash;
}
