#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_head = malloc(sizeof(struct list_head));
    if (!new_head) {
        return NULL;
    }
    INIT_LIST_HEAD(new_head);
    return new_head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *iterator_pointer, *safe;
    list_for_each_entry_safe (iterator_pointer, safe, head, list) {
        free(iterator_pointer->value);
        free(iterator_pointer);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *node = malloc(sizeof(element_t));
    if (!node) {
        return false;
    }
    INIT_LIST_HEAD(&node->list);
    node->value = strdup(s);
    list_add(&node->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *node = malloc(sizeof(element_t));
    if (!node) {
        return false;
    }
    INIT_LIST_HEAD(&node->list);
    node->value = strdup(s);
    list_add_tail(&node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head) || !sp) {
        return NULL;
    }
    element_t *removed_node = list_first_entry(head, element_t, list);
    list_del(head->next);
    memcpy(sp, removed_node->value, bufsize);
    sp[bufsize - 1] = '\0';
    return removed_node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head) || !sp) {
        return NULL;
    }
    element_t *removed_node = list_last_entry(head, element_t, list);
    list_del(head->prev);
    memcpy(sp, removed_node->value, bufsize);
    sp[bufsize - 1] = '\0';
    return removed_node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return 0;
    }
    int q_s = 0;
    struct list_head *node;
    list_for_each (node, head)
        q_s++;
    return q_s;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return false;
    }
    int size = q_size(head);
    size /= 2;
    struct list_head *current, *delete_node;
    current = head->next;
    while (--size) {
        current = current->next;
    }
    delete_node = current->next;
    // current->next = current->next->next;
    // current->next->prev = current;
    element_t *node = list_entry(delete_node, element_t, list);
    list_del(delete_node);
    free(node->value);
    free(node);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head)) {
        return false;
    }
    element_t *current, *future;
    bool dup_edge = false;
    list_for_each_entry_safe (current, future, head, list) {
        if (!strcmp(current->value, future->value)) {
            list_del(&current->list);
            free(current->value);
            free(current);
            dup_edge = true;
        } else if (dup_edge) {
            list_del(&current->list);
            free(current->value);
            free(current);
            dup_edge = false;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *current, *future;
    list_for_each_safe (current, future, head) {
        if (future != head && current != head) {
            future->next->prev = current;
            current->next = future->next;
            future->next = current;
            future->prev = current->prev;
            current->prev->next = future;
            current->prev = future;
            future = current->next;
        }
    }
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *current, *future;
    list_for_each_safe (current, future, head) {
        current->next = current->prev;
        current->prev = future;
    }
    current->next = current->prev;
    current->prev = future;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *current_head = head;
    int count = q_size(head) / k;
    struct list_head *current, *future;
    while (count--) {
        int temp = k;
        list_for_each_safe (current, future, current_head) {
            current->next = current->prev;
            current->prev = future;
            if (--temp == 1) {
                break;
            }
        }
        current_head->next->next = future->next;
        future->next->prev = current_head->next;
        current_head->next = future;
        future->prev = current_head;
        future->next = current;
        current_head = current->next;
        // current->next = future->next;
        // current->next->prev = current;
        // future->next = current;
        // current_head->next = future;
        // future->prev = current_head;
        // current_head = current;
        // current = current->next;
        // future = current->next;
    }
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;
    q_reverse(head);
    element_t *current, *future, *max;
    max = list_entry(head->next, element_t, list);
    list_for_each_entry_safe (current, future, head, list) {
        if (strcmp(max->value, current->value) >= 0) {
            max = current;
        } else {
            list_del(&current->list);
            free(current->value);
            free(current);
        }
    }
    q_reverse(head);
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;
    q_reverse(head);
    element_t *current, *future, *max;
    max = list_entry(head->next, element_t, list);
    list_for_each_entry_safe (current, future, head, list) {
        if (strcmp(max->value, current->value) <= 0) {
            max = current;
        } else {
            list_del(&current->list);
            free(current->value);
            free(current);
        }
    }
    q_reverse(head);
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
