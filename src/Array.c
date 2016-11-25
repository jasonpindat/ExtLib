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
    Ptr *ct;
};



static inline void arrayResize(Array a) {
    a->size*=2;
    a->ct=realloc(a->ct, a->size*sizeof(Ptr));
}

static inline void arrayShift(Array a, int base) {
    for(int i=a->length-1; i>=base; i--) // size must be ok because Shift adds a "new" index
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

    a->copyFct = NULL;
    a->delFct = NULL;

    a->temp = malloc(a->elemSize);
    a->length = 0;

    a->size = DEFSIZE;
    a->ct = malloc(DEFSIZE*sizeof(Ptr));

    return a;
}

void arrayDel(Array a) {
    arrayClear(a);

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
    a2->length = 0;
    a2->size = a2->length * 2;
    a2->elemSize = a->elemSize;
    a2->multithread = a->multithread;
    a2->temp = malloc(a2->elemSize);
    a2->ct = malloc(a2->size * sizeof(Ptr));

    for(int i=from; i<to; i++)
        arrayPush_base(a2, a->ct[i]);

    return a2;
}



void arrayClear(Array a) {
    while(a->length > 0)
        arrayPop(a);
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
        a->ct = realloc(a->ct, a->size*sizeof(Ptr));
    }
}



bool arrayContains(Array a, Ptr data) {
    return arrayIndexOf(a, data) != -1;
}

int arrayIndexOf(Array a, Ptr data) {
    for(int i=0; i<a->length; i++)
        if(a->cmpFct(a->ct[i], data) == 0) //!\ ct[i] à gérer selon pointeur ou pas
            return i;

    return -1;
}

int arrayLastIndexOf(Array a, Ptr data) {
    for(int i=a->length-1; i>=0; i--)
        if(a->cmpFct(a->ct[i], data) == 0) //!\ ct[i] à gérer selon pointeur ou pas
            return i;

    return -1;
}



Ptr arrayGet_base(Array a, int pos) {
    if(a->copyFct) {
        if(a->multithread) {
            Ptr temp = malloc(a->elemSize);
            a->copyFct(temp, a->ct[pos]); //!\ ct[i] à gérer selon pointeur ou pas
            return temp;
        }
        else {
            a->copyFct(a->temp, a->ct[pos]); //!\ ct[i] à gérer selon pointeur ou pas
            return a->temp;
        }
    }
    else
        return a->ct[pos]; //!\ ct[i] à gérer selon pointeur ou pas
}



void arraySet_base(Array a, int pos, Ptr data) {
    if(a->delFct)
        a->delFct(a->ct[pos]); //!\ ct[i] à gérer selon pointeur ou pas

    if(a->copyFct)
        a->copyFct(a->ct[pos], data); //!\ ct[i] à gérer selon pointeur ou pas
    else
        memcpy(a->ct[pos], data, a->elemSize); //!\ ct[i] à gérer selon pointeur ou pas
}



void arrayPush_base(Array a, Ptr data) {
    arrayAdd_base(a, a->length, data);
}

void arrayAdd_base(Array a, int pos, Ptr data) {
    if(a->length >= a->size)
        arrayResize(a);

    arrayShift(a, pos);

    a->ct[pos] = malloc(a->elemSize); //!\ ct[i] à gérer selon pointeur ou pas

    if(a->copyFct)
        a->copyFct(a->ct[pos], data); //!\ ct[i] à gérer selon pointeur ou pas
    else
        memcpy(a->ct[pos], data, a->elemSize); //!\ ct[i] à gérer selon pointeur ou pas

    a->length++;
}



void arrayPop(Array a) {
    arrayRemove(a, a->length-1);
}

void arrayRemove(Array a, int pos) {
    if(a->delFct)
        a->delFct(a->ct[pos]); //!\ ct[i] à gérer selon pointeur ou pas

    free(a->ct[pos]); //!\ ct[i] à gérer selon pointeur ou pas

    arrayUnshift(a, pos);

    a->length--;
}



static void arraySortQS(Array a, int method, int p, int r) {
    if (p < r) {

        int i = p-1, j = r+1;

        while(true) {
            do
                j--;
            while(method * a->cmpFct(a->ct[j], a->ct[p]) > 0);

            do
                i++;
            while(method * a->cmpFct(a->ct[i], a->ct[p]) < 0);

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
    int opcost=sizeof(struct _Array)+a->size*sizeof(Ptr); //!\ ct[i] à gérer selon pointeur ou pas
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
        actFct(a->ct[i], infos); //!\ ct[i] à gérer selon pointeur ou pas
}
