#include "List.h"

MakeList(double,free)

typedef struct Test {
    int i;
    List(double) *darr;
} Test;

void f(Test *test);

void Test_delete(Test *p);
