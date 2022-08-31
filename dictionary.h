#ifndef __dictionary_h
#define __dictionary_h

#include "stdafx.h"

struct dict_t;

struct dict_t *dict_create() __attribute__((malloc));
void dict_free(struct dict_t *dict);

#endif // __dictionary_h