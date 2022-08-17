#ifndef __stack_h
#define __stack_h

#include "stdafx.h"

struct stack_t
{
    ulli size;
    ulli _max_size;
    byte *data;
};

struct stack_t *sta_create(void);
void sta_free(struct stack_t *src);
void sta_push_s(struct stack_t *dst, void *src, ulli size);
void sta_pop_s(struct stack_t *src, ulli size);
void *sta_top_s(struct stack_t *src, ulli size);

#define sta_push(stack, ptr_src) \
    sta_push_s(stack, (void *)ptr_src, sizeof(*ptr_src))
#define sta_pop(stack, type) \
    sta_pop_s(stack, sizeof(type))
#define sta_top(stack, type) \
    (type *)sta_top_s(stack, sizeof(type))
#define sta_top_v(stack, type) \
    *sta_top(stack, type)

#endif // __stack_h