#ifndef __list_h
#define __list_h

#include "stdafx.h"

typedef void (*__dealloc_func_t)(void *);

struct list_node_t
{
    struct list_node_t *prev;
    struct list_node_t *next;
    void *data;
};

struct list_t
{
    size_t size;
    struct list_node_t *sentry;
    struct list_node_t *traveler;
    size_t traveler_pos;
    __dealloc_func_t __data_dealloc_func;
};

/**
 * @param dealloc_func pointer to funtion
 * to free this type of data
 * @return pointer to list
 */
struct list_t *list_create(__dealloc_func_t dealloc_func);
void list_clear(struct list_t *list);
void list_free(struct list_t *list);

void list_push_back(struct list_t *list, void *data_move);
void list_pop_back(struct list_t *list);
void *list_back_p(struct list_t *list);
// @return pointer to data
#define list_back(list, type) ((type *)list_back_p(list))
// @return value of data
#define list_back_v(list, type) (*list_back(list, type))
void list_push_front(struct list_t *list, void *data_move);
void list_pop_front(struct list_t *list);
void *list_front_p(struct list_t *list);
// @return pointer to data
#define list_front(list, type) ((type *)list_front_p(list))
// @return value of data
#define list_front_v(list, type) (*list_front(list, type))
void *list_at_p(struct list_t *list, size_t index);

#endif // __list_h
