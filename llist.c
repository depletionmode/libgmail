
#include "global.h"
#include "llist.h"

LLIST *llist_init()
{
  return calloc( 1, sizeof( LLIST ) );
}

void llist_kill(LLIST *l)
{
    // todo: empty list
    N_FREE(l);
}

void llist_append(LLIST *l, void *obj)
{
    LLIST_NODE *node = calloc( 1, sizeof( LLIST_NODE ) );

    if( !l->first )
        l->first = node;
    else
        l->last->next = node;

    node->prev = l->last;
    l->last = node;
}

void *llist_iterator_init(LLIST *l, LLIST_ITER *iter)
{
    iter = l->first;

    if( iter )
        return iter->obj;

    return NULL;
}

void *llist_iterate(LLIST_ITER *iter)
{
    if( iter ) {
        iter = iter->next;

        if( iter )
            return iter->obj;
    }

    return NULL;
}

void *llist_remove_and_iterate(LLIST_ITER *iter)
{
    if( iter ) {
        void *obj = iter->obj;
        LLIST_NODE *node = iter;

        if( iter->prev )
            iter->prev->next = iter->next;

        if( iter->next )
            iter->next->prev = iter->prev;

        iter = iter->next;

        N_FREE(node);
    }

    return NULL;
}
