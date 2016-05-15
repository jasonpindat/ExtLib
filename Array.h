/**
 * \file Array.h
 * \brief Primitives functions for arrays
 * \author Jason Pindat
 * \version 1.0
 * \date 05/21/2014
 *
 * All the basic functions to manage dynamic one-dimention arrays.
 *
 */

#ifndef EXTLIB_ARRAY_H
#define EXTLIB_ARRAY_H

#include "Common.h"
#include "Iterator.h"

/** Array : type for an array. */
typedef struct _Array *Array;



/** \brief Creates a new array.
 *
 * \param elemSize : the size in bytes of each element of the array. You can use the EL_* constants for the basic types, this will automatically link the comparison function too.
 * \return New empty array.
 *
 */
Array arrayNew(int elemSize);

/** \brief Destroys an array and all its content.
 *
 * \param a : Array to destroy.
 * \return void
 *
 */
void arrayDel(Array a);


/** \brief Sets the function to compare 2 elements of this array, note that if you declared the array with EL_*, the comparison function of the specified type is automatically linked.
 *
 * \param a : Array in which you set the fonction.
 * \param fct : pointer to the function, the function must take 2 pointers to the data and return an int which is <0 if 1st value is lower tha 2nd, >0 for the opposite and =0 if 1st value equals 2nd value.
 * \return nothing.
 *
 */
void arrayComparable(Array a, ElCmpFct fct);

/** \brief Sets the functions to copy an element and to delete an element of this array. If NULL or arrayElementCopy is not called, the elements will be copied bit by bit.
 *
 * \param a : Array in which you set the fonction.
 * \param copyFct : pointer to the copy function, the function must take 2 pointers, the first is the source element and the second is the new allocated element to initialize and return nothing.
 * \param delFct : pointer to the deletion function, the function must take a pointer to the element to destroy. Note that this pointer will be automatically freed, so delFct must'nt do this.
 * \return nothing.
 *
 */
void arrayElementCopy(Array a, ElCopyFct copyFct, ElDelFct delFct);



/** \brief Copies an array and all its content.
 *
 * \param a : Array to copy.
 * \return Copy of the array
 *
 */
Array arrayClone(Array a);

/** \brief Returns a sub-array and all its content.
 *
 * \param a : Array to copy.
 * \param from : start index (included).
 * \param to : end index (excluded).
 * \return Copy of the array
 *
 */
Array arraySubArray(Array a, int from, int to);



/** \brief Removes the whole content of an array.
 *
 * \param a : Array to clear.
 * \return nothing.
 *
 */
void arrayClear(Array a);



/** \brief Tells whether a vector is empty or not
 *
 * \param a : Array to look in.
 * \return true if empty, false if not.
 *
 */
bool arrayIsEmpty(Array a);

/** \brief Returns the length of the array.
 *
 * \param a : Array to count elements.
 * \return Number of elements.
 *
 */
int arrayLength(Array a);



/** \brief Makes the capacity of the array fit the data inside it while freeing all the preallocated memory.
 *
 * \param a : Array to trim.
 * \return nothing.
 *
 */
void arrayTrimCapacity(Array a);



/** \brief Tells if the array contains an element or not, arrayComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param a : Array to look into.
 * \param data : Element to seek.
 * \return true if found, false otherwise.
 *
 */
bool arrayContains(Array a, Ptr data);

/** \brief Returns the index of the first occurrence of an element in the array, arrayComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param a : Array to look into.
 * \param data : Element to seek.
 * \return first index if found, -1 otherwise.
 *
 */
int arrayIndexOf(Array a, Ptr data);

/** \brief Returns the index of the last occurrence of an element in the array, arrayComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param a : Array to look into.
 * \param data : Element to seek.
 * \return last index if found, -1 otherwise.
 *
 */

int arrayLastIndexOf(Array a, Ptr data);



/** \brief Returns the element at a given position in an array.
 *
 * \param a : Array to seek in.
 * \param pos : Position of the element, 0 is start.
 * \return Pointer to data.
 *
 */
Ptr arrayGet_base(Array a, int pos);
#define arrayGet(a, pos, type) (*(type*)arrayGet_base(a, pos))



/** \brief Sets the element at a given position in an array.
 *
 * \param a : Array to modify.
 * \param pos : Position of the element, 0 is start.
 * \param data : Pointer to the data.
 * \return nothing.
 *
 */
void arraySet_base(Array a, int pos, Ptr data);
#define arraySet(a, pos, data) arraySet_base(a, pos, &(data))



/** \brief Adds an element at the end of an array.
 *
 * \param a : Array to add in.
 * \param data : Pointer to the data.
 * \return nothing.
 *
 */
void arrayAdd_base(Array a, Ptr data);
#define arrayAdd(a, data) arrayAdd_base(a, &(data))

/** \brief Adds an element at a given position in an array (Not primitive).
 *
 * \param a : Array to add in.
 * \param pos : Position for the element, 0 is start.
 * \param data : Pointer to the data.
 * \return nothing.
 *
 */
void arrayAddAt_base(Array a, int pos, Ptr data);
#define arrayAddAt(a, pos, data) arrayAddAt_base(a, pos, &(data))



/** \brief Removes the element at a given position in the array (Not primitive).
 *
 * \param a : Array to remove in.
 * \param pos : Position of the element, 0 is start.
 * \return nothing.
 *
 */
void arrayRemove(Array a, int pos);

/** \brief Removes the first found occurrence of the element in the array (Not primitive), arrayComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param a : Array to remove in.
 * \param data : Element to remove.
 * \return nothing.
 *
 */
void arrayRemoveElement(Array a, Ptr data);



/** \brief Sorts the array with a quick sort, arrayComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param a : Array to sort.
 * \param method : must be EL_ASC or EL_DESC.
 * \return nothing.
 *
 */
void arraySort(Array a, int method);

/** \brief Randomizes the array.
 *
 * \param a : Array to randomize.
 * \return nothing.
 *
 */
void arrayRandomize(Array a);



/** \brief Details the heap usage of a given array
 *
 * \param a : Array to seek in.
 * \return nothing.
 *
 */
void arrayHeap(Array a);


// Iteration

bool arrayItExists(Iterator *it);
void arrayItNext(Iterator *it);
Ptr arrayItGet(Iterator *it);

#endif
