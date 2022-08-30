#ifndef __hash_h
#define __hash_h

#include "stdafx.h"
#include <stdint.h>

uint64_t hash_polynomial_rolling_53(const unsigned char *str);
uint32_t hash_cyclic_redundancy_check_32(const unsigned char *str);
uint64_t hash_cyclic_redundancy_check_64_ECMA(const unsigned char *str);
uint32_t hash_elf_unix(const unsigned char *str);
uint64_t hash_djb2(const unsigned char *str);
uint64_t hash_bkdr(const unsigned char *str);

uint32_t hash_murmurhash3_x86_32(const uint8_t *data, int len, uint32_t seed);
__uint128_t hash_murmurhash3_x64_128(const uint8_t *data, int len, uint32_t seed);

#endif // __hash_h