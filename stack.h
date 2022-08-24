#ifndef __stack_h
#define __stack_h

#include "stdafx.h"

#define __STACK_INIT_SIZE 16

struct stack_t
{
    size_t size;
    size_t _max_size;
    byte *data;
};

struct stack_t *stack_create(void);
void stack_free(struct stack_t *src);
void stack_reserve(struct stack_t *dst, size_t size);
void stack_push_s(struct stack_t *dst, void *src, size_t size);
void stack_pop_s(struct stack_t *src, size_t size);
void *stack_top_s(struct stack_t *src, size_t size);

#define stack_push(stack, ptr_src) \
    stack_push_s(stack, (void *)ptr_src, sizeof(*ptr_src))
#define stack_pop(stack, type) \
    stack_pop_s(stack, sizeof(type))
// @return pointer to data
#define stack_top(stack, type) \
    ((type *)stack_top_s(stack, sizeof(type)))
// @return value of data 
#define stack_top_v(stack, type) \
    *stack_top(stack, type)

#endif // __stack_h