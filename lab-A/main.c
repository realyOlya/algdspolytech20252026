#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"


XorList* xor_list_create(void) 
{
    XorList* list = (XorList*)calloc(1, sizeof(XorList));
    return list;
}

void xor_list_destroy(XorList* list) 
{
    if (!list) return;  // нету
    XorNode* current = list->head;
    XorNode* prev = NULL;
    while (current != NULL) 
    {
        XorNode* next = (XorNode*)xor_uintptr((uintptr_t)prev, current->xor_ptr);
        free(current->data);
        free(current);
        prev = current;
        current = next;
    }
    free(list);
}

void xor_list_push_back(XorList* list, const char* str) 
{
    if (!list || !str) return;// не то
    XorNode* new_node = (XorNode*)malloc(sizeof(XorNode));
    new_node->data = strdup(str);        
    new_node->xor_ptr = 0;

    if (list->head == NULL) 
    {        
        list->head = new_node;
        list->tail = new_node;
    } 
    else 
    {
        new_node->xor_ptr = (uintptr_t)list->tail;
        list->tail->xor_ptr = list->tail->xor_ptr ^ (uintptr_t)new_node;
        list->tail = new_node;
    }
}

XorNode* xor_list_find(XorList* list, const char* key) 
{
    if (!list || !key) return NULL;// проверка
    XorNode* current = list->head;
    XorNode* prev = NULL;
    while (current != NULL) 
    {
        if (strcmp(current->data, key) == 0) // сравнение, 0 - истина
        {
            return current;
        }
        XorNode* next = (XorNode*)xor_uintptr((uintptr_t)prev, current->xor_ptr);
        prev = current;
        current = next;
    }
    return NULL;
}

int xor_list_remove_by_node(XorList* list, XorNode* node) 
{
    if (!list || !node) return 0;// проверка

    XorNode* current = list->head;
    XorNode* prev = NULL;
    XorNode* found_prev = NULL;
    XorNode* found_next = NULL;

    while (current != NULL)
    {
        if (current == node) 
        {
            found_prev = prev;
            found_next = (XorNode*)xor_uintptr((uintptr_t)prev, current->xor_ptr);
            break;
        }
        XorNode* next = (XorNode*)xor_uintptr((uintptr_t)prev, current->xor_ptr);
        prev = current;
        current = next;
    }
    if (current == NULL) return 0; 


    if (found_prev)
     {
        found_prev->xor_ptr = xor_uintptr(
            xor_uintptr(found_prev->xor_ptr, (uintptr_t)node),
            (uintptr_t)found_next
        );
    }
    if (found_next)
    {
        found_next->xor_ptr = xor_uintptr(
            xor_uintptr(found_next->xor_ptr, (uintptr_t)node),
            (uintptr_t)found_prev
        );
    }

    if (node == list->head) list->head = found_next;
    if (node == list->tail) list->tail = found_prev;

    free(node->data);
    free(node);
    return 1;
}

int xor_list_remove_by_key(XorList* list, const char* key) 
{
    XorNode* node = xor_list_find(list, key);
    if (node == NULL) return 0;
    return xor_list_remove_by_node(list, node);
}

XorNode* xor_list_begin(XorList* list) 
{
    if (list == NULL) 
    {
        return NULL;
    }
    return list->head;
}

XorNode* xor_list_next(XorList* list, XorNode* current, XorNode** prev) 
{
    if (!current || !prev) return NULL;
    XorNode* next = (XorNode*)xor_uintptr((uintptr_t)*prev, current->xor_ptr);
    *prev = current;
    return next;
}