
#include "ExtLib/Collection.h"
#include "ExtLib/Iterable.h"
#include "ExtLib/Array.h"
#include "ExtLib/Heap.h"
#include "ExtLib/Hash.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testLValue() {
    Array a = arrayNew(sizeof(int));

    int n = 5;
    arrayPush(a, n);
    arrayPushI(a, 4, int);
    arrayPushI(a, 4, int);
    int k1 = arrayGet(a, 0, int);
    int k2 = arrayGet(a, 1, int);
    int k3 = arrayGet(a, 2, int);

    printf("%d %d %d\n", k1, k2, k3);

    arrayDel(a);
}

int main() {
    //testArray1();
    testLValue();

    return EXIT_SUCCESS;
}
