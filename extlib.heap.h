/**
 * \file extlib.heap.h
 * \brief Primitives functions for heaps
 * \author Jason Pindat
 * \version 1.0
 * \date 03/16/2016
 *
 * All the basic functions to manage heaps.
 *
 */

#ifndef EXTLIB_HEAP_H
#define EXTLIB_HEAP_H

#include "extlib.def.h"

/** Heap : type for a heap. */
typedef struct _Heap *Heap;

//typedef int type;

/** \brief Creates a new heap.
 *
 * \param elemSize : the size in bytes of each element of the heap. You can use the EL_* constants for the basic types, this will automatically link the comparison function too.
 * \return New empty heap.
 *
 */
Heap heapNew(int elemSize);

/** \brief Destroys a heap and all its content (Not primitive).
 *
 * \param h : Heap to destroy.
 * \return void
 *
 */
void heapDel(Heap h);


/** \brief Sets the function to compare 2 elements of this heap, note that if you declared the heap with EL_*, the comparison function of the specified type is automatically linked.
 *
 * \param heap : Heap in which you set the fonction.
 * \param fct : pointer to the function, the function must take 2 pointers to the data and return an int which is <0 if 1st value is lower tha 2nd, >0 for the opposite and =0 if 1st value equals 2nd value.
 * \return The same heap.
 *
 */
Heap heapComparable(Heap heap, ElCmpFct fct);


/** \brief Adds an element in a heap.
 *
 * \param h : Heap to add in.
 * \param data : Pointer to the data.
 * \return The same vector.
 *
 */
void heapPush_base(Heap h, Ptr data);
/** Automatic macro to send the address of data to heapPush_base */
#define heapPush(h, data) heapPush_base(h, &(data))

/** \brief Removes the element with the highest priority.
 *
 * \param h : Heap to remove in.
 * \return void
 *
 */
void heapPop(Heap h);

/** \brief Returns the element with the highest priority in the heap.
 *
 * \param h : Heap to seek in.
 * \return Pointer to data.
 *
 */
Ptr heapGet_base(Heap h);
/** Automatic cast macro to translate Ptr returned by heapGet_base into type */
#define heapGet(h, type) (*(type*)heapGet_base(h))


/** \brief Returns the size of the heap.
 *
 * \param h : Heap to count elements.
 * \return Number of elements.
 *
 */
unsigned int heapCount(Heap h);

// Returns the biggest element in the heap
//#define heapGet(h) (*(h)->data)

// Frees the allocated memory
//#define heapDel(h) (free((h)->data))

//void heapHeapify(Ptr data, unsigned int count);


#endif
