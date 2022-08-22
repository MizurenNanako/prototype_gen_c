#include "list.h"
#include <stdlib.h>
#include "err.h"

#define __reset_traveler(list)         \
    {                                  \
        list->traveler = list->sentry; \
        list->traveler_pos &= 0x00;    \
    }

struct list_t *list_create(__dealloc_func_t dealloc_func)
{
    struct list_t *l =
        (struct list_t *)
            malloc(sizeof(struct list_t));
    if (!l)
        err_malloc;
    struct list_node_t *n =
        (struct list_node_t *)
            malloc(sizeof(struct list_node_t));
    if (!n)
        err_malloc;
    n->data = NULL;
    n->prev = n;
    n->next = n;
    l->sentry = n;
    l->size = 0;
    l->traveler = l->sentry;
    l->traveler_pos = 0;
    l->__data_dealloc_func = dealloc_func;
    return l;
}

void list_clear(struct list_t *list)
{
    if (!list)
        err_nullptr;
    struct list_node_t *p1, *p2, *pt;
    pt = list->sentry;
    p1 = pt->next;
    p2 = pt->prev;
    pt->next = pt->prev = pt;
    // two bread clapping cheese
    while (p1 != p2)
    {
        pt = p1;
        p1 = p1->next;
        (list->__data_dealloc_func)(pt->data);
        free(pt);
        if (p1 == p2)
            break;
        pt = p2;
        p2 = p2->prev;
        (list->__data_dealloc_func)(pt->data);
        free(pt);
    }
    (list->__data_dealloc_func)(p1->data);
    free(p1);
    list->size &= 0x00;
    list->traveler_pos &= 0x00;
    list->traveler = list->sentry;
}

void list_free(struct list_t *list)
{
    // no need to check nullptr,
    // list_clear() will check it.
    list_clear(list);
    free(list->sentry);
    free(list);
}

void list_push_back(struct list_t *list, void *data)
{
    struct list_node_t *p =
        (struct list_node_t *)
            malloc(sizeof(struct list_node_t));
    if (!p)
        err_malloc;
    p->data = data;
    p->next = list->sentry;
    p->prev = list->sentry->prev;
    p->next->prev = p;
    p->prev->next = p;
    ++list->size;
}

void list_pop_back(struct list_t *list)
{
    if (!list)
        err_nullptr;
    struct list_node_t *p = list->sentry->prev;
    if (p == p->next)
        err_out_of_range(index);
    if (list->traveler == p)
        __reset_traveler(list);
    p->next->prev = p->prev;
    p->prev->next = p->next;
    (list->__data_dealloc_func)(p->data);
    free(p);
    --list->size;
}

void *list_back_p(struct list_t *list)
{
    if (!list)
        err_nullptr;
    struct list_node_t *p = list->sentry->prev;
    if (p == p->next)
        err_out_of_range(index);
    return p->data;
}

void list_push_front(struct list_t *list, void *data)
{
    struct list_node_t *p =
        (struct list_node_t *)
            malloc(sizeof(struct list_node_t));
    if (!p)
        err_malloc;
    p->data = data;
    p->prev = list->sentry;
    p->next = list->sentry->next;
    p->next->prev = p;
    p->prev->next = p;
    ++list->size;
}

void list_pop_front(struct list_t *list)
{
    if (!list)
        err_nullptr;
    struct list_node_t *p = list->sentry->next;
    if (p == p->prev)
        err_out_of_range(index);
    if (list->traveler == p)
        __reset_traveler(list);
    p->next->prev = p->prev;
    p->prev->next = p->next;
    (list->__data_dealloc_func)(p->data);
    free(p);
    --list->size;
}

void *list_front_p(struct list_t *list)
{
    if (!list)
        err_nullptr;
    struct list_node_t *p = list->sentry->next;
    if (p == p->prev)
        err_out_of_range(index);
    return p->data;
}

void *list_at_p(struct list_t *list, size_t index)
{
    if (!list)
        err_nullptr;
    if (index >= list->size)
        err_out_of_range(index);
    struct list_node_t *p;
    size_t t = list->traveler_pos;
    if (t && __absolute_dest(t, index) <= index / 2)
    {
        p = list->traveler;
        while (t > index)
        // from: trav, dir: prev
        {
            p = p->prev;
            --t;
        }
        while (t < index)
        // from: trav, dir: next
        {
            p = p->next;
            ++t;
        }
    }
    else if (index < list->size / 2)
    // anti-clockwise
    {
        p = list->sentry->next;
        t = 0;
        while (t < index)
        {
            p = p->next;
            ++t;
        }
    }
    else
    // clockwise
    {
        p = list->sentry->prev;
        t = list->size - 1;
        while (t > index)
        {
            p = p->prev;
            --t;
        }
    }
    list->traveler = p;
    list->traveler_pos = t;
    return p->data;
}