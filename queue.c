#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
        free(node);
        return false;
    }
    INIT_LIST_HEAD(&node->list);
    node->value = strdup(s);
    if (!node->value) {
        free(node->value);
        free(node);
        return false;
    }
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
        free(node);
        return false;
    }
    INIT_LIST_HEAD(&node->list);
    node->value = strdup(s);
    if (!node->value) {
        free(node->value);
        free(node);
        return false;
    }
    list_add_tail(&node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *removed_node = list_first_entry(head, element_t, list);
    list_del(head->next);
    if (sp) {
        memcpy(sp, removed_node->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    return removed_node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *removed_node = list_last_entry(head, element_t, list);
    list_del(head->prev);
    if (sp) {
        memcpy(sp, removed_node->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
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
        if (&future->list != head && !strcmp(current->value, future->value)) {
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
    }
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}
struct list_head *connect_prev(struct list_head *head)
{
    struct list_head *cur, *prev;
    cur = head->next;
    prev = head;
    while (cur) {
        cur->prev = prev;
        cur = cur->next;
        prev = prev->next;
    }
    prev->next = head;
    return prev;
}
struct list_head *mergeTwoLists(struct list_head *first,
                                struct list_head *second)
{
    struct list_head *head, *result;
    if (strcmp(list_entry(first, element_t, list)->value,
               list_entry(second, element_t, list)->value) <= 0) {
        head = first;
        first = first->next;
        if (!first) {
            head->next = second;
        }
    } else {
        head = second;
        second = second->next;
        if (!second) {
            head->next = first;
        }
    }
    result = head;
    while (first && second) {
        if (strcmp(list_entry(first, element_t, list)->value,
                   list_entry(second, element_t, list)->value) <= 0) {
            head->next = first;
            head = head->next;
            first = first->next;
            if (!first) {
                head->next = second;
                break;
            }
        } else {
            head->next = second;
            head = head->next;
            second = second->next;
            if (!second) {
                head->next = first;
                break;
            }
        }
    }
    return result;
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    struct list_head *result = head;
    int listsSize = q_size(head);
    if (!head || !head->next || !listsSize)
        return;
    result = result->next;
    struct list_head *lists[1000000] = {NULL};

    int k = 0;
    while (result != head) {
        if (result->next == head) {
            lists[k] = result;
            lists[k++]->next = NULL;
            break;
        }
        if (strcmp(list_entry(result, element_t, list)->value,
                   list_entry(result->next, element_t, list)->value) <= 0) {
            lists[k] = result;
            do {
                result = result->next;
            } while (result->next != head &&
                     strcmp(list_entry(result, element_t, list)->value,
                            list_entry(result->next, element_t, list)->value) <=
                         0);
            result = result->next;
            result->prev->next = NULL;
        } else {
            struct list_head *temp;
            lists[k] = result;
            result = result->next->next;
            lists[k]->next->next = NULL;
            temp = lists[k]->next;
            lists[k]->next = NULL;
            lists[k] = mergeTwoLists(lists[k], temp);
        }
        k++;
    }
    listsSize = k;
    while (listsSize > 1) {
        for (int i = 0, j = listsSize - 1; i < j; i++, j--) {
            lists[i] = mergeTwoLists(lists[i], lists[j]);
        }
        listsSize = (listsSize + 1) / 2;
    }
    lists[0] = connect_prev(lists[0]);
    head->next = lists[0]->next;
    head->next->prev = head;
    head->prev = lists[0];
    lists[0]->next = head;
    if (descend) {
        q_reverse(head);
    }
    return;
    /* Insertion sort
    if (!head || list_empty(head))
        return;
    struct list_head *current, *future, *temp;
    element_t *compare, *current_value;
    list_for_each_safe (current, future, head) {
        temp = current->prev;
        while (temp != head->prev) {
            compare = list_entry(temp, element_t, list);
            current_value = list_entry(current, element_t, list);
            if (temp == head && temp->next == current) {
                break;
            } else if (temp == head) {
                list_move(current, head);
                current = future->prev;
                break;
            } else if (strcmp(compare->value, current_value->value) > 0) {
                temp = temp->prev;
            } else if (temp->next != current) {
                list_move(current, temp);
                current = future->prev;
                break;
            } else {
                break;
            }
        }
        if (descend) {
            q_reverse(head);
        }
    }
    */
}

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
    if (!head || list_empty(head)) {
        return 0;
    }
    queue_contex_t *result = list_first_entry(head, queue_contex_t, chain);
    queue_contex_t *current = NULL;
    if (list_is_singular(head)) {
        return q_size(result->q);
    }
    list_for_each_entry (current, head, chain) {
        if (current != result) {
            list_splice_init(current->q, result->q);
            result->size = result->size + current->size;
        }
    }
    q_sort(result->q, descend);
    return result->size;
}
void q_shuffle(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    int len = q_size(head);
    struct list_head *new = head->prev;
    while (len > 1) {
        struct list_head *old = head->next;
        int random = rand() % len;
        if (random == len - 1) {
            len--;
            new = new->prev;
            continue;
        }
        for (int i = 0; i < random; i++) {
            old = old->next;
        }
        struct list_head *tail_prev = new->prev;
        struct list_head *node_prev = old->prev;
        if (old->next == new) {
            list_del(old);
            list_add(old, new);
        } else {
            list_del(old);
            list_del(new);
            list_add(old, tail_prev);
            list_add(new, node_prev);
        }
        len--;
        new = old->prev;
    }
}