
#include "ExtLib/Collection.h"
#include "ExtLib/Array.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testArray1() {
    int nb=100;

    Array a = arrayNew(EL_SHORT);

    short *vals = malloc(nb*sizeof(short));

    for(int i=0; i<nb; i++)
        vals[i]=(short)i;

    for(int i=0; i<nb; i++)
        arrayPush(a, vals[i]);

    free(vals);

    arrayDump(a);

    arrayTrimCapacity(a);

    arrayDump(a);

    arrayRandomize(a);

    for(int i=0; i<nb; i++)
        printf("%d\n", arrayGet(a, i, short));

    arraySort(a, EL_ASC);

    for(int i=0; i<nb; i++)
        printf("%d\n", arrayGet(a, i, short));

    for(int i=0; i<nb; i++)
        arrayRemove(a, arrayLength(a)-1);

    printf("%d\n", arrayLength(a));

    arrayDel(a);
}

void printElement(int *elt, Ptr infos) {
    printf("%d\n", *elt);
}

void testArrayIt() {
    Array a = arrayNew(sizeof(int));

    for(int i=0; i<20; i++) {
        arrayPush(a, i);
    }

    for(ArrayIt it = arrayItNew(a); arrayItExists(&it); arrayItNext(&it)) {
        printf("%d\n", arrayItGet(&it, int));
    }

    for(int i=0; i<arrayLength(a); i++)
        printf("%d\n", arrayGet(a, i, int));

    arrayForEach(a, (ElActFct)printElement, NULL);

    arrayDel(a);
}

struct test {
    int *nb;
};

void copyTest(struct test *dest, struct test *src) {
    dest->nb = malloc(sizeof(int));
    *(dest->nb) = *(src->nb);
}

void delTest(struct test *elt) {
    free(elt->nb);
}

void testArrayCopy() {
    Array a = arrayNew(sizeof(struct test));
    collectionElementInstanciable((Collection)a, (ElCopyFct)copyTest, (ElDelFct)delTest);

    struct test t;
    t.nb = malloc(sizeof(int));
    *(t.nb) = 5;

    arrayPush(a, t);

    struct test t2 = arrayGet(a, 0, struct test);

    arrayDel(a);

    printf("%d\n", *(t2.nb));
}

int main() {
    testArrayIt();

    return EXIT_SUCCESS;
}
