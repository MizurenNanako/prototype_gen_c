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

__attribute__((always_inline)) unsigned __calc_pos(char ch)
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

__attribute__((flatten)) unsigned __count_1_before(__bitset_t bset, char ch)
{
    bset &= ((UINT64_C(1) << __calc_pos(ch)) - 1);
    return __count_1(bset);
}

#define __BITSET_EMPTY UINT64_C(0)
#define __bitset_mark(bset, ch) (bset |= (UINT64_C(1) << __calc_pos(ch)))
#define __bitset_unmark(bset, ch) (bset &= (UINT64_C(1) << __calc_pos(ch)))
#define __bitset_at(bset, ch) (bset & (UINT64_C(1) << __calc_pos(ch)))
#define __bitset_reset(bset) (bset = UINT64_C(0))

// store actual links in list to save space.

struct _dnode_t
{
    __bitset_t bitset;
    struct list_t *nodes;
};
struct _dnode_t *_dnode_create() __attribute__((malloc));
void _dnode_free(struct _dnode_t *_dnode);

struct dict_t
{
    struct _dnode_t *root;
};

// !begin _dnode method

struct _dnode_t *_dnode_create()
{
    struct _dnode_t *p =
        (struct _dnode_t *)
            malloc(sizeof(struct _dnode_t));
    if (!p)
        err_malloc;
    p->bitset = __BITSET_EMPTY;
    p->nodes = list_create(_dnode_free);
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
    p->root = _dnode_create();
}
void dict_free(struct dict_t *dict)
{
    if (!dict)
        err_nullptr;
    _dnode_free(dict->root);
    free(dict);
}
