#include "Common.h"
#include "Array.h"

#include <time.h>

#define DEFSIZE 8

struct _Array {
    RealType type;
    ElCopyFct copyFct;
    ElDelFct delFct;
    ElCmpFct cmpFct;
    int length;
    int size;
    int elemSize;
    Ptr temp;
    Ptr ct;
};



static inline void arrayResize(Array a) {
    a->size*=2;
    a->ct=realloc(a->ct, a->size*a->elemSize);
}

static inline void arrayShift(Array a, int base) {
    for(int i=a->length-1; i>=base; i--) // size must be ok because Shift adds a "new" index
        memcpy(a->ct+(i+1)*a->elemSize, a->ct+i*a->elemSize, a->elemSize);
}

static inline void arrayUnshift(Array a, int base) {
    for(int i=base; i<a->length-1; i++)
        memcpy(a->ct+i*a->elemSize, a->ct+(i+1)*a->elemSize, a->elemSize);
}

static inline void arraySwap(Array a, int pos1, int pos2) {
    memcpy(a->temp,                a->ct+a->elemSize*pos1, a->elemSize);
    memcpy(a->ct+a->elemSize*pos1, a->ct+a->elemSize*pos2, a->elemSize);
    memcpy(a->ct+a->elemSize*pos2, a->temp,                a->elemSize);
}



Array arrayNew(int elemSize) {
    Array a=malloc(sizeof(struct _Array));

    a->type = ARRAY;

    if(elemSize<=0) {
        a->elemSize=_elSizeFct(elemSize);
        a->cmpFct=_elCompareFct(elemSize);
    }
    else {
        a->elemSize=elemSize;
        a->cmpFct=NULL;
    }

    a->copyFct = NULL;
    a->delFct = NULL;
    a->length = 0;
    a->size = DEFSIZE;
    a->temp = malloc(a->elemSize);
    a->ct = malloc(DEFSIZE*a->elemSize);
    return a;
}

void arrayDel(Array a) {
    if(a->delFct)
        for(int i=0; i<a->length; i++)
            a->delFct(a->ct + i*a->elemSize);

    free(a->temp);
    free(a->ct);
    free(a);
}

void arrayComparable(Array a, ElCmpFct fct) {
    a->cmpFct = fct;
}

void arrayElementCopy(Array a, ElCopyFct copyFct, ElDelFct delFct) {
    a->copyFct = copyFct;
    a->delFct = delFct;
}



Array arrayClone(Array a) {
    return arraySubArray(a, 0, a->length);
}

Array arraySubArray(Array a, int from, int to) {
    Array a2 = malloc(sizeof(struct _Array));

    a->type = ARRAY;

    a2->copyFct = a->copyFct;
    a2->delFct = a->delFct;
    a2->cmpFct = a->cmpFct;
    a2->length = to-from;
    a2->size = a2->length * 2;
    a2->elemSize = a->elemSize;
    a2->temp = malloc(a2->elemSize);
    a2->ct = malloc(a2->size * a2->elemSize);

    if(a2->copyFct) {
        for(int i=0; i<a2->length; i++)
            a2->copyFct(a->ct + (i+from)*a2->elemSize, a2->ct + i*a2->elemSize);
    }
    else
        memcpy(a2->ct, a->ct + from*a2->elemSize, a2->length*a2->elemSize);

    return a2;
}



void arrayClear(Array a) {
    if(a->delFct)
        for(int i=0; i<a->length; i++)
            a->delFct(a->ct + i*a->elemSize);

    a->length = 0;
}



bool arrayIsEmpty(Array a) {
    return a->length == 0;
}

int arrayLength(Array a) {
    return a->length;
}



void arrayTrimCapacity(Array a) {
    if(a->size > a->length) {
        a->size = a->length;
        a->ct = realloc(a->ct, a->size*a->elemSize);
    }
}



bool arrayContains(Array a, Ptr data) {
    return arrayIndexOf(a, data) != -1;
}

int arrayIndexOf(Array a, Ptr data) {
    for(int i=0; i<a->length; i++)
        if(a->cmpFct(a->ct + i*a->elemSize, data) == 0)
            return i;

    return -1;
}

int arrayLastIndexOf(Array a, Ptr data) {
    for(int i=a->length-1; i>=0; i--)
        if(a->cmpFct(a->ct + i*a->elemSize, data) == 0)
            return i;

    return -1;
}



Ptr arrayGet_base(Array a, int pos) {
    if(a->copyFct) {
        a->copyFct(a->ct + a->elemSize*pos, a->temp);
        return a->temp;
    }
    else
        return a->ct + a->elemSize*pos;
}



void arraySet_base(Array a, int pos, Ptr data) {
    if(a->delFct)
        a->delFct(a->ct + a->elemSize*pos);

    if(a->copyFct)
        a->copyFct(data, a->ct + a->elemSize*pos);
    else
        memcpy(a->ct + a->elemSize*pos, data, a->elemSize);
}



void arrayAdd_base(Array a, Ptr data) {
    arrayAddAt_base(a, a->length, data);
}

void arrayAddAt_base(Array a, int pos, Ptr data) {
    if(a->length >= a->size)
        arrayResize(a);

    arrayShift(a, pos);

    if(a->copyFct)
        a->copyFct(data, a->ct + a->elemSize*pos);
    else
        memcpy(a->ct + a->elemSize*pos, data, a->elemSize);

    a->length++;
}



void arrayRemove(Array a, int pos) {
    if(a->delFct)
        a->delFct(a->ct + a->elemSize*pos);

    arrayUnshift(a, pos);
    a->length--;
}

void arrayRemoveElement(Array a, Ptr data) {
    int index = arrayIndexOf(a, data);
    if(index != -1)
        arrayRemove(a, index);
}



static void arraySortQS(Array a, int method, int p, int r) {
    if (p < r) {

        int i = p-1, j = r+1;

        while(true) {
            do
                j--;
            while(method * a->cmpFct(a->ct+j*a->elemSize, a->ct+p*a->elemSize) > 0);

            do
                i++;
            while(method * a->cmpFct(a->ct+i*a->elemSize, a->ct+p*a->elemSize) < 0);

            if(i < j)
                arraySwap(a, i, j);
            else
                break;
        }

        arraySortQS(a, method, p, j);
        arraySortQS(a, method, j+1, r);
    }
}

void arraySort(Array a, int method) {
    if(a->length>=2)
        arraySortQS(a, method, 0, a->length-1);
}

void arrayRandomize(Array a) {
    srand(time(NULL));

    for(int i=a->length-1; i>0; i--) {
        int r = rand()%(i+1);
        arraySwap(a, i, r);
    }
}



void arrayHeap(Array a) {
    int elts=arrayLength(a);
    int effcost=elts*a->elemSize;
    int opcost=sizeof(struct _Array);
    int preallcost=(a->size-elts)*a->elemSize;

    printf("Array at %p\n", a);
    printf("\t%d elements, each using %d bytes\n", elts, a->elemSize);
    printf("\t%d bytes used for elements\n", effcost);
    printf("\t%d bytes used as operating cost\n", opcost);
    printf("\t%d bytes used as preallocated\n", preallcost);
    printf("\t%d bytes total used\n", effcost+opcost+preallcost);
}


// Iteration

bool arrayItExists(Iterator *it) {
    return it->data.arrayData < arrayLength((Array)(it->col));
}

void arrayItNext(Iterator *it) {
    it->data.arrayData++;
}

Ptr arrayItGet(Iterator *it) {
    return arrayGet_base((Array)(it->col), it->data.arrayData);
}
