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
#include <string.h>

#define DEBUG
const CListElementType INVALID_RETURN = {TOK_END, NULL};

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

    new->element = element;
    new->next = next;

    return new;
}

CList CL_new()
{
    CList list = (CList)malloc(sizeof(struct _clist));

    list->head = NULL;
    list->length = 0;

    return list;
}

void CL_free(CList list)
{
    struct _cl_node *current;
    if (list == NULL)
    {
        return;
    }

    while ((current = list->head) != NULL)
    {
        list->head = current->next;
        free(current);
    }

    free(list);
}

int CL_length(CList list)
{

#ifdef DEBUG

    int len = 0;
    struct _cl_node *node;
    for (node = list->head; node != NULL; node = node->next)
        len++;

#endif

    return list->length;
}

CListElementType CL_pop(CList list)
{

    struct _cl_node *popped_node = list->head;
    CListElementType ret;

    if (popped_node == NULL)
        return INVALID_RETURN;

    ret = popped_node->element;

    list->head = popped_node->next;
    free(popped_node);

    list->length--;

    return ret;
}

bool CL_insert(CList list, CListElementType element, int pos)
{

    int len;
    int i = 0;
    struct _cl_node *curr = list->head;
    struct _cl_node *prev = NULL;
    struct _cl_node *new_node = _CL_new_node(element, NULL);

    len = CL_length(list);

    
    if (pos <= (-1 * len) || pos > len)
    {
        return false;
    }

    if (pos < 0)
    {
        pos = len + pos + 1;
    }

    i = 0;
    curr = list->head;
    prev = NULL;
    new_node = _CL_new_node(element, NULL);

    if (new_node == NULL)
    {
        return false;
    }

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

    new_node->next = curr;
    prev->next = new_node;
    list->length++;

    return true;
}
CList CL_copy(CList list)
{

    CList copy = CL_new();

    struct _cl_node *curr = list->head;
    struct _cl_node *currcpy = NULL;

    while (curr != NULL)
    {

        struct _cl_node *new_node = _CL_new_node(curr->element, NULL);

        if (copy->head == NULL)
        {

            copy->head = new_node;
            currcpy = new_node;
        }
        else
        {
            currcpy->next = new_node;
            currcpy = new_node;
        }
        curr = curr->next;
        copy->length++;
    }
    return copy;
}
void CL_push(CList list, CListElementType element)
{

    list->head = _CL_new_node(element, list->head);
    list->length++;
}
void CL_append(CList list, CListElementType element)
{

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

CListElementType CL_nth(CList list, int pos)
{

    int length = list->length;
    int i = 0;
    struct _cl_node *curr = list->head;

    if (pos < -length || pos >= length)
    {
        return INVALID_RETURN;
    }

    if (pos < 0)
    {
        pos = length + pos;
    }

    

    while (curr != NULL)
    {
        if (i == pos)
        {
            return curr->element;
        }
        curr = curr->next;
        i++;
    }

    return INVALID_RETURN;
}

CListElementType CL_remove(CList list, int pos)
{
    int i;
    int len = CL_length(list);
    struct _cl_node *curr = list->head;
    struct _cl_node *prev = NULL;
    CListElementType removedElement;

    if (pos < -len || pos >= len)
    {
        return INVALID_RETURN;
    }

    if (pos < 0)
    {
        pos += len;
    }

    

    if (!curr)
    {
        return INVALID_RETURN;
    }

    if (pos == 0)
    {
        list->head = curr->next;
        removedElement = curr->element;
        free(curr);
        list->length--;
        return removedElement;
    }

    for (i = 0; curr != NULL && i < pos; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr)
    {
        if (prev)
        {
            prev->next = curr->next;
        }
        removedElement = curr->element;
        free(curr);
        list->length--;
        return removedElement;
    }
    return INVALID_RETURN;
}

void CL_join(CList list1, CList list2)
{

    if (list1->head == NULL)
    {
        list1->head = list2->head;
    }
    else
    {
        struct _cl_node *curr = list1->head;

        while (curr->next != NULL)
        {
            curr = curr->next;
        }

        curr->next = list2->head;
    }

    list1->length += list2->length;

    list2->head = NULL;
    list2->length = 0;
}

void CL_reverse(CList list)
{

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

void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data)
{

    struct _cl_node *curr = list->head;
    int pos = 0;

    while (curr != NULL)
    {
        callback(pos, curr->element, cb_data);
        curr = curr->next;
        pos++;
    }
}