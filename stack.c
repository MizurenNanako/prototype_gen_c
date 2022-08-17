#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include "err.h"

struct stack_t *sta_create(void)
{
    struct stack_t *p =
        (struct stack_t *)
            malloc(sizeof(struct stack_t));
    if (!p)
        err_malloc;
    p->size = 0;
    p->_max_size = 20;
    p->data = (byte *)
        malloc(sizeof(byte) * 20);
    return p;
}

void sta_free(struct stack_t *src)
{
    free(src->data);
    free(src);
    src = NULL;
}

void sta_push_s(struct stack_t *dst, void *src, ulli size)
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

void sta_pop_s(struct stack_t *src, ulli size)
{
    if (!src)
        err_nullptr;
    src->size -= size;
    if (src->_max_size > 32)
        // free some memory
        if (src->size < src->_max_size / 2)
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

void *sta_top_s(struct stack_t *src, ulli size)
{
    return src->data + src->size - size;
}
