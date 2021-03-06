/**
 * \file SimpleList.h
 * \brief Primitives functions for singly-linked lists
 * \author Jason Pindat
 * \date 04/21/2014
 *
 * All the basic functions to manage singly-linked lists, every function is in constant complexity except sllDel which can be linear if items remains in the list when destroying it.
 * SimpleList is an Iterable Collection.
 *
 * Copyright 2014-2016
 *
 */

#ifndef EXTLIB_SIMPLELIST_H
#define EXTLIB_SIMPLELIST_H

#include "Common.h"

/** SimpleList : type for a singly-linked list. */
typedef struct _SimpleList *SimpleList;

/** SimpleListNode : type for a node in a singly-linked list. */
typedef struct _SimpleListNode *SimpleListNode;



/** \brief Creates a new singly-linked list.
 *
 * \param elemSize : the size in bytes of each element of the list. You can use the EL_* constants for the basic types, this will automatically link the comparison function too.
 * \return New empty list.
 *
 */
SimpleList simpleListNew(int elemSize);

/** \brief Destroys a list and all its content (Not primitive).
 *
 * \param l : List to destroy.
 * \return nothing.
 *
 */
void simpleListDel(SimpleList l);



/** \brief Sets the function to compare 2 elements of this list, note that if you declared the list with EL_*, the comparison function of the specified type is automatically linked.
 *
 * \param l : Singly-linked list in which you set the fonction.
 * \param fct : pointer to the function, the function must take 2 pointers to the data and return an int which is <0 if 1st value is lower tha 2nd, >0 for the opposite and =0 if 1st value equals 2nd value.
 * \return nothing.
 *
 */
void simpleListComparable(SimpleList l, ElCmpFct fct);

/** \brief Sets the functions to copy an element and to delete an element of this collection. If not called, the elements will be copied bit by bit. /!\ Must be set before any SimpleList update
 *
 * \param l : SimpleList in which you set the fonction.
 * \param copyFct : pointer to the copy function, the function must take 2 pointers, the first is the new allocated element to initialize and the second is the source element and return nothing.
 * \param delFct : pointer to the deletion function, the function must take a pointer to the element to destroy. Note that this pointer will be automatically freed, so delFct must'nt do this.
 * \return nothing.
 *
 */
void simpleListElementInstanciable(SimpleList l, ElCopyFct copyFct, ElDelFct delFct);



/** \brief Copies a list and all its content.
 *
 * \param l : List to copy.
 * \return Copy of the list
 *
 */
SimpleList simpleListClone(const SimpleList l);



/** \brief Removes the whole content of a list (Not primitive).
 *
 * \param l : List to clear.
 * \return nothing.
 *
 */
void simpleListClear(SimpleList l);



/** \brief Tells whether a list is empty or not
 *
 * \param l : List to look in.
 * \return true if empty, false if not.
 *
 */
bool simpleListIsEmpty(const SimpleList l);

/** \brief Returns the size of the list.
 *
 * \param list : List to count elements.
 * \return Number of elements.
 *
 */
int simpleListLength(const SimpleList l);



/** \brief Tells whether the list contains an element or not, simpleListComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param l : List to look into.
 * \param data : Element to seek.
 * \return true if found, false otherwise.
 *
 */
bool simpleListContains(const SimpleList l, const Ptr data);



/** \brief Returns the first element of a given list.
 *
 * \param l : List to seek in.
 * \return First element of the list.
 *
 */
const Ptr simpleListGetFirst_base(const SimpleList l);
#define simpleListGetFirst(l, type) (*(type*)simpleListGetFirst_base(l))



/** \brief Updates the data of the first element of the list.
 *
 * \param l : List to update.
 * \param data : Pointer to the new data.
 * \return nothing.
 *
 */
void simpleListSetFirst_base(SimpleList l, const Ptr data);
/** Automatic macro to send the address of data to simpleListSetFirst_base */
#define simpleListSetFirst(l, data) simpleListSetFirst_base(l, &(data))
#define simpleListSetFirstI(l, data, type) {type tmp = (data); simpleListSetFirst_base(l, &(tmp));}



/** \brief Adds a new element at the beginning of the list.
 *
 * \param l : List to add in.
 * \param data : Pointer to the data to add.
 * \return nothing.
 *
 */
void simpleListAddFirst_base(SimpleList l, const Ptr data);
/** Automatic macro to send the address of data to simpleListAddFirst_base */
#define simpleListAddFirst(l, data) simpleListAddFirst_base(l, &(data))
#define simpleListAddFirstI(l, data, type) {type tmp = (data); simpleListAddFirst_base(l, &(tmp));}



/** \brief Removes the first element of a list.
 *
 * \param l : List to remove in.
 * \return nothing.
 *
 */
void simpleListRemoveFirst(SimpleList l);



/** \brief Sorts the list with a merge sort, simpleListComparable must have been called or the list must have been created with a EL_* constant.
 *
 * \param l : List to sort.
 * \param method : must be EL_ASC or EL_DESC.
 * \return nothing.
 *
 */
void simpleListSort(SimpleList l, int method);



/** \brief Details the heap usage of a given list
 *
 * \param l : List to seek in
 * \return void
 *
 */
void simpleListDump(const SimpleList l);



// Iteration

typedef struct {
    SimpleList list;
    SimpleListNode prevNode;
    SimpleListNode node;
    bool onNext;
} SimpleListIt;



/** \brief Creates an iterator on the list (Starting with the first element).
 *
 * \param l : List to iterate.
 * \return Iterator on this list.
 *
 */
SimpleListIt simpleListItNew(const SimpleList l);



/** \brief Determines whether the element pointed by the iterator exists or it is the end of the iteration
 *
 * \param it : Iterator on a list.
 * \return true if element exists, false otherwise.
 *
 */
bool simpleListItExists(const SimpleListIt *it);



/** \brief Positions the iterator on the next element
 *
 * \param it : Iterator on a list.
 * \return nothing.
 *
 */
void simpleListItNext(SimpleListIt *it);



/** \brief Returns the element pointed by the iterator
 *
 * \param it : Iterator on a list.
 * \return element.
 *
 */
const Ptr simpleListItGet_base(const SimpleListIt *it);
#define simpleListItGet(it, type) (*(type*)simpleListItGet_base(it))



/** \brief Updates the element pointed by the iterator
 *
 * \param it : Iterator on a list.
 * \return nothing.
 *
 */
void simpleListItSet_base(SimpleListIt *it, const Ptr data);
#define simpleListItSet(it, data) simpleListItSet_base(it, &(data))
#define simpleListItSetI(it, data, type) {type tmp = (data); simpleListItSet_base(it, &(tmp));}



/** \brief Adds a new element after the position pointed by the iterator.
 *
 * \param it : Iterator on a list.
 * \param data : Pointer to the data to add.
 * \return nothing.
 *
 */
void simpleListItAddAfter_base(SimpleListIt *it, const Ptr data);
/** Automatic macro to send the address of data to simpleListItAddAfter_base */
#define simpleListItAddAfter(it, data) simpleListItAddAfter_base(it, &(data))
#define simpleListIAddAfterI(it, data, type) {type tmp = (data); simpleListItAddAfter_base(it, &(tmp));}



/** \brief Removes the element pointed by the iterator.
 *
 * \param it : Iterator on a list.
 * \return nothing.
 *
 */
void simpleListItRemove(SimpleListIt *it);



/** \brief makes an action for every element of the list
 *
 * \param l : List to iterate.
 * \param actFct : Pointer to a function called for each element of the list.
 * \param infos : pointer broadcasted to actFct. Useful to share additional informations to the function.
 * \return nothing.
 *
 */
void simpleListForEach(SimpleList l, ElActFct actFct, Ptr infos);

#endif
