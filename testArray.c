
#include "Array.h"
#include "Iterator.h"

void testArray1() {
    int nb=100;

    Array a = arrayNew(EL_SHORT);

    short *vals = malloc(nb*sizeof(short));

    for(int i=0; i<nb; i++)
        vals[i]=(short)i;

    for(int i=0; i<nb; i++)
        arrayAdd(a, vals[i]);

    free(vals);

    arrayHeap(a);

    arrayTrimCapacity(a);

    arrayHeap(a);

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

void testArrayIt() {
    Array a = arrayNew(sizeof(int));

    for(int i=0; i<20; i++) {
        arrayAdd(a, i);
    }

    for(Iterator it = itNew((Iterable)a); itExists(&it); itNext(&it)) {
        printf("%d\n", itGet(&it, int));
    }

    arrayDel(a);
}

struct test {
    int *nb;
};

void copyTest(struct test *src, struct test *dest) {
    dest->nb = malloc(sizeof(int));
    *(dest->nb) = *(src->nb);
}

void delTest(struct test *elt) {
    free(elt->nb);
}

int main() {
    Array a = arrayNew(sizeof(struct test));
    arrayElementCopy(a, copyTest, delTest);

    struct test t;
    t.nb = malloc(sizeof(int));
    *(t.nb) = 5;

    arrayAdd(a, t);

    struct test t2 = arrayGet(a, 0, struct test);

    arrayDel(a);

    printf("%d\n", *(t2.nb));

    return EXIT_SUCCESS;
}
