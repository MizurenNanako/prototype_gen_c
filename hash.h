#ifndef __hash_h
#define __hash_h

#include "stdafx.h"
#include <stdint.h>

uint64_t hash_polynomial_rolling_53(const unsigned char *str);
uint32_t hash_cyclic_redundancy_check_32(const unsigned char *str);
uint64_t hash_cyclic_redundancy_check_64_ECMA(const unsigned char *str);
uint32_t hash_elf_unix(const unsigned char *str);

#endif // __hash_h