////////////////////////////////
/// usage : 1.	call MakeList() to instantiate the template before using it.
/// 
/// note  : 1.	type MakeList(int) in MakeList.c (with #include "List.h")
///             and add following pre-build event to see the instantiated classes.
///             ```cmd
///             echo #include ^"List.h^" >MakeList.h
///             echo MakeList^(char,free^) >>MakeList.h
///
///             echo #include ^<stdio.h^> >Expand.c
///             echo #include ^<stdlib.h^> >>Expand.c
///             echo #include ^<string.h^> >>Expand.c
///             cl /DSZX_USER_CODE_ONLY /EP MakeList.h >>Expand.c
///
///             "C:\Program Files\LLVM\bin\clang-format" Expand.c >MakeList.h
///             ```
///
///         2.  there is no nullptr check on list.
///         3.  the remove() will change the order of the list.
///         4.  associate copy constructor like the destructor?
///         5.  deep copy/remove/merge is to be implemented.
///         6.  deep resize will not invoke constructor if new size becomes greater.
///
/// example :
///         void freeIntPtr(int *p) { free(p); }
///         
///         MakeList(int,freeIntPtr)
///         
///         void f() {
///             List_SizeType(int) i = 0;
///             List_ItemPtr(int) p = (int*)malloc(sizeof(int));
///             List_Destructor(int) fp = freeIntPtr;
///             List(int) *arr = List_new(int)();
///             int j = 1, k = 2;
///         
///             *p = i;
///             List_pushback(int)(arr, p);
///             List_insert(int)(arr, &j, 0);
///             if ((List_find(int)(arr, &j) == 0) && (List_find(int)(arr, &k) == List_InvalidIndex)) {
///                 printf("%d, %d\n", *List_get(int)(arr, 0), *List_get(int)(arr, 1));
///             }
///             List_resize_deep(int)(arr, 1);
///         
///             List_delete(int)(arr);
///         }
///
////////////////////////////////


#ifndef SZX_CUTILIBS_ARR_H
#define SZX_CUTILIBS_ARR_H


#ifndef SZX_USER_CODE_ONLY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif // !SZX_USER_CODE_ONLY


/// helper types.
typedef int Bool;
enum { False = 0, True = 1 };
enum { List_InvalidIndex = -1 };
enum { List_Nullptr = (int)NULL };


/// class template.
#define List(ItemType)  _public_List_##ItemType

/// member types.
#define List_SizeType(ItemType)  _public_List_##ItemType##_SizeType
#define List_ItemPtr(ItemType)  _public_List_##ItemType##_ItemPtr
#define List_Destructor(ItemType)  _public_List_##ItemType##_Destructor

/// public member methods.
#define List_new(ItemType)  _public_List_##ItemType##_new
#define List_delete_shallow(ItemType)  _public_List_##ItemType##_delete_shallow
#define List_delete_deep(ItemType)  _public_List_##ItemType##_delete_deep
#define List_reserve(ItemType)  _public_List_##ItemType##_reserve
#define List_resize_shallow(ItemType)  _public_List_##ItemType##_resize_shallow
#define List_resize_deep(ItemType)  _public_List_##ItemType##_resize_deep
#define List_empty(ItemType)  _public_List_##ItemType##_empty
#define List_get(ItemType)  _public_List_##ItemType##_get
#define List_set(ItemType)  _public_List_##ItemType##_set
#define List_find(ItemType)  _public_List_##ItemType##_find
#define List_pushback(ItemType)  _public_List_##ItemType##_pushback
#define List_insert(ItemType)  _public_List_##ItemType##_insert
#define List_clear_shallow(ItemType)  _public_List_##ItemType##_clear_shallow
#define List_clear_deep(ItemType)  _public_List_##ItemType##List_clear_deep
#define List_copy_shallow(ItemType)  _public_List_##ItemType##_copy_shallow
#define List_remove_shallow(ItemType)  _public_List_##ItemType##remove_shallow
#define List_merge_shallow(ItemType)  _public_List_##ItemType##_merge_shallow
#define List_reverse(ItemType)  _public_List_##ItemType##_reverse
#define List_shuffle(ItemType)  _public_List_##ItemType##_shuffle


/// template instantiation.
#define MakeList(ItemType,ItemDestructor)\
/* member types. */\
typedef int _public_List_##ItemType##_SizeType;\
typedef ItemType *_public_List_##ItemType##_ItemPtr;\
typedef void (*_public_List_##ItemType##_Destructor)(_public_List_##ItemType##_ItemPtr pItem);\
\
/* class template. */\
typedef struct _public_List_##ItemType{\
    _public_List_##ItemType##_SizeType length;\
    _public_List_##ItemType##_SizeType capacity;\
    ItemType **data;\
} _public_List_##ItemType;\
\
/* private methods. */\
static Bool _private_List_##ItemType##_indexValid(_public_List_##ItemType *list, _public_List_##ItemType##_SizeType index) {\
    return ((0 <= index) && (index < list->length));\
}\
\
static Bool _private_List_##ItemType##_newIndexValid(_public_List_##ItemType *list, _public_List_##ItemType##_SizeType index) {\
    return ((0 <= index) && (index <= list->length));\
}\
\
static void _private_List_##ItemType##_deleteItems(_public_List_##ItemType *list, _public_List_##ItemType##_SizeType begin, _public_List_##ItemType##_SizeType end) {\
    _public_List_##ItemType##_Destructor destructor = (_public_List_##ItemType##_Destructor)ItemDestructor;\
    if (destructor != (_public_List_##ItemType##_Destructor)List_Nullptr) {\
        for (; begin < end; ++begin) { destructor(list->data[begin]); }\
    }\
}\
\
static void _private_List_##ItemType##_swapItems(_public_List_##ItemType *list, _public_List_##ItemType##_SizeType index0, _public_List_##ItemType##_SizeType index1) {\
    _public_List_##ItemType##_ItemPtr tmp = list->data[index0];\
    list->data[index0] = list->data[index1];\
    list->data[index1] = tmp;\
}\
\
/* public methods. */\
static _public_List_##ItemType* _public_List_##ItemType##_new() {\
    _public_List_##ItemType *list = (_public_List_##ItemType*)malloc(sizeof(_public_List_##ItemType));\
    list->capacity = 0;\
    list->length = 0;\
    list->data = (_public_List_##ItemType##_ItemPtr*)List_Nullptr;\
    return list;\
}\
\
static void _public_List_##ItemType##_delete_shallow(_public_List_##ItemType *list) {\
    free(list->data);\
    free(list);\
}\
static void _public_List_##ItemType##_delete_deep(_public_List_##ItemType *list) {\
    _private_List_##ItemType##_deleteItems(list, 0, list->length);\
    _public_List_##ItemType##_delete_shallow(list);\
}\
\
static void _public_List_##ItemType##_reserve(_public_List_##ItemType *list, _public_List_##ItemType##_SizeType newCapacity) {\
    if (newCapacity < list->capacity) { return; }\
    list->data = (_public_List_##ItemType##_ItemPtr*)realloc(list->data, newCapacity * sizeof(_public_List_##ItemType##_ItemPtr));\
    list->capacity = newCapacity;\
}\
\
static void _public_List_##ItemType##_resize_shallow(_public_List_##ItemType *list, _public_List_##ItemType##_SizeType newSize) {\
    _public_List_##ItemType##_reserve(list, newSize);\
    list->length = newSize;\
}\
\
static void _public_List_##ItemType##_resize_deep(_public_List_##ItemType *list, _public_List_##ItemType##_SizeType newSize) {\
    _private_List_##ItemType##_deleteItems(list, newSize, list->length);\
    _public_List_##ItemType##_resize_shallow(list, newSize);\
}\
\
static Bool _public_List_##ItemType##_empty(_public_List_##ItemType *list) {\
    return (list->length == 0);\
}\
\
static _public_List_##ItemType##_ItemPtr _public_List_##ItemType##_get(_public_List_##ItemType *list, _public_List_##ItemType##_SizeType index) {\
    if (!_private_List_##ItemType##_indexValid(list, index)) { return NULL; }\
    return list->data[index];\
}\
\
static Bool _public_List_##ItemType##_set(_public_List_##ItemType *list, _public_List_##ItemType##_ItemPtr pItem, _public_List_##ItemType##_SizeType index) {\
    if (!_private_List_##ItemType##_indexValid(list, index)) { return False; }\
    list->data[index] = pItem;\
    return True;\
}\
\
static _public_List_##ItemType##_SizeType _public_List_##ItemType##_find(_public_List_##ItemType *list, _public_List_##ItemType##_ItemPtr pItem) {\
    _public_List_##ItemType##_SizeType i;\
    for (i = 0; i < list->length; ++i) {\
        if (list->data[i] == pItem) { return i; }\
    }\
    return List_InvalidIndex;\
}\
\
static void _public_List_##ItemType##_pushback(_public_List_##ItemType *list, _public_List_##ItemType##_ItemPtr pItem) {\
    if (list->length == list->capacity) { _public_List_##ItemType##_reserve(list, list->length * 2); }\
    list->data[list->length] = pItem;\
    ++list->length;\
}\
\
static Bool _public_List_##ItemType##_insert(_public_List_##ItemType *list, _public_List_##ItemType##_ItemPtr pItem, _public_List_##ItemType##_SizeType index) {\
    _public_List_##ItemType##_SizeType i;\
    if (!_private_List_##ItemType##_newIndexValid(list, index)) { return False; }\
    if (list->length == list->capacity) { _public_List_##ItemType##_reserve(list, list->capacity * 2); }\
    for (i = list->length; i > index; --i) { list->data[i] = list->data[i - 1]; }\
    list->data[index] = pItem;\
    ++list->length;\
    return True;\
}\
\
static void _public_List_##ItemType##_clear_shallow(_public_List_##ItemType *list) {\
    list->length = 0;\
}\
static void _public_List_##ItemType##_clear_deep(_public_List_##ItemType *list) {\
    _public_List_##ItemType##_resize_deep(list, 0);\
}\
\
static void _public_List_##ItemType##_copy_shallow(_public_List_##ItemType *srcList, _public_List_##ItemType *dstList) {\
    _public_List_##ItemType##_resize_shallow(dstList, srcList->length);\
    dstList->length = srcList->length;\
    memcpy(dstList->data, srcList->data, srcList->length * sizeof(_public_List_##ItemType##_ItemPtr));\
}\
\
static void _public_List_##ItemType##_remove_shallow(_public_List_##ItemType *list, _public_List_##ItemType##_SizeType index) {\
    if (!_private_List_##ItemType##_indexValid(list, index)) { return; }\
\
    list->data[index] = list->data[list->length - 1];\
    --list->length;\
}\
\
static void _public_List_##ItemType##_merge_shallow(_public_List_##ItemType *listA, _public_List_##ItemType *listB) {\
    _public_List_##ItemType##_resize_shallow(listA, listA->length + listB->length);\
    memcpy(listA->data + listA->length, listB->data, sizeof(_public_List_##ItemType##_ItemPtr) * listB->length);\
}\
\
static void _public_List_##ItemType##_reverse(_public_List_##ItemType *list) {\
    _public_List_##ItemType##_SizeType i, j;\
    for (i = 0, j = list->length - 1; i < j; ++i, --j) {\
        _private_List_##ItemType##_swapItems(list, i, j);\
    }\
}\
\
static void _public_List_##ItemType##_shuffle(_public_List_##ItemType *list) {\
    _public_List_##ItemType##_SizeType i;\
    for (i = list->length; i > 1; --i) {\
        _private_List_##ItemType##_swapItems(list, (i - 1), (rand() % i));\
    }\
}

#endif // SZX_CUTILIBS_ARR_H
