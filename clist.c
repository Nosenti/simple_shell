/*
 * clist.c
 *
 * Linked list implementation
 *
 * Author: Innocent Ingabire
 */
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define DEBUG

struct _cl_node
{
    CListElementType element;
    struct _cl_node *next;
};

struct _clist
{
    struct _cl_node *head;
    int length;
};

static struct _cl_node *

_CL_new_node(CListElementType element, struct _cl_node *next)
{
    struct _cl_node *new = (struct _cl_node *)malloc(sizeof(struct _cl_node));

    assert(new);

    new->element = element;
    new->next = next;

    return new;
}

// Documented in .h file
CList CL_new()
{
    CList list = (CList)malloc(sizeof(struct _clist));
    assert(list);

    list->head = NULL;
    list->length = 0;

    return list;
}

// Documented in .h file
void CL_free(CList list) {
    if (list == NULL) {
        return;
    }

    struct _cl_node *current;
    while ((current = list->head) != NULL) {
        list->head = current->next;
        free(current);
    }

    free(list);
}



// Documented in .h file
int CL_length(CList list)
{
    assert(list);
#ifdef DEBUG
    // In production code, we simply return the stored value for
    // length. However, as a defensive programming method to prevent
    // bugs in our code, in DEBUG mode we walk the list and ensure the
    // number of elements on the list is equal to the stored length.

    int len = 0;
    for (struct _cl_node *node = list->head; node != NULL; node = node->next)
        len++;
    assert(len == list->length);
#endif // DEBUG

    return list->length;
}

// Documented in .h file


// Documented in .h file
CListElementType CL_pop(CList list)
{
    assert(list);
    struct _cl_node *popped_node = list->head;

    if (popped_node == NULL)
        return INVALID_RETURN;

    CListElementType ret = popped_node->element;

    // unlink previous head node, then free it
    list->head = popped_node->next;
    free(popped_node);
    // we cannot refer to popped node any longer

    list->length--;

    return ret;
}

// Documented in .h file
bool CL_insert(CList list, CListElementType element, int pos)
{
    assert(list);

    int len = CL_length(list);

    if (pos <= (-1 * len) || pos > len)
    {
        return false;
    }

    // adjust pos if it is negative
    if (pos < 0)
    {
        pos = len + pos + 1;
    }

    unsigned int i = 0;
    struct _cl_node *curr = list->head;
    struct _cl_node *prev = NULL;
    struct _cl_node *new_node = _CL_new_node(element, NULL);

    if (new_node == NULL)
    {
        return false; // Failure of memory allocation
    }

    // Insert at the start of the list
    if (pos == 0)
    {
        new_node->next = list->head;
        list->head = new_node;
        list->length++;
        return true;
    }

    while (curr != NULL && i < pos)
    {
        prev = curr;
        curr = curr->next;
        i++;
    }

    // Insert in the middle or at the end
    new_node->next = curr;
    prev->next = new_node;
    list->length++;

    return true;
}
CList CL_copy(CList list)
{
    assert(list);

    CList copy = CL_new(); // new list

    // pointers to the current node in original and copied lists
    struct _cl_node *curr = list->head;
    struct _cl_node *currcpy = NULL;

    while (curr != NULL)
    {
        // allocate a new node and copy the element
        struct _cl_node *new_node = _CL_new_node(curr->element, NULL);

        if (copy->head == NULL)
        {
            // if the first node, set it as the head of the copied list
            copy->head = new_node;
            currcpy = new_node;
        }
        else
        {
            // link the new node to the copied list
            currcpy->next = new_node;
            currcpy = new_node;
        }

        // next node in the original list
        curr = curr->next;
        copy->length++;
    }
    return copy;
}
void CL_push(CList list, CListElementType element)
{
    assert(list);
    list->head = _CL_new_node(element, list->head);
    list->length++;
}
void CL_append(CList list, CListElementType element)
{
    assert(list);

    struct _cl_node *new_node = _CL_new_node(element, NULL);
    if (!new_node)
    {
        return;
    }

    if (list->head == NULL)
    {
        list->head = new_node;
    }
    else
    {
        struct _cl_node *curr = list->head;

        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = new_node;
    }
    list->length++;
}

// Documented in .h file
CListElementType CL_nth(CList list, int pos)
{
    assert(list);

    int length = list->length;

    // Check for out-of-range positions
    if (pos < -length || pos >= length)
    {
        return INVALID_RETURN;
    }

    // If pos is negative, adjust it to fetch from the end
    if (pos < 0)
    {
        pos = length + pos; // Convert negative index to equivalent positive index
    }

    int i = 0;
    struct _cl_node *curr = list->head;

    while (curr != NULL)
    {
        if (i == pos)
        {
            return curr->element;
        }
        curr = curr->next; // Move to the next node in the list
        i++;
    }

    return INVALID_RETURN;
}

// Documented in .h file

// Documented in .h file
CListElementType CL_remove(CList list, int pos)
{
    assert(list);

    int len = CL_length(list);

    // Out of bounds
    if (pos < -len || pos >= len)
    {
        return INVALID_RETURN;
    }

    // Adjust pos if it's negative
    if (pos < 0)
    {
        pos += len;
    }

    struct _cl_node *curr = list->head;
    struct _cl_node *prev = NULL;
    CListElementType removedElement;

    // If the list is empty
    if (!curr)
    {
        return INVALID_RETURN;
    }

    // If the element to remove is the head node
    if (pos == 0)
    {
        list->head = curr->next;
        removedElement = curr->element;
        free(curr);
        list->length--; // Don't forget to decrease the length of the list.
        return removedElement;
    }

    for (int i = 0; curr != NULL && i < pos; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr)
    {
        if (prev) // It's always good to be defensive
        {
            prev->next = curr->next;
        }
        removedElement = curr->element;
        free(curr);
        list->length--; // Don't forget to decrease the length of the list.
        return removedElement;
    }

    // If we reached here, something unexpected happened.
    return INVALID_RETURN;
}

// Documented in .h file



// Documented in .h file
void CL_join(CList list1, CList list2)
{
    assert(list1);
    assert(list2);

    if (list1->head == NULL)
    {
        list1->head = list2->head;
    }
    else
    {
        struct _cl_node *curr = list1->head;

        // Traverse to the end of list1
        while (curr->next != NULL)
        {
            curr = curr->next;
        }

        // Points the last node of list1 to the head of list2
        curr->next = list2->head;
    }

    // Adjust the length of list1
    list1->length += list2->length;

    // Clear list2 without freeing the nodes since they are now in list1
    list2->head = NULL;
    list2->length = 0;
}

// Documented in .h file
void CL_reverse(CList list)
{
    assert(list);
    struct _cl_node *p2 = list->head;
    struct _cl_node *p1 = NULL;
    struct _cl_node *p3;

    while (p2 != NULL)
    {
        p3 = p2->next;
        p2->next = p1;
        p1 = p2;
        p2 = p3;
    }
    list->head = p1;
}

// Documented in .h file
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data)
{
    assert(list);

    struct _cl_node *curr = list->head;
    int pos = 0;

    while (curr != NULL)
    {
        callback(pos, curr->element, cb_data);
        curr = curr->next;
        pos++;
    }
}

