#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Bool;
enum { False = 0, True = 1 };
enum { List_InvalidIndex = -1 };
enum { List_Nullptr = (int)NULL };

typedef int _public_List_char_SizeType;
typedef char *_public_List_char_ItemPtr;
typedef void (*_public_List_char_Destructor)(_public_List_char_ItemPtr pItem);

typedef struct _public_List_char {
    _public_List_char_SizeType length;
    _public_List_char_SizeType capacity;
    char **data;
} _public_List_char;


static Bool _private_List_char_indexValid(_public_List_char *list,
    _public_List_char_SizeType index) {
    return ((0 <= index) && (index < list->length));
}

static Bool _private_List_char_newIndexValid(_public_List_char *list,
    _public_List_char_SizeType index) {
    return ((0 <= index) && (index <= list->length));
}

static void _private_List_char_deleteItems(_public_List_char *list,
    _public_List_char_SizeType begin,
    _public_List_char_SizeType end) {
    _public_List_char_Destructor destructor = (_public_List_char_Destructor)free;
    if (destructor != (_public_List_char_Destructor)List_Nullptr) {
        for (; begin < end; ++begin) {
            destructor(list->data[begin]);
        }
    }
}

static void _private_List_char_swapItems(_public_List_char *list,
    _public_List_char_SizeType index0,
    _public_List_char_SizeType index1) {
    _public_List_char_ItemPtr tmp = list->data[index0];
    list->data[index0] = list->data[index1];
    list->data[index1] = tmp;
}

static _public_List_char *_public_List_char_new() {
    _public_List_char *list = (_public_List_char *)malloc(sizeof(_public_List_char));
    list->capacity = 0;
    list->length = 0;
    list->data = (_public_List_char_ItemPtr *)List_Nullptr;
    return list;
}

static void _public_List_char_delete(_public_List_char *list) {
    _private_List_char_deleteItems(list, 0, list->length);
    free(list->data);
    free(list);
}

static void _public_List_char_reserve(_public_List_char *list,
    _public_List_char_SizeType newCapacity) {
    if (newCapacity < list->capacity) {
        return;
    }
    list->data = (_public_List_char_ItemPtr *)realloc(
        list->data, newCapacity * sizeof(_public_List_char_ItemPtr));
    list->capacity = newCapacity;
}

static void _public_List_char_resize_shallow(_public_List_char *list,
    _public_List_char_SizeType newSize) {
    _public_List_char_reserve(list, newSize);
    list->length = newSize;
}

static void _public_List_char_resize_deep(_public_List_char *list,
    _public_List_char_SizeType newSize) {
    _private_List_char_deleteItems(list, newSize, list->length);
    _public_List_char_resize_shallow(list, newSize);
}

static Bool _public_List_char_empty(_public_List_char *list) {
    return (list->length == 0);
}

static _public_List_char_ItemPtr
_public_List_char_get(_public_List_char *list, _public_List_char_SizeType index) {
    if (!_private_List_char_indexValid(list, index)) {
        return NULL;
    }
    return list->data[index];
}

static Bool _public_List_char_set(_public_List_char *list,
    _public_List_char_ItemPtr pItem,
    _public_List_char_SizeType index) {
    if (!_private_List_char_indexValid(list, index)) {
        return False;
    }
    list->data[index] = pItem;
    return True;
}

static _public_List_char_SizeType
_public_List_char_find(_public_List_char *list, _public_List_char_ItemPtr pItem) {
    _public_List_char_SizeType i;
    for (i = 0; i < list->length; ++i) {
        if (list->data[i] == pItem) {
            return i;
        }
    }
    return List_InvalidIndex;
}

static void _public_List_char_pushback(_public_List_char *list,
    _public_List_char_ItemPtr pItem) {
    if (list->length == list->capacity) {
        _public_List_char_reserve(list, list->length * 2);
    }
    list->data[list->length] = pItem;
    ++list->length;
}

static Bool _public_List_char_insert(_public_List_char *list,
    _public_List_char_ItemPtr pItem,
    _public_List_char_SizeType index) {
    _public_List_char_SizeType i;
    if (!_private_List_char_newIndexValid(list, index)) {
        return False;
    }
    if (list->length == list->capacity) {
        _public_List_char_reserve(list, list->capacity * 2);
    }
    for (i = list->length; i > index; --i) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = pItem;
    ++list->length;
    return True;
}

static void _public_List_char_clear_shallow(_public_List_char *list) {
    list->length = 0;
}

static void _public_List_char_clear_deep(_public_List_char *list) {
    _public_List_char_resize_deep(list, 0);
}

static void _public_List_char_copy_shallow(_public_List_char *srcList,
    _public_List_char *dstList) {
    _public_List_char_resize_shallow(dstList, srcList->length);
    dstList->length = srcList->length;
    memcpy(dstList->data, srcList->data,
        srcList->length * sizeof(_public_List_char_ItemPtr));
}

static void _public_List_char_remove_shallow(_public_List_char *list,
    _public_List_char_SizeType index) {
    if (!_private_List_char_indexValid(list, index)) {
        return;
    }
    list->data[index] = list->data[list->length - 1];
    --list->length;
}

static void _public_List_char_merge_shallow(_public_List_char *listA,
    _public_List_char *listB) {
    _public_List_char_resize_shallow(listA, listA->length + listB->length);
    memcpy(listA->data + listA->length, listB->data,
        sizeof(_public_List_char_ItemPtr) * listB->length);
}

static void _public_List_char_reverse(_public_List_char *list) {
    _public_List_char_SizeType i, j;
    for (i = 0, j = list->length - 1; i < j; ++i, --j) {
        _private_List_char_swapItems(list, i, j);
    }
}

static void _public_List_char_shuffle(_public_List_char *list) {
    _public_List_char_SizeType i;
    for (i = list->length; i > 1; --i) {
        _private_List_char_swapItems(list, (i - 1), (rand() % i));
    }
}
