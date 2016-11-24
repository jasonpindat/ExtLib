/**
 * \file List.h
 * \brief Primitives functions for doubly-linked lists
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * All the basic functions to manage doubly-linked lists.
 * List is an Iterable Collection
 *
 * Copyright 2014-2016
 *
 */

#ifndef EXTLIB_LIST_H
#define EXTLIB_LIST_H

#include "Common.h"

/** List : type for a doubly-linked list. */
typedef struct _List *List;

/** ListNode : type for a node in a doubly-linked list. */
typedef struct _ListNode *ListNode;



/** \brief Creates a new doubly-linked list.
 *
 * \param elemSize : the size in bytes of each element of the list. You can use the EL_* constants for the basic types, this will automatically link the comparison function too.
 * \return New empty list.
 *
 */
List listNew(int elemSize);

/** \brief Destroys a list and all its content (Not primitive).
 *
 * \param l : List to destroy.
 * \return void
 *
 */
void listDel(List l);



/** \brief Sets the function to compare 2 elements of this list, note that if you declared the list with EL_*, the comparison function of the specified type is automatically linked.
 *
 * \param l : Doubly-linked list in which you set the fonction.
 * \param fct : pointer to the function, the function must take 2 pointers to the data and return an int which is <0 if 1st value is lower tha 2nd, >0 for the opposite and =0 if 1st value equals 2nd value.
 * \return nothing.
 *
 */
void listComparable(List l, ElCmpFct fct);

/** \brief Sets whether the list can be accessed for reading by multiple threads.
 *
 * \param l : List.
 * \param multithread : boolean value to allow multithreaded access.
 * \return nothing.
 *
 */
void listMultithread(List l, bool multithread);



/** \brief Copies a list and all its content.
 *
 * \param l : List to copy.
 * \return Copy of the list
 *
 */
List listClone(List l);



/** \brief Removes the whole content of a list (Not primitive).
 *
 * \param l : List to clear.
 * \return nothing.
 *
 */
void listClear(List l);



/** \brief Tells whether a list is empty or not
 *
 * \param l : List to look in.
 * \return true if empty, false if not.
 *
 */
bool listIsEmpty(List l);

/** \brief Returns the size of the list.
 *
 * \param l : List to count elements.
 * \return Number of elements.
 *
 */
int listLength(List l);



/** \brief Tells whether the list contains an element or not, listComparable must have been called or the vector must have been created with a EL_* constant.
 *
 * \param l : List to look into.
 * \param data : Element to seek.
 * \return true if found, false otherwise.
 *
 */
bool listContains(List l, Ptr data);



/** \brief Returns the first element of a given list.
 *
 * \param l : List to seek in.
 * \return First element of the list.
 *
 */
Ptr listGetFirst_base(List l);
#define listGetFirst(l, type) (*(type*)listGetFirst_base(l))

/** \brief Returns the last element of a given list.
 *
 * \param l : List to seek in.
 * \return Last element of the list.
 *
 */
Ptr listGetLast_base(List l);
#define listGetLast(l, type) (*(type*)listGetLast_base(l))



/** \brief Updates the data of the first element of the list.
 *
 * \param l : List to update.
 * \param data : Pointer to the new data.
 * \return nothing.
 *
 */
void listSetFirst_base(List l, Ptr data);
/** Automatic macro to send the address of data to listSetFirst_base */
#define listSetFirst(l, data) listSetFirst_base(l, &(data))

/** \brief Updates the data of the last element of the list.
 *
 * \param l : List to update.
 * \param data : Pointer to the new data.
 * \return nothing.
 *
 */
void dllSetLast_base(List l, Ptr data);
/** Automatic macro to send the address of data to dllSetLast_base */
#define dllSetLast(l, data) dllSetLast_base(l, &(data))



/** \brief Adds a new element at the beginning of the list.
 *
 * \param l : List to add in.
 * \param data : Pointer to the data to add.
 * \return nothing.
 *
 */
void listAddFirst_base(List l, Ptr data);
/** Automatic macro to send the address of data to listAddFirst_base */
#define listAddFirst(l, data) listAddFirst_base(l, &(data))

/** \brief Adds a new node at the end of the list.
 *
 * \param l : List to add in.
 * \param data : Pointer to the data to add.
 * \return nothing.
 *
 */
void listAddLast_base(List l, Ptr data);
/** Automatic macro to send the address of data to listAddLast_base */
#define listAddLast(l, data) listAddLast_base(l, &(data))



/** \brief Removes the first element of a list.
 *
 * \param l : List to remove in.
 * \return nothing.
 *
 */
void listRemoveFirst(List l);

/** \brief Removes the last element of a list.
 *
 * \param l : List to remove in.
 * \return nothing.
 *
 */
void listRemoveLast(List l);



/** \brief Sorts the list with a merge sort, listComparable must have been called or the list must have been created with a EL_* constant.
 *
 * \param l : List to sort.
 * \param method : must be EL_ASC or EL_DESC.
 * \return nothing.
 *
 */
void listSort(List l, int method);



/** \brief Details the heap usage of a given list
 *
 * \param l : List to dump
 * \return void
 *
 */
void listDump(List l);



// Iteration

typedef struct {
    List list;
    ListNode node;
} ListIt;



/** \brief Creates an iterator on the list (Starting with the first element).
 *
 * \param l : List to iterate.
 * \return Iterator on this list.
 *
 */
ListIt listItNew(List l);

/** \brief Creates an iterator on the list (Starting with the last element).
 *
 * \param l : List to iterate.
 * \return Iterator on this list.
 *
 */
ListIt listItNewBack(List l);



/** \brief Determines whether the element pointed by the iterator exists or it is the end of the iteration
 *
 * \param it : Iterator on a list.
 * \return true if element exists, false otherwise.
 *
 */
bool listItExists(ListIt *it);



/** \brief Positions the iterator on the next element
 *
 * \param it : Iterator on a list.
 * \return nothing.
 *
 */
void listItNext(ListIt *it);

/** \brief Positions the iterator on the previous element
 *
 * \param it : Iterator on a list.
 * \return nothing.
 *
 */
void listItPrev(ListIt *it);



/** \brief Returns the element pointed by the iterator
 *
 * \param it : Iterator on a list.
 * \return element.
 *
 */
Ptr listItGet_base(ListIt *it);
#define listItGet(it, type) (*(type*)listItGet_base(it))



/** \brief Updates the element pointed by the iterator
 *
 * \param it : Iterator on a list.
 * \return nothing.
 *
 */
void listItSet_base(ListIt *it, Ptr data);
#define listItSet(it, data) listItSet_base(it, &(data))



/** \brief Adds a new element after the position pointed by the iterator.
 *
 * \param it : Iterator on a list.
 * \param data : Pointer to the data to add.
 * \return nothing.
 *
 */
void listItAddAfter_base(ListIt *it, Ptr data);
/** Automatic macro to send the address of data to listItAddAfter_base */
#define listItAddAfter(it, data) listItAddAfter_base(it, &(data))

/** \brief Adds a new element before the position pointed by the iterator.
 *
 * \param it : Iterator on a list.
 * \param data : Pointer to the data to add.
 * \return nothing.
 *
 */
void listItAddBefore_base(ListIt *it, Ptr data);
/** Automatic macro to send the address of data to listItAddBefore_base */
#define listItAddBefore(it, data) listItAddBefore_base(it, &(data))



/** \brief Removes the element pointed by the iterator.
 *
 * \param it : Iterator on a list.
 * \return nothing.
 *
 */
void listItRemove(ListIt *it);



/** \brief makes an action for every element of the list
 *
 * \param l : List to iterate.
 * \param actFct : Pointer to a function called for each element of the list.
 * \param infos : pointer broadcasted to actFct. Useful to share additional informations to the function.
 * \return nothing.
 *
 */
void listForEach(List l, ElActFct actFct, Ptr infos);

#endif
