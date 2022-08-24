#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include "err.h"

struct stack_t *stack_create(void)
{
    struct stack_t *p =
        (struct stack_t *)
            malloc(sizeof(struct stack_t));
    if (!p)
        err_malloc;
    p->size = 0;
    p->_max_size = __STACK_INIT_SIZE;
    p->data = (byte *)
        malloc(sizeof(byte) * __STACK_INIT_SIZE);
    return p;
}

void stack_free(struct stack_t *src)
{
    free(src->data);
    free(src);
    src = NULL;
}

void stack_reserve(struct stack_t *dst, size_t size)
{
    if (!dst)
        err_nullptr;
    if (size <= dst->_max_size)
        return;
    byte *t = (byte *)malloc(size);
    if (!t)
        err_malloc;
    memcpy(t, dst->data, dst->size);
    free(dst->data);
    dst->data = t;
}

void stack_push_s(struct stack_t *dst, void *src, size_t size)
{
    if (!dst || !src)
        err_nullptr;
    if (dst->size + size < dst->_max_size)
    {
        // no need malloc
        memcpy(dst->data + dst->size, src, size);
    }
    else
    {
        // need malloc
        byte *tmp = (byte *)
            malloc(dst->_max_size + size);
        if (!tmp)
            err_malloc;
        memcpy(tmp, dst->data, dst->size);
        memcpy(tmp + dst->size, src, size);
        free(dst->data);
        dst->data = tmp;
    }
    dst->size += size;
}

void stack_pop_s(struct stack_t *src, size_t size)
{
    if (!src)
        err_nullptr;
    src->size -= size;
    if (src->_max_size > 64 &&
        src->size < src->_max_size / 2)
    // free some memory
    {
        byte *tmp = (byte *)
            malloc(src->_max_size / 4 * 3);
        if (!tmp)
            err_malloc;
        memcpy(tmp, src->data, src->size);
        free(src->data);
        src->data = tmp;
    }
}

void *stack_top_s(struct stack_t *src, size_t size)
{
    return src->data + src->size - size;
}
