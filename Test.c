#include "Test.h"

void Test_delete(Test *p) {
    List_delete_deep(double)(p->darr);
    free(p);
}

MakeList(Test,Test_delete)

void f(Test *test) {
    Test *p;
    List_ItemPtr(Test) q;
    double *d = (double*)malloc(sizeof(double));
    List(Test) *arr = List_new(Test)();

    *d = 1024;
    List_pushback(double)(test->darr, d);

    List_pushback(Test)(arr, test);
    p = List_get(Test)(arr, 0);
    q = p;

    printf("%d, %lf\n", q->i, *List_get(double)(test->darr, 0));

    List_delete_deep(Test)(arr);
}
