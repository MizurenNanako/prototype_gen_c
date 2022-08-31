#include "dictionary.h"
#include <stdint.h>
#include "list.h"
#include "err.h"

// '$'      -   36      -   1
// '0'~'9'  -   48~57   -   10
// 'A'~'Z'  -   65~90   -   26
// '_'      -   95      -   1
// 'a'~'z'  -   97~122  -   26
//          -           -   64

// since there are only 64 probability
// store it with a bitset of int64

// mapping :
// 'a'~'z' -> 0~25
// 'A'~'Z' -> 26~51
// '0'~'9' -> 52~61
// '_'     -> 62
// '$'     -> 63

typedef uint64_t __bitset_t;

__attribute__((always_inline)) unsigned __calc_id(char ch)
{
    // calc the real mapping pos
    if (ch == '_')
        return 62;
    else if ((ch >= 'a') && (ch <= 'z'))
    {
        return ch - 'a';
    }
    else if ((ch >= 'A') && (ch <= 'Z'))
    {
        return ch - 'A' + 26;
    }
    else if ((ch >= '0') && (ch <= '9'))
    {
        return ch - '0' + 52;
    }
    else if (ch == '$')
        return 63;
    else
        return -1; // noreturn, should not be called
}

__attribute__((always_inline)) unsigned __count_1(__bitset_t bset)
{
    // count 1 in bitset
    unsigned cnt = 0;
    while (bset)
    {
        bset &= (bset - 1);
        ++cnt;
    }
    return cnt;
}

__attribute__((flatten)) unsigned __calc_index(__bitset_t bset, char ch)
{
    bset &= ((UINT64_C(1) << __calc_id(ch)) - 1);
    return __count_1(bset);
}

#define __BITSET_EMPTY UINT64_C(0)
#define __bitset_mark(bset, index) (bset |= (UINT64_C(1) << index))
#define __bitset_unmark(bset, index) (bset &= (UINT64_C(1) << index))
#define __bitset_at(bset, index) (bset & (UINT64_C(1) << index))
#define __bitset_reset(bset) (bset = UINT64_C(0))

// store actual links in list to save space.

struct _dnode_t
{
    __bitset_t bitset;
    struct list_t *nodes;
    unsigned id;
    bool isend;
};
struct _dnode_t *_dnode_create() __attribute__((malloc));
void _dnode_free(struct _dnode_t *_dnode);

struct dict_t
{
    struct _dnode_t *root;
};

// !begin _dnode method

struct _dnode_t *_dnode_create(unsigned id)
{
    struct _dnode_t *p =
        (struct _dnode_t *)
            malloc(sizeof(struct _dnode_t));
    if (!p)
        err_malloc;
    p->bitset = __BITSET_EMPTY;
    p->nodes = list_create((__dealloc_func_t)_dnode_free);
    p->id = id;
    p->isend = false;
    return p;
}

void _dnode_free(struct _dnode_t *_dnode)
{
    if (!_dnode)
        err_nullptr;
    list_free(_dnode->nodes);
    free(_dnode);
}

// !end _dnode method

struct dict_t *dict_create()
{
    struct dict_t *p =
        (struct dict_t *)
            malloc(sizeof(struct dict_t));
    if (!p)
        err_malloc;
    p->root = _dnode_create(0); // value 0 does not matter
    return p;
}
void dict_free(struct dict_t *dict)
{
    if (!dict)
        err_nullptr;
    _dnode_free(dict->root);
    free(dict);
}
void dict_clear(struct dict_t *dict)
{
    if (!dict)
        err_nullptr;
    _dnode_free(dict->root);
    dict->root = _dnode_create(0);
}

bool dict_insert_r(struct _dnode_t *node, const char *word)
{
    char ch = *word;
    unsigned id = __calc_id(ch);
    __bitset_t bset = node->bitset;
    struct list_t *list = node->nodes;
    if (!ch) // at end of the string
    {
        // already marked
        if (node->isend)
            return false;
        // mark end and return
        else
            return node->isend = true;
    }
    if (__bitset_at(bset, id)) // the char exists
    {
        // go deeper
        struct _dnode_t *deeper =
            list_at(list, __calc_index(bset, ch), struct _dnode_t);
        return dict_insert_r(deeper, word + 1);
    }
    else // the char does not exist
    {
        // mark
        __bitset_mark(node->bitset, __calc_id(ch));
        // create node
        struct _dnode_t *p =
            (struct _dnode_t *)
                malloc(sizeof(struct _dnode_t));
        if (!p)
            err_malloc;
        p = _dnode_create(id);

        size_t list_len = list_size(list);
        if (list_len) // list has item(s)
        {
            unsigned i = 0;
            while (i < list_len && list_at(list, i, struct _dnode_t)->id < id)
                ++i;
            if (i == list_len) // the last one
            {
                list_push_back(list, p);
                // now go deeper
                return dict_insert_r(p, word + 1);
            }
            else // in middle or at front
            {
                list_insert_before(list, i, p);
                // now go deeper
                return dict_insert_r(p, word + 1);
            }
        }
        else // list has no item
        {
            list_push_back(list, p);
            // now go deeper
            return dict_insert_r(p, word + 1);
        }
    }
}

bool dict_insert(struct dict_t *dict, const char *word)
{
    return dict_insert_r(dict->root, word);
}
