/**
 * \file Heap.h
 * \brief Primitives functions for heaps
 * \author Jason Pindat
 * \date 2016-11-26
 *
 * All the basic functions to manage heaps.
 * Heap is a Collection but is not Iterable
 *
 * Copyright 2014-2016
 *
 */

#ifndef EXTLIB_HEAP_H
#define EXTLIB_HEAP_H

#include "Common.h"

/** Heap : type for a heap. */
typedef struct _Heap *Heap;



/** \brief Creates a new heap.
 *
 * \param elemSize : the size in bytes of each element of the heap. You can use the EL_* constants for the basic types, this will automatically link the comparison function too.
 * \param cmpFct : pointer to the comparison function, the function must take 2 pointers to the data and return an int which is <0 if 1st value is lower tha 2nd, >0 for the opposite and =0 if 1st value equals 2nd value.
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



/** \brief Sets the function to compare 2 elements of this heap, note that if you declared the heap with EL_*, the comparison function of the specified type is automatically linked. Otherwise, a function must be provided.
 *
 * \param h : Heap in which you set the fonction.
 * \param fct : pointer to the function, the function must take 2 pointers to the data and return an int which is <0 if 1st value is lower tha 2nd, >0 for the opposite and =0 if 1st value equals 2nd value.
 * \return nothing.
 *
 */
void heapComparable(Heap h, ElCmpFct fct);



/** \brief Copies a heap and all its content.
 *
 * \param h : Heap to copy.
 * \return Copy of the heap
 *
 */
Heap heapClone(Heap h);



/** \brief Removes the whole content of a heap.
 *
 * \param h : Heap to clear.
 * \return nothing.
 *
 */
void heapClear(Heap h);



/** \brief Tells whether a heap is empty or not
 *
 * \param h : Heap to look in.
 * \return true if empty, false if not.
 *
 */
bool heapIsEmpty(Heap h);

/** \brief Returns the length of the heap.
 *
 * \param h : Heap to count elements.
 * \return Number of elements.
 *
 */
int heapLength(Heap h);



/** \brief Returns the element with the highest priority in the heap.
 *
 * \param h : Heap to seek in.
 * \return Pointer to data.
 *
 */
Ptr heapGet_base(Heap h);
/** Automatic cast macro to translate Ptr returned by heapGet_base into type */
#define heapGet(h, type) (*(type*)heapGet_base(h))



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



/** \brief Details the heap usage of a given heap
 *
 * \param h : Heap to dump.
 * \return nothing.
 *
 */
void heapDump(Heap h);



//void heapHeapify(Ptr data, unsigned int count);

#endif
