
#include "ExtLib/Collection.h"
#include "ExtLib/Iterable.h"
#include "ExtLib/Array.h"
#include "ExtLib/Heap.h"
#include "ExtLib/Hash.h"

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


    a = arrayNew(EL_POINTER);

    Ptr *vals2 = malloc(nb*sizeof(Ptr));

    for(int i=0; i<nb; i++)
        vals2[i]=(Ptr)i;

    for(int i=0; i<nb; i++)
        arrayPush(a, vals2[i]);

    free(vals2);

    arrayDump(a);

    arrayTrimCapacity(a);

    arrayDump(a);

    for(int i=0; i<nb; i++)
        printf("%d\n", (int)arrayGet(a, i, Ptr));

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

void testHeap() {

    Array a = arrayNew(EL_INT);

    for(int i=0; i<100; i++) {
        int k = 100-i;
        arrayPush(a, i);
        arrayPush(a, k);
    }

    Heap h = toHeap((Collection)a);

    arrayDel(a);

    heapDump(h);

    while(!heapIsEmpty(h)) {
        printf("%d\n", heapGet(h, int));
        heapPop(h);
    }

    heapDel(h);
}

void testHash() {
    Hash h = hashNewStr(sizeof(char *));

    char *s1 = "coucou1";
    char *s2 = "coucou2";
    char *s3 = "coucou3";


    hashSet(h, s1, s1);
    hashSet(h, s2, s2);

    printf("%p\n", hashGet(h, s1, char *));
    printf("%p\n", hashGet(h, s2, char *));
    printf("%p\n", hashGet(h, s3, char *));

    hashUnset(h, s1);

    printf("%p\n", hashGet(h, s1, char *));

    hashDel(h);
}

int main() {
    //testArray1();
    testHash();

    return EXIT_SUCCESS;
}
