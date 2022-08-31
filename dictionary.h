#ifndef __dictionary_h
#define __dictionary_h

#include "stdafx.h"

struct dict_t;

struct dict_t *dict_create() __attribute__((malloc));
void dict_free(struct dict_t *dict);

void dict_insert(struct dict_t *dict, const char *word);

#endif // __dictionary_h