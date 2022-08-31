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
    uint64_t hash = 5381ULL;
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

//-----------------------------------------------------------------------------

#define __ROTL32(x, r) ((x << r) | (x >> (32 - r)));
#define __ROTL64(x, r) ((x << r) | (x >> (64 - r)));

uint32_t hash_murmurhash3_x86_32(const uint8_t *data, int len, uint32_t seed)
{
    const int nblocks = len / 4;
    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    //----------
    // body

    const uint32_t *blocks = (const uint32_t *)(data + nblocks * 4);

    for (int i = -nblocks; i; i++)
    {
        uint32_t k1 = blocks[i];

        k1 *= c1;
        k1 = __ROTL32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = __ROTL32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    //----------
    // tail

    const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);

    uint32_t k1 = 0;

    switch (len & 3)
    {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = __ROTL32(k1, 15);
        k1 *= c2;
        h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;

    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}

__uint128_t hash_murmurhash3_x64_128(const uint8_t *data, int len, uint32_t seed)
{
    const int nblocks = len / 16;
    uint64_t h1 = seed;
    uint64_t h2 = seed;

    const uint64_t c1 = UINT64_C(0x87c37b91114253d5);
    const uint64_t c2 = UINT64_C(0x4cf5ad432745937f);

    //----------
    // body

    const uint64_t *blocks = (const uint64_t *)(data);

    for (int i = 0; i < nblocks; i++)
    {
        uint64_t k1 = blocks[i * 2 + 0];
        uint64_t k2 = blocks[i * 2 + 1];

        k1 *= c1;
        k1 = __ROTL64(k1, 31);
        k1 *= c2;
        h1 ^= k1;

        h1 = __ROTL64(h1, 27);
        h1 += h2;
        h1 = h1 * 5 + 0x52dce729;

        k2 *= c2;
        k2 = __ROTL64(k2, 33);
        k2 *= c1;
        h2 ^= k2;

        h2 = __ROTL64(h2, 31);
        h2 += h1;
        h2 = h2 * 5 + 0x38495ab5;
    }

    //----------
    // tail

    const uint8_t *tail = (const uint8_t *)(data + nblocks * 16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch (len & 15)
    {
    case 15:
        k2 ^= ((uint64_t)tail[14]) << 48;
    case 14:
        k2 ^= ((uint64_t)tail[13]) << 40;
    case 13:
        k2 ^= ((uint64_t)tail[12]) << 32;
    case 12:
        k2 ^= ((uint64_t)tail[11]) << 24;
    case 11:
        k2 ^= ((uint64_t)tail[10]) << 16;
    case 10:
        k2 ^= ((uint64_t)tail[9]) << 8;
    case 9:
        k2 ^= ((uint64_t)tail[8]) << 0;
        k2 *= c2;
        k2 = __ROTL64(k2, 33);
        k2 *= c1;
        h2 ^= k2;
    case 8:
        k1 ^= ((uint64_t)tail[7]) << 56;
    case 7:
        k1 ^= ((uint64_t)tail[6]) << 48;
    case 6:
        k1 ^= ((uint64_t)tail[5]) << 40;
    case 5:
        k1 ^= ((uint64_t)tail[4]) << 32;
    case 4:
        k1 ^= ((uint64_t)tail[3]) << 24;
    case 3:
        k1 ^= ((uint64_t)tail[2]) << 16;
    case 2:
        k1 ^= ((uint64_t)tail[1]) << 8;
    case 1:
        k1 ^= ((uint64_t)tail[0]) << 0;
        k1 *= c1;
        k1 = __ROTL64(k1, 31);
        k1 *= c2;
        h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;
    h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 ^= h1 >> 33;
    h1 *= UINT64_C(0xff51afd7ed558ccd);
    h1 ^= h1 >> 33;
    h1 *= UINT64_C(0xc4ceb9fe1a85ec53);
    h1 ^= h1 >> 33;

    h2 ^= h2 >> 33;
    h2 *= UINT64_C(0xff51afd7ed558ccd);
    h2 ^= h2 >> 33;
    h2 *= UINT64_C(0xc4ceb9fe1a85ec53);
    h2 ^= h2 >> 33;

    h1 += h2;
    h2 += h1;

    return (((__uint128_t)h1) << 64) | ((__uint128_t)h2);
}