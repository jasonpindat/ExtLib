/**
 * \file Array.c
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Common.h"
#include "ExtLib/Array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEFSIZE 8

struct _Array {
    RealType type;
    int elemSize;
    ElCopyFct copyFct;
    ElDelFct delFct;

    int length;
    ElCmpFct cmpFct;
    Ptr temp;

    bool multithread;

    int size;
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
    if(a->copyFct)
        a->copyFct(a->temp,                a->ct+a->elemSize*pos1);
    else
        memcpy(a->temp,                    a->ct+a->elemSize*pos1, a->elemSize);

    if(a->delFct)
        a->delFct(a->ct+a->elemSize*pos1);

    if(a->copyFct)
        a->copyFct(a->ct+a->elemSize*pos1, a->ct+a->elemSize*pos2);
    else
        memcpy(a->ct+a->elemSize*pos1,     a->ct+a->elemSize*pos2, a->elemSize);

    if(a->delFct)
        a->delFct(a->ct+a->elemSize*pos2);

    if(a->copyFct)
        a->copyFct(a->ct+a->elemSize*pos2, a->temp);
    else
        memcpy(a->ct+a->elemSize*pos2,     a->temp, a->elemSize);
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

    a->temp = malloc(a->elemSize);
    a->length = 0;

    a->size = DEFSIZE;
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

void arrayMultithread(Array a, bool multithread) {
    a->multithread = multithread;
}



Array arrayClone(Array a) {
    return arraySubArray(a, 0, a->length);
}

Array arraySubArray(Array a, int from, int to) {
    Array a2 = malloc(sizeof(struct _Array));

    a2->type = ARRAY;

    a2->copyFct = a->copyFct;
    a2->delFct = a->delFct;
    a2->cmpFct = a->cmpFct;
    a2->length = to-from;
    a2->size = a2->length * 2;
    a2->elemSize = a->elemSize;
    a2->multithread = a->multithread;
    a2->temp = malloc(a2->elemSize);
    a2->ct = malloc(a2->size * a2->elemSize);

    if(a2->copyFct) {
        for(int i=0; i<a2->length; i++)
            a2->copyFct(a2->ct + i*a2->elemSize, a->ct + (i+from)*a2->elemSize);
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
        if(a->multithread) {
            Ptr temp = malloc(a->elemSize);
            a->copyFct(temp, a->ct + a->elemSize*pos);
            return temp;
        }
        else {
            a->copyFct(a->temp, a->ct + a->elemSize*pos);
            return a->temp;
        }
    }
    else
        return a->ct + a->elemSize*pos;
}



void arraySet_base(Array a, int pos, Ptr data) {
    if(a->delFct)
        a->delFct(a->ct + a->elemSize*pos);

    if(a->copyFct)
        a->copyFct(a->ct + a->elemSize*pos, data);
    else
        memcpy(a->ct + a->elemSize*pos, data, a->elemSize);
}



void arrayPush_base(Array a, Ptr data) {
    arrayAdd_base(a, a->length, data);
}

void arrayAdd_base(Array a, int pos, Ptr data) {
    if(a->length >= a->size)
        arrayResize(a);

    arrayShift(a, pos);

    if(a->copyFct)
        a->copyFct(a->ct + a->elemSize*pos, data);
    else
        memcpy(a->ct + a->elemSize*pos, data, a->elemSize);

    a->length++;
}



void arrayPop(Array a) {
    arrayRemove(a, a->length-1);
}

void arrayRemove(Array a, int pos) {
    if(a->delFct)
        a->delFct(a->ct + a->elemSize*pos);

    arrayUnshift(a, pos);
    a->length--;
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



void arrayDump(Array a) {
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

ArrayIt arrayItNew(Array a) {
    ArrayIt it;

    it.array = a;
    it.index = 0;

    return it;
}

ArrayIt arrayItNewBack(Array a) {
    ArrayIt it;

    it.array = a;
    it.index = a->length-1;

    return it;
}



bool arrayItExists(ArrayIt *it) {
    return it->index >=0 && it->index < it->array->length;
}



void arrayItNext(ArrayIt *it) {
    it->index++;
}

void arrayItPrev(ArrayIt *it) {
    it->index--;
}



Ptr arrayItGet_base(ArrayIt *it) {
    return arrayGet_base(it->array, it->index);
}



void arrayItSet_base(ArrayIt *it, Ptr data) {
    arraySet_base(it->array, it->index, data);
}



void arrayItAddAfter_base(ArrayIt *it, Ptr data) {
    arrayAdd_base(it->array, it->index+1, data);
}

void arrayItAddBefore_base(ArrayIt *it, Ptr data) {
    arrayAdd_base(it->array, it->index, data);
    it->index++;
}



void arrayItRemove(ArrayIt *it) {
    arrayRemove(it->array, it->index);
    it->index--;
}



void arrayForEach(Array a, ElActFct actFct, Ptr infos) {
    for(int i=0; i<a->length; i++)
        actFct(a->ct + i*a->elemSize, infos);
}
