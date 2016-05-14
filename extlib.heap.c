//#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "extlib.def.h"
#include "extlib.heap.h"

//#define CMP(a, b) ((a) >= (b))

struct _Heap {
	unsigned int size; // Size of the allocated memory (in number of items)
	unsigned int count; // Count of the elements in the heap
	ElCmpFct cmpFct;
    int elemSize;
    Ptr data; // Array with the elements
};

//static const unsigned int base_size = 4;

#define DEFSIZE 8

// Prepares the heap for use
Heap heapNew(int elemSize)
{
    Heap h = malloc(sizeof(struct _Heap));

    if(elemSize<=0) {
        h->elemSize=_elSizeFct(elemSize);
        h->cmpFct=_elCompareFct(elemSize);
    }
    else {
        h->elemSize=elemSize;
        h->cmpFct=NULL;
    }

    h->size = DEFSIZE;
    h->count = 0;
    h->data = malloc(DEFSIZE*h->elemSize);

	if (!h->data) exit(1); // Exit if the memory allocation fails

	return h;
}

// Inserts element to the heap
void heapPush_base(Heap h, Ptr value)
{
	unsigned int index, parent;

	// Resize the heap if it is too small to hold all the data
	if (h->count == h->size)
	{
		h->size <<= 1;
		h->data = realloc(h->data, h->elemSize * h->size);
		if (!h->data) {printf("b");exit(1);} // Exit if the memory allocation fails
	}

	// Find out where to put the element and put it
	for(index = h->count++; index; index = parent)
	{
		parent = (index - 1) >> 1;
		if (h->cmpFct(h->data+parent*h->elemSize, value) >= 0) break;
		//h->data[index] = h->data[parent];
		memcpy(h->data+index*h->elemSize, h->data+parent*h->elemSize, h->elemSize);
	}
	//h->data[index] = value;
	memcpy(h->data+index*h->elemSize, value, h->elemSize);
}

// Removes the biggest element from the heap
void heapPop(Heap h)
{
	unsigned int index, swap, other;

	// Remove the biggest element
	Ptr temp = h->data+(--h->count)*h->elemSize;

	// Resize the heap if it's consuming too much memory
	if ((h->count <= (h->size >> 2)) && (h->size > DEFSIZE))
	{
		h->size >>= 1;
		h->data = realloc(h->data, h->elemSize * h->size);
		if (!h->data) {printf("c");exit(1);} // Exit if the memory allocation fails
	}

	// Reorder the elements
	for(index = 0; 1; index = swap)
	{
		// Find the child to swap with
		swap = (index << 1) + 1;
		if (swap >= h->count) break; // If there are no children, the heap is reordered
		other = swap + 1;
		if ((other < h->count) && h->cmpFct(h->data+other*h->elemSize, h->data+swap*h->elemSize) >= 0) swap = other;
		if (h->cmpFct(temp, h->data+swap*h->elemSize) >= 0) break; // If the bigger child is less than or equal to its parent, the heap is reordered

		//h->data[index] = h->data[swap];
		memcpy(h->data+index*h->elemSize, h->data+swap*h->elemSize, h->elemSize);
	}
	//h->data[index] = temp;
	memcpy(h->data+index*h->elemSize, temp, h->elemSize);
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

Ptr heapGet_base(Heap h) {
    return h->data;
}

void heapDel(Heap h) {
    free(h->data);
    free(h);
}

unsigned int heapCount(Heap h) {
    return h->count;
}

Heap heapComparable(Heap heap, ElCmpFct fct) {
    heap->cmpFct=fct;
    return heap;
}
