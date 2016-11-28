/**
 * \file Heap.c
 * \author Jason Pindat
 * \date 2016-11-26
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Common.h"
#include "ExtLib/Collection.h"
#include "ExtLib/Heap.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFSIZE 8

struct _Heap {
    RealType type;
    int elemSize;
    ElCmpFct cmpFct;
    ElCopyFct copyFct;
    ElDelFct delFct;
    bool needsAllocation;
    Ptr (*ptrTransform)(Ptr);

    unsigned int length;

	unsigned int size;
    Ptr *data;
};



Heap heapNew(int elemSize) {
    Heap h = malloc(sizeof(struct _Heap));

    h->type = HEAP;

    if(elemSize<=0) {
        h->elemSize=_elSizeFct(elemSize);
        h->cmpFct=_elCompareFct(elemSize);
    }
    else {
        h->elemSize=elemSize;
        h->cmpFct=NULL;
    }

    collectionElementInstanciable((Collection)h, NULL, NULL);

    h->length = 0;

    h->size = DEFSIZE;
    h->data = malloc(DEFSIZE * sizeof(Ptr));

	return h;
}

void heapDel(Heap h) {
    heapClear(h);

    free(h->data);
    free(h);
}



void heapComparable(Heap h, ElCmpFct fct) {
    h->cmpFct = fct;
}



Heap heapClone(Heap h) {
    Heap h2 = malloc(sizeof(struct _Heap));

    h2->type = HEAP;
    h2->elemSize = h->elemSize;
    h2->cmpFct = h->cmpFct;
    h2->copyFct = h->copyFct;
    h2->delFct = h->delFct;
    h2->needsAllocation = h->needsAllocation;
    h2->ptrTransform = h->ptrTransform;

    h2->length = 0;

    h2->size = h->length * 2;
    if(h2->size < DEFSIZE)
        h2->size = DEFSIZE;
    h2->data = malloc(h2->size * sizeof(Ptr));

    for(int i=0; i<h->length; i++)
        heapPush_base(h2, h->ptrTransform(&h->data[i]));

    return h2;
}



void heapClear(Heap h) {
    if(h->needsAllocation) {
        for(int i=0; i<h->length; i++) {
            if(h->delFct)
                h->delFct(h->data[i]);

            free(h->data[i]);
        }
    }

    h->length = 0;
    h->size = DEFSIZE;
    h->data = realloc(h->data, DEFSIZE * sizeof(Ptr));
}



bool heapIsEmpty(Heap h) {
    return h->length == 0;
}

int heapLength(Heap h) {
    return h->length;
}



Ptr heapGet_base(Heap h) {
    return h->ptrTransform(&h->data[0]);
}



void heapPush_base(Heap h, Ptr value)
{
	unsigned int index, parent;

	if (h->length >= h->size) {
		h->size *= 2;
		h->data = realloc(h->data, h->size*sizeof(Ptr));
	}

	// Find out where to put the element and put it
	for(index = h->length++; index != 0; index = parent)
	{
		parent = (index - 1) / 2;

		if(h->cmpFct(h->ptrTransform(&h->data[parent]), value) >= 0)
            break;

		h->data[index] = h->data[parent];
	}

	if(h->needsAllocation) {
        h->data[index] = malloc(h->elemSize);

        if(h->copyFct)
            h->copyFct(h->data[index], value);
        else
            memcpy(h->data[index], value, h->elemSize);
	}
	else
        h->data[index] = value;
}



void heapPop(Heap h)
{
	unsigned int index, swap, other;

    if(h->needsAllocation) {
        if(h->delFct)
            h->delFct(h->data[0]);

        free(h->data[0]);
    }

	h->length--;

	Ptr temp = h->ptrTransform(&h->data[h->length]);

	// Reorder the elements
	for(index = 0; true; index = swap)
	{
		// Find the child to swap with
		swap = (index * 2) + 1;

		if (swap >= h->length)
            break; // If there are no children, the heap is reordered

		other = swap + 1;
		if ((other < h->length) && h->cmpFct(h->ptrTransform(&h->data[other]), h->ptrTransform(&h->data[swap])) >= 0)
            swap = other;

		if (h->cmpFct(temp, h->ptrTransform(&h->data[swap])) >= 0)
            break; // If the bigger child is less than or equal to its parent, the heap is reordered

		h->data[index] = h->data[swap];
	}

	h->data[index] = h->data[h->length];


    if ((h->length <= (h->size / 4)) && (h->size / 2 >= DEFSIZE)) {
		h->size /= 2;
		h->data = realloc(h->data, h->size * sizeof(Ptr));
	}
}



void heapDump(Heap h) {
    int elts=heapLength(h);
    int effcost=elts*h->elemSize;
    int opcost=sizeof(struct _Heap);
    if(h->needsAllocation)
        opcost += h->size*sizeof(Ptr);
    int preallcost=(h->size-elts)*h->elemSize;

    printf("Heap at %p\n", h);
    printf("\t%d elements, each using %d bytes\n", elts, h->elemSize);
    printf("\t%d bytes used for elements\n", effcost);
    printf("\t%d bytes used as operating cost\n", opcost);
    printf("\t%d bytes used as preallocated\n", preallcost);
    printf("\t%d bytes total used\n", effcost+opcost+preallcost);

    /*for(int i=0; i<h->length; i++)
        printf("%d\n", *((int*)h->ptrTransform(&h->data[i])));*/
}



// Heapifies a non-empty array
/*void heapify(type data[restrict], unsigned int count)
{
	unsigned int item, index, swap, other;
	type temp;

	// Move every non-leaf element to the right position in its subtree
	item = (count >> 1) - 1;
	while (1)
	{
		// Find the position of the current element in its subtree
		temp = data[item];
		for(index = item; 1; index = swap)
		{
			// Find the child to swap with
			swap = (index << 1) + 1;
			if (swap >= count) break; // If there are no children, the current element is positioned
			other = swap + 1;
			if ((other < count) && CMP(data[other], data[swap])) swap = other;
			if CMP(temp, data[swap]) break; // If the bigger child is smaller than or equal to the parent, the heap is reordered

			data[index] = data[swap];
		}
		if (index != item) data[index] = temp;

		if (!item) return;
		--item;
	}
}*/
