/**
 * \file Heap.c
 * \author Jason Pindat
 * \date 2016-11-24
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Common.h"
#include "ExtLib/Heap.h"

#include <string.h>
#include <stdlib.h>

#define DEFSIZE 8

struct _Heap {
    RealType type;
    int elemSize;
    ElCopyFct copyFct;
    ElDelFct delFct;

    unsigned int length;
    ElCmpFct cmpFct;
    Ptr temp;

    bool multithread;

	unsigned int size;
    Ptr data;
};



Heap heapNew(int elemSize, ElCmpFct cmpFct) {
    Heap h = malloc(sizeof(struct _Heap));

    h->type = HEAP;

    /*if(elemSize<=0) {
        h->elemSize=_elSizeFct(elemSize);
        h->cmpFct=_elCompareFct(elemSize);
    }
    else {*/
        h->elemSize=elemSize;
        /*h->cmpFct=NULL;
    }*/

    h->copyFct = NULL;
    h->delFct = NULL;

    h->temp = malloc(h->elemSize);
    h->length = 0;

    h->size = DEFSIZE;
    h->data = malloc(DEFSIZE*h->elemSize);
    h->cmpFct = cmpFct;

	return h;
}

void heapDel(Heap h) {
    if(h->delFct)
        for(int i=0; i<h->length; i++)
            h->delFct(h->data + i*h->elemSize);

    free(h->temp);
    free(h->data);
    free(h);
}



void heapMultithread(Heap h, bool multithread) {
    h->multithread = multithread;
}



Heap heapClone(Heap h) {
    Heap h2 = malloc(sizeof(struct _Heap));

    h->type = HEAP;

    h2->copyFct = h->copyFct;
    h2->delFct = h->delFct;
    h2->cmpFct = h->cmpFct;
    h2->length = h->length;
    h2->size = h2->length * 2;
    h2->elemSize = h->elemSize;
    h2->temp = malloc(h2->elemSize);
    h2->data = malloc(h2->size * h2->elemSize);

    if(h2->copyFct) {
        for(int i=0; i<h2->length; i++)
            h2->copyFct(h2->data + i*h2->elemSize, h->data + i*h2->elemSize);
    }
    else
        memcpy(h2->data, h->data, h2->length*h2->elemSize);

    return h2;
}



void heapClear(Heap h) {
    if(h->delFct)
        for(int i=0; i<h->length; i++)
            h->delFct(h->data + i*h->elemSize);

    h->length = 0;
}



bool heapIsEmpty(Heap h) {
    return h->length == 0;
}

int heapLength(Heap h) {
    return h->length;
}



Ptr heapGet_base(Heap h) {
    if(h->copyFct) {
        if(h->multithread) {
            Ptr temp = malloc(h->elemSize);
            h->copyFct(temp, h->data);
            return temp;
        }
        else {
            h->copyFct(h->temp, h->data);
            return h->temp;
        }
    }
    else
        return h->data;
}



void heapPush_base(Heap h, Ptr value)
{
	unsigned int index, parent;

	// Resize the heap if it is too small to hold all the data
	if (h->length == h->size)
	{
		h->size <<= 1;
		h->data = realloc(h->data, h->elemSize * h->size);
	}

	// Find out where to put the element and put it
	for(index = h->length++; index; index = parent)
	{
		parent = (index - 1) >> 1;

		if (h->cmpFct(h->data+parent*h->elemSize, value) >= 0)
            break;

		//h->data[index] = h->data[parent];
		if(h->copyFct)
            h->copyFct(h->data+index*h->elemSize, h->data+parent*h->elemSize);
        else
            memcpy(h->data+index*h->elemSize, h->data+parent*h->elemSize, h->elemSize);

        if(h->delFct)
            h->delFct(h->data+parent*h->elemSize);
	}

	//h->data[index] = value;
	if(h->copyFct)
        h->copyFct(h->data+index*h->elemSize, value);
    else
        memcpy(h->data+index*h->elemSize, value, h->elemSize);
}



void heapPop(Heap h)
{
	unsigned int index, swap, other;

	// Remove the biggest element
	Ptr temp = h->data+(--h->length)*h->elemSize;

	// Resize the heap if it's consuming too much memory
	if ((h->length <= (h->size >> 2)) && (h->size > DEFSIZE))
	{
		h->size >>= 1;
		h->data = realloc(h->data, h->elemSize * h->size);
	}

	// Reorder the elements
	for(index = 0; 1; index = swap)
	{
		// Find the child to swap with
		swap = (index << 1) + 1;

		if (swap >= h->length)
            break; // If there are no children, the heap is reordered

		other = swap + 1;
		if ((other < h->length) && h->cmpFct(h->data+other*h->elemSize, h->data+swap*h->elemSize) >= 0)
            swap = other;

		if (h->cmpFct(temp, h->data+swap*h->elemSize) >= 0)
            break; // If the bigger child is less than or equal to its parent, the heap is reordered

		//h->data[index] = h->data[swap];
		if(h->delFct)
            h->delFct(h->data+index*h->elemSize);

		if(h->copyFct)
            h->copyFct(h->data+index*h->elemSize, h->data+swap*h->elemSize);
        else
            memcpy(h->data+index*h->elemSize, h->data+swap*h->elemSize, h->elemSize);
	}
	//h->data[index] = temp;
	if(h->delFct)
        h->delFct(h->data+index*h->elemSize);

	if(h->copyFct)
        h->copyFct(h->data+index*h->elemSize, temp);
    else
        memcpy(h->data+index*h->elemSize, temp, h->elemSize);

    if(h->delFct)
        h->delFct(temp);
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
