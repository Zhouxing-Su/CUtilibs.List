#include "List.h"
#include "Test.h"

void freeIntPtr(int *p) {
    free(p);
}

MakeList(int, freeIntPtr)

int main() {
    List_SizeType(int) i = 0;
    List_ItemPtr(int) p = (int*)malloc(sizeof(int));
    List_Destructor(int) fp = freeIntPtr;

    int j = 1, k = 2;
    *p = i;
    Test test = { -2, List_new(double)() };

    List(int) *arr = List_new(int)();
    List_pushback(int)(arr, p);
    List_insert(int)(arr, &j, 0);

    if ((List_find(int)(arr, &j) == 0) && (List_find(int)(arr, &k) == List_InvalidIndex)) {
        printf("%d, %d\n", *List_get(int)(arr, 0), *List_get(int)(arr, 1));
    }

    f(&test);

    List_delete(double)(test.darr);

    List_resize_deep(int)(arr, 1);
    List_delete(int)(arr); // crash when trying to free &j.

    return 0;
}