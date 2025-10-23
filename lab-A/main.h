#ifndef XOR_LIST_H
#define XOR_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef struct XorNode 
{
    char* data;
    uintptr_t xor_ptr; 
} XorNode;

typedef struct XorList 
{
    XorNode* head;
    XorNode* tail;
} XorList;
// побитовая операция
static inline uintptr_t xor_uintptr(uintptr_t a, uintptr_t b) {
    return a ^ b;
}
// создание списка
XorList* xor_list_create(void);
// удаление списка, освобождение памяти
void xor_list_destroy(XorList* list);
// добавление в конец списка
void xor_list_push_back(XorList* list, const char* str);
// ищет узел по строке KEY
int xor_list_remove_by_key(XorList* list, const char* key);
// удаляет узел по KEY
int xor_list_remove_by_node(XorList* list, XorNode* node);
// удаление по адресу
XorNode* xor_list_find(XorList* list, const char* key);
// возращение первого узла
XorNode* xor_list_begin(XorList* list);
// возвращение следующего узла
XorNode* xor_list_next(XorList* list, XorNode* current, XorNode** prev);

#ifdef __cplusplus
}
#endif

#endif 