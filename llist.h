
#include <malloc.h>

typedef struct llist_node LLIST_NODE;
struct llist_node {
    LLIST_NODE *next;
    LLIST_NODE *prev;
    void *obj;
};

typedef struct llist {
    LLIST_NODE *first;
    LLIST_NODE *last;
} LLIST;

typedef LLIST_ITER LLIST_NODE;

LLIST *llist_init();
void llist_kill(LLIST *l);

void llist_append(LLIST *l, void *obj);

void *llist_iterator_init(LLIST *l);
void *llist_iterate(LLIST_ITER *iter);
void *llist_remove_and_iterate(LLIST_ITER *iter);
