/**
 * \file Array.h
 * \brief Primitives functions for arrays
 * \author Jason Pindat
 * \date 2016-11-26
 *
 * All the basic functions to manage dynamic one-dimention arrays.
 * Array is an Iterable Collection.
 *
 * Copyright 2014-2016
 *
 */

#ifndef EXTLIB_ARRAY_H
#define EXTLIB_ARRAY_H

#include "Common.h"

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

/** \brief Sets the functions to copy an element and to delete an element of this collection. If not called, the elements will be copied bit by bit. /!\ Must be set before any Array update
 *
 * \param a : Array in which you set the fonction.
 * \param copyFct : pointer to the copy function, the function must take 2 pointers, the first is the new allocated element to initialize and the second is the source element and return nothing.
 * \param delFct : pointer to the deletion function, the function must take a pointer to the element to destroy. Note that this pointer will be automatically freed, so delFct must'nt do this.
 * \return nothing.
 *
 */
void arrayElementInstanciable(Array a, ElCopyFct copyFct, ElDelFct delFct);



/** \brief Copies an array and all its content.
 *
 * \param a : Array to copy.
 * \return Copy of the array
 *
 */
Array arrayClone(const Array a);

/** \brief Returns a sub-array and all its content.
 *
 * \param a : Array to copy.
 * \param from : start index (included).
 * \param to : end index (excluded).
 * \return Copy of the array
 *
 */
Array arraySubArray(const Array a, int from, int to);



/** \brief Removes the whole content of an array.
 *
 * \param a : Array to clear.
 * \return nothing.
 *
 */
void arrayClear(Array a);



/** \brief Tells whether an array is empty or not
 *
 * \param a : Array to look in.
 * \return true if empty, false if not.
 *
 */
bool arrayIsEmpty(const Array a);

/** \brief Returns the length of the array.
 *
 * \param a : Array to count elements.
 * \return Number of elements.
 *
 */
int arrayLength(const Array a);



/** \brief Makes the capacity of the array fit the data inside it while freeing all the preallocated memory.
 *
 * \param a : Array to trim.
 * \return nothing.
 *
 */
void arrayTrimCapacity(Array a);



/** \brief Tells whether the array contains an element or not, arrayComparable must have been called or the array must have been created with a EL_* constant.
 *
 * \param a : Array to look into.
 * \param data : Element to seek.
 * \return true if found, false otherwise.
 *
 */
bool arrayContains(const Array a, const Ptr data);

/** \brief Returns the index of the first occurrence of an element in the array, arrayComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param a : Array to look into.
 * \param data : Element to seek.
 * \return first index if found, -1 otherwise.
 *
 */
int arrayIndexOf(const Array a, const Ptr data);

/** \brief Returns the index of the last occurrence of an element in the array, arrayComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param a : Array to look into.
 * \param data : Element to seek.
 * \return last index if found, -1 otherwise.
 *
 */
int arrayLastIndexOf(const Array a, const Ptr data);

/** \brief Returns the index of an element in the array, array must be sorted in ascending order, arrayComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param a : Array to look into.
 * \param data : Element to seek.
 * \return index if found, -1 otherwise.
 *
 */
int arrayBSearch(const Array a, const Ptr data);



/** \brief Returns the element at a given position in an array.
 *
 * \param a : Array to seek in.
 * \param pos : Position of the element, 0 is start.
 * \return Pointer to data.
 *
 */
const Ptr arrayGet_base(const Array a, int pos);
#define arrayGet(a, pos, type) (*(type*)arrayGet_base(a, pos))



/** \brief Sets the element at a given position in an array.
 *
 * \param a : Array to modify.
 * \param pos : Position of the element, 0 is start.
 * \param data : Pointer to the data.
 * \return nothing.
 *
 */
void arraySet_base(Array a, int pos, const Ptr data);
#define arraySet(a, pos, data) arraySet_base(a, pos, &(data))
#define arraySetI(a, pos, data, type) {type tmp = (data); arraySet_base(a, pos, &(tmp));}



/** \brief Adds an element at the end of an array.
 *
 * \param a : Array to add in.
 * \param data : Pointer to the data.
 * \return nothing.
 *
 */
void arrayPush_base(Array a, const Ptr data);
#define arrayPush(a, data) arrayPush_base(a, &(data))
#define arrayPushI(a, data, type) {type tmp = (data); arrayPush_base(a, &(tmp));}

/** \brief Adds an element at a given position in an array (Not primitive).
 *
 * \param a : Array to add in.
 * \param pos : Position for the element, 0 is start.
 * \param data : Pointer to the data.
 * \return nothing.
 *
 */
void arrayAdd_base(Array a, int pos, const Ptr data);
#define arrayAdd(a, pos, data) arrayAdd_base(a, pos, &(data))
#define arrayAddI(a, pos, data, type) {type tmp = (data); arrayAdd_base(a, pos, &(tmp));}



/** \brief Removes the last element in the array.
 *
 * \param a : Array to remove in.
 * \return nothing.
 *
 */
void arrayPop(Array a);

/** \brief Removes the element at a given position in the array (Not primitive).
 *
 * \param a : Array to remove in.
 * \param pos : Position of the element, 0 is start.
 * \return nothing.
 *
 */
void arrayRemove(Array a, int pos);



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
 * \param a : Array to dump.
 * \return nothing.
 *
 */
void arrayDump(const Array a);



// Iteration

typedef struct {
    Array array;
    int index;
    bool onNext;
} ArrayIt;



/** \brief Creates an iterator on the array (Starting with the first element).
 *
 * \param a : Array to iterate.
 * \return Iterator on this array.
 *
 */
ArrayIt arrayItNew(const Array a);

/** \brief Creates an iterator on the array (Starting with the last element).
 *
 * \param a : Array to iterate.
 * \return Iterator on this array.
 *
 */
ArrayIt arrayItNewBack(const Array a);



/** \brief Determines whether the element pointed by the iterator exists or it is the end of the iteration
 *
 * \param it : Iterator on an array.
 * \return true if element exists, false otherwise.
 *
 */
bool arrayItExists(const ArrayIt *it);



/** \brief Positions the iterator on the next element
 *
 * \param it : Iterator on an array.
 * \return nothing.
 *
 */
void arrayItNext(ArrayIt *it);

/** \brief Positions the iterator on the previous element
 *
 * \param it : Iterator on an array.
 * \return nothing.
 *
 */
void arrayItPrev(ArrayIt *it);



/** \brief Returns the element pointed by the iterator
 *
 * \param it : Iterator on an array.
 * \return element.
 *
 */
const Ptr arrayItGet_base(const ArrayIt *it);
#define arrayItGet(it, type) (*(type*)arrayItGet_base(it))



/** \brief Updates the element pointed by the iterator
 *
 * \param it : Iterator on an array.
 * \return nothing.
 *
 */
void arrayItSet_base(ArrayIt *it, const Ptr data);
#define arrayItSet(it, data) arrayItSet_base(it, &(data))
#define arrayItSetI(it, data, type) {type tmp = (data); arrayItSet_base(it, &(tmp));}



/** \brief Adds a new element after the position pointed by the iterator.
 *
 * \param it : Iterator on an array.
 * \param data : Pointer to the data to add.
 * \return nothing.
 *
 */
void arrayItAddAfter_base(ArrayIt *it, const Ptr data);
/** Automatic macro to send the address of data to arrayItAddAfter_base */
#define arrayItAddAfter(it, data) arrayItAddAfter_base(it, &(data))
#define arrayItAddAfterI(it, data, type) {type tmp = (data); arrayItAddAfter_base(it, &(tmp));}

/** \brief Adds a new element before the position pointed by the iterator.
 *
 * \param it : Iterator on an array.
 * \param data : Pointer to the data to add.
 * \return nothing.
 *
 */
void arrayItAddBefore_base(ArrayIt *it, const Ptr data);
/** Automatic macro to send the address of data to arrayItAddBefore_base */
#define arrayItAddBefore(it, data) arrayItAddBefore_base(it, &(data))
#define arrayItAddBeforeI(it, data, type) {type tmp = (data); arrayItAddBefore_base(it, &(tmp));}



/** \brief Removes the element pointed by the iterator.
 *
 * \param it : Iterator on an array.
 * \return nothing.
 *
 */
void arrayItRemove(ArrayIt *it);



/** \brief makes an action for every element of the array
 *
 * \param a : Array to iterate.
 * \param actFct : Pointer to a function called for each element of the array.
 * \param infos : pointer broadcasted to actFct. Useful to share additional informations to the function.
 * \return nothing.
 *
 */
void arrayForEach(Array a, ElActFct actFct, Ptr infos);

#endif
