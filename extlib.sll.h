/**
 * \file extlib.sll.h
 * \brief Primitives functions for singly-linked lists
 * \author Jason Pindat
 * \version 1.0
 * \date 04/21/2014
 *
 * All the basic functions to manage singly-linked lists, every function is in constant complexity except sllDel which can be linear if items remains in the list when destroying it.
 *
 */

#ifndef EXTLIB_SLL_H
#define EXTLIB_SLL_H

#include "extlib.def.h"

/** Sll : type for a singly-linked list. */
typedef struct _Sll *Sll;

/** SllNode : type for a node in a singly-linked list. */
typedef struct _SllNode *SllNode;



/** \brief Creates a new singly-linked list.
 *
 * \param elemSize : the size in bytes of each element of the list. You can use the EL_* constants for the basic types, this will automatically link the comparison function too.
 * \return New empty list.
 *
 */
Sll sllNew(int elemSize);

/** \brief Destroys a list and all its content (Not primitive).
 *
 * \param list : List to destroy.
 * \return void
 *
 */
void sllDel(Sll list);



/** \brief Tells whether a list is empty or not
 *
 * \param list : List to look in.
 * \return true if empty, false if not.
 *
 */
bool sllIsEmpty(Sll list);



/** \brief Returns a pointer to the data in a given node.
 *
 * \param node : Node that contains the data.
 * \return Generic pointer to the data.
 *
 */
Ptr sllGetData_base(SllNode node);
#define sllGetData(node, type) (*(type*)sllGetData_base(node))



/** \brief Returns the first node of a given list.
 *
 * \param list : List to seek in.
 * \return First node of the list, NIL if empty list.
 *
 */
SllNode sllGetFront(Sll list);

/** \brief Returns the node after a given node.
 *
 * \param node : Node to go after.
 * \return Node after the given one, NIL if end of list.
 *
 */
SllNode sllGetNext(SllNode node);



/** \brief Modifies the data of the node at the begining of the list.
 *
 * \param list : List to modify.
 * \param data : Pointer to the new data.
 * \return The modified node.
 *
 */
SllNode sllSetFront_base(Sll list, Ptr data);
#define sllSetFront(list, data) sllSetFront_base(list, &(data))

/** \brief Modifies the data of a given node in a list.
 *
 * \param list : List in which the node is.
 * \param node : The node to modify in.
 * \param data : Pointer to the new data.
 * \return The modified node.
 *
 */
SllNode sllSetNode_base(Sll list, SllNode node, Ptr data);
#define sllSetNode(list, node, data) sllSetNode_base(list, node, &(data))



/** \brief Adds a new node at the beginning of the list.
 *
 * \param list : List to add in.
 * \param data : Pointer to the data to add.
 * \return The added node.
 *
 */
SllNode sllPushFront_base(Sll list, Ptr data);
#define sllPushFront(list, data) sllPushFront_base(list, &(data))

/** \brief Adds a new node to a list after a given node.
 *
 * \param list : List to add in.
 * \param node : Node to add after.
 * \param data : Pointer to the data to add.
 * \return The added node.
 *
 */
SllNode sllPushAfter_base(Sll list, SllNode node, Ptr data);
#define sllPushAfter(list, node, data) sllPushAfter_base(list, node, &(data))



/** \brief Removes the first node of a list.
 *
 * \param list : List to remove in.
 * \return The same list.
 *
 */
Sll sllPopFront(Sll list);

/** \brief Removes the node after a given Node.
 *
 * \param node : Node to remove after.
 * \return The same node.
 *
 */
SllNode sllPopAfter(SllNode node);



/** \brief Details the heap usage of a given list
 *
 * \param list : List to seek in
 * \return void
 *
 */
void sllHeap(Sll list);

#endif
