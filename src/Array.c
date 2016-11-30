/**
 * \file Array.c
 * \author Jason Pindat
 * \date 2016-11-26
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Common.h"
#include "ExtLib/Collection.h"
#include "ExtLib/Array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEFSIZE 8

struct _Array {
    RealType type;
    int elemSize;
    ElCmpFct cmpFct;
    ElCopyFct copyFct;
    ElDelFct delFct;
    bool needsAllocation;
    Ptr (*ptrTransform)(Ptr);

    int length;

    int size;
    Ptr *ct;
};



static inline void arrayShift(Array a, int base) {
    for(int i=a->length-1; i>=base; i--)
        a->ct[i+1] = a->ct[i];
}

static inline void arrayUnshift(Array a, int base) {
    for(int i=base; i<a->length-1; i++)
        a->ct[i] = a->ct[i+1];
}

static inline void arraySwap(Array a, int pos1, int pos2) {
    Ptr temp = a->ct[pos1];
    a->ct[pos1] = a->ct[pos2];
    a->ct[pos2] = temp;
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

    collectionElementInstanciable((Collection)a, NULL, NULL);

    a->length = 0;

    a->size = DEFSIZE;
    a->ct = malloc(DEFSIZE * sizeof(Ptr));

    return a;
}

void arrayDel(Array a) {
    arrayClear(a);

    free(a->ct);
    free(a);
}



void arrayComparable(Array a, ElCmpFct fct) {
    a->cmpFct = fct;
}



Array arrayClone(const Array a) {
    return arraySubArray(a, 0, a->length);
}

Array arraySubArray(const Array a, int from, int to) {
    Array a2 = malloc(sizeof(struct _Array));

    a2->type = ARRAY;
    a2->elemSize = a->elemSize;
    a2->cmpFct = a->cmpFct;
    a2->copyFct = a->copyFct;
    a2->delFct = a->delFct;
    a2->needsAllocation = a->needsAllocation;
    a2->ptrTransform = a->ptrTransform;

    a2->length = 0;

    a2->size = a->length * 2;
    if(a2->size < DEFSIZE)
        a2->size = DEFSIZE;
    a2->ct = malloc(a2->size * sizeof(Ptr));

    for(int i=from; i<to; i++)
        arrayPush_base(a2, a->ptrTransform(&a->ct[i]));

    return a2;
}



void arrayClear(Array a) {
    if(a->needsAllocation) {
        for(int i=0; i<a->length; i++) {
            if(a->delFct)
                a->delFct(a->ct[i]);

            free(a->ct[i]);
        }
    }

    a->length = 0;
    a->size = DEFSIZE;
    a->ct = realloc(a->ct, DEFSIZE * sizeof(Ptr));
}



bool arrayIsEmpty(const Array a) {
    return a->length == 0;
}

int arrayLength(const Array a) {
    return a->length;
}



void arrayTrimCapacity(Array a) {
    if(a->size > a->length) {
        a->size = a->length;
        a->ct = realloc(a->ct, a->size*sizeof(Ptr));
    }
}



bool arrayContains(const Array a, const Ptr data) {
    return arrayIndexOf(a, data) != -1;
}

int arrayIndexOf(const Array a, const Ptr data) {
    for(int i=0; i<a->length; i++)
        if(a->cmpFct(a->ptrTransform(&a->ct[i]), data) == 0)
            return i;

    return -1;
}

int arrayLastIndexOf(const Array a, const Ptr data) {
    for(int i=a->length-1; i>=0; i--)
        if(a->cmpFct(a->ptrTransform(&a->ct[i]), data) == 0)
            return i;

    return -1;
}



const Ptr arrayGet_base(const Array a, int pos) {
    return a->ptrTransform(&a->ct[pos]);
}



void arraySet_base(Array a, int pos, const Ptr data) {
    if(a->needsAllocation) {
        if(a->delFct)
            a->delFct(a->ct[pos]);

        if(a->copyFct)
            a->copyFct(a->ct[pos], data);
        else
            memcpy(a->ct[pos], data, a->elemSize);
    }
    else
        memcpy(&a->ct[pos], data, a->elemSize);
}



void arrayPush_base(Array a, const Ptr data) {
    arrayAdd_base(a, a->length, data);
}

void arrayAdd_base(Array a, int pos, const Ptr data) {
    if(a->length >= a->size) {
        a->size *= 2;
        a->ct = realloc(a->ct, a->size*sizeof(Ptr));
    }

    arrayShift(a, pos);

    if(a->needsAllocation) {
        a->ct[pos] = malloc(a->elemSize);

        if(a->copyFct)
            a->copyFct(a->ct[pos], data);
        else
            memcpy(a->ct[pos], data, a->elemSize);
    }
    else
        memcpy(&a->ct[pos], data, a->elemSize);

    a->length++;
}



void arrayPop(Array a) {
    arrayRemove(a, a->length-1);
}

void arrayRemove(Array a, int pos) {
    if(a->needsAllocation) {
        if(a->delFct)
            a->delFct(a->ct[pos]);

        free(a->ct[pos]);
    }

    arrayUnshift(a, pos);

    a->length--;

    if ((a->length <= (a->size / 4)) && (a->size / 2 >= DEFSIZE)) {
		a->size /= 2;
		a->ct = realloc(a->ct, a->size * sizeof(Ptr));
	}
}



static void arraySortQS(Array a, int method, int p, int r) {
    if (p < r) {

        Ptr pPtr = a->ptrTransform(&a->ct[p]);

        int i = p-1, j = r+1;

        while(true) {
            do
                j--;
            while(method * a->cmpFct(a->ptrTransform(&a->ct[j]), pPtr) > 0);

            do
                i++;
            while(method * a->cmpFct(a->ptrTransform(&a->ct[i]), pPtr) < 0);

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
    if(a->length >= 2)
        arraySortQS(a, method, 0, a->length-1);
}

void arrayRandomize(Array a) {
    srand(time(NULL));

    for(int i=a->length-1; i>0; i--) {
        int r = rand()%(i+1);
        arraySwap(a, i, r);
    }
}



void arrayDump(const Array a) {
    int elts=arrayLength(a);
    int effcost=elts*a->elemSize;
    int opcost=sizeof(struct _Array);
    if(a->needsAllocation)
        opcost += a->size*sizeof(Ptr);
    int preallcost=(a->size-elts)*a->elemSize;

    printf("Array at %p\n", a);
    printf("\t%d elements, each using %d bytes\n", elts, a->elemSize);
    printf("\t%d bytes used for elements\n", effcost);
    printf("\t%d bytes used as operating cost\n", opcost);
    printf("\t%d bytes used as preallocated\n", preallcost);
    printf("\t%d bytes total used\n", effcost+opcost+preallcost);
}



// Iteration

ArrayIt arrayItNew(const Array a) {
    ArrayIt it;

    it.array = a;
    it.index = 0;

    return it;
}

ArrayIt arrayItNewBack(const Array a) {
    ArrayIt it;

    it.array = a;
    it.index = a->length-1;

    return it;
}



bool arrayItExists(const ArrayIt *it) {
    return it->index >=0 && it->index < it->array->length;
}



void arrayItNext(ArrayIt *it) {
    it->index++;
}

void arrayItPrev(ArrayIt *it) {
    it->index--;
}



const Ptr arrayItGet_base(const ArrayIt *it) {
    return arrayGet_base(it->array, it->index);
}



void arrayItSet_base(ArrayIt *it, const Ptr data) {
    arraySet_base(it->array, it->index, data);
}



void arrayItAddAfter_base(ArrayIt *it, const Ptr data) {
    arrayAdd_base(it->array, it->index+1, data);
}

void arrayItAddBefore_base(ArrayIt *it, const Ptr data) {
    arrayAdd_base(it->array, it->index, data);
    it->index++;
}



void arrayItRemove(ArrayIt *it) {
    arrayRemove(it->array, it->index);
    it->index--;
}



void arrayForEach(Array a, ElActFct actFct, Ptr infos) {
    for(int i=0; i<a->length; i++)
        actFct(a->ptrTransform(&a->ct[i]), infos);
}
