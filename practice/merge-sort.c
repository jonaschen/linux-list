#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

#include "common.h"

static uint16_t values[256];

static void list_mergesort_2_items(struct list_head *head)
{
    struct list_head *left, *right;
    struct listitem *iteml, *itemr;

    left = head->prev;
    right = head->next;

    iteml = list_entry(left, struct listitem, list);
    itemr = list_entry(right, struct listitem, list);
    if (cmpint(&iteml->i, &itemr->i) < 0) {
        list_move(&iteml->list, head);
    }

    return;
}

static void list_mergesort_divide(struct list_head *head,
                                  struct list_head *left_half,
                                  struct list_head *right_half)
{
    struct list_head *left, *right;

    left = head->prev;
    right = head->next;

    while (right->next != left && right != left) {
        right = right->next;
        left = left->prev;
    }

    list_cut_position(right_half, head, right);
    list_cut_position(left_half, head, head->prev);

    INIT_LIST_HEAD(head);
}

static void list_mergesort_merge(struct list_head *head,
                                 struct list_head *left,
                                 struct list_head *right)
{
    struct listitem *iteml, *itemr, *next;

    while (!list_empty(left) && !list_empty(right)) {
        iteml = list_entry(left->next, struct listitem, list);
        itemr = list_entry(right->next, struct listitem, list);
        if (cmpint(&iteml->i, &itemr->i) < 0) {
            list_del(&iteml->list);
            list_add_tail(&iteml->list, head);
        } else {
            list_del(&itemr->list);
            list_add_tail(&itemr->list, head);
        }
    }

    if (!list_empty(right)) {
        list_for_each_entry_safe (itemr, next, right, list) {
            list_del(&itemr->list);
            list_add_tail(&itemr->list, head);
        }
    }

    if (!list_empty(left)) {
        list_for_each_entry_safe (iteml, next, left, list) {
            list_del(&iteml->list);
            list_add_tail(&iteml->list, head);
        }
    }
}

static void list_mergesort(struct list_head *head)
{
    struct list_head list_left, list_right;
    struct list_head *left = &list_left, *right = &list_right;
    struct listitem *iteml, *itemr, *next;

    if (!head || (head->next == head->prev))
        return;

    if (head->next->next == head->prev) {
        list_mergesort_2_items(head);
        return;
    }

    INIT_LIST_HEAD(left);
    INIT_LIST_HEAD(right);
    list_mergesort_divide(head, left, right);

    list_mergesort(right);
    list_mergesort(left);

    list_mergesort_merge(head, left, right);
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item = NULL, *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);
    list_mergesort(&testlist);

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));

    return 0;
}
