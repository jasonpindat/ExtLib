
/**
 * \file extlib.dll.h
 * \brief Primitives functions for doubly-linked lists
 * \author Jason Pindat
 * \version 1.0
 * \date 29/21/2014
 *
 * All the basic functions to manage doubly-linked lists, every function is in constant complexity except dllDel which can be linear if items remains in the list when destroying it.
 *
 */

#ifndef EXTLIB_DLL_H
#define EXTLIB_DLL_H

#include "extlib.def.h"

/** Dll : type for a doubly-linked list. */
typedef struct _Dll *Dll;

/** DllNode : type for a node in a doubly-linked list. */
typedef struct _DllNode *DllNode;



/** \brief Creates a new doubly-linked list.
 *
 * \param elemSize : the size in bytes of each element of the list. You can use the EL_* constants for the basic types, this will automatically link the comparison function too.
 * \return New empty list.
 *
 */
Dll dllNew(int elemSize);

/** \brief Destroys a list and all its content (Not primitive).
 *
 * \param list : List to destroy.
 * \return void
 *
 */
void dllDel(Dll list);



/** \brief Tells whether a list is empty or not
 *
 * \param list : List to look in.
 * \return true if empty, false if not.
 *
 */
bool dllIsEmpty(Dll list);



/** \brief Returns a pointer to the data in a given node.
 *
 * \param node : Node that contains the data.
 * \return Generic pointer to the data.
 *
 */
Ptr dllGetData_base(DllNode node);
/** Automatic cast macro to translate Ptr returned by dllGetData_base into type */
#define dllGetData(node, type) (*(type*)dllGetData_base(node))



/** \brief Returns the first node of a given list.
 *
 * \param list : List to seek in.
 * \return First node of the list, NIL if empty list.
 *
 */
DllNode dllGetFront(Dll list);

/** \brief Returns the node after a given node.
 *
 * \param node : Node to go after.
 * \return Node after the given one, NIL if end of list.
 *
 */
DllNode dllGetNext(DllNode node);

/** \brief Returns the node before a given node.
 *
 * \param node : Node to go before.
 * \return Node before the given one, NIL if start of list.
 *
 */
DllNode dllGetPrev(DllNode node);

/** \brief Returns the last node of a given list.
 *
 * \param list : List to seek in.
 * \return Last node of the list, NIL if empty list.
 *
 */
DllNode dllGetBack(Dll list);



/** \brief Modifies the data of the node at the begining of the list.
 *
 * \param list : List to modify.
 * \param data : Pointer to the new data.
 * \return The modified node.
 *
 */
DllNode dllSetFront_base(Dll list, Ptr data);
/** Automatic macro to send the address of data to dllSetFront_base */
#define dllSetFront(list, data) dllSetFront_base(list, &(data))

/** \brief Modifies the data of a given node in a list.
 *
 * \param list : List in which the node is.
 * \param node : The node to modify in.
 * \param data : Pointer to the new data.
 * \return The modified node.
 *
 */
DllNode dllSetNode_base(Dll list, DllNode node, Ptr data);
/** Automatic macro to send the address of data to dllSetNode_base */
#define dllSetNode(list, node, data) dllSetNode_base(list, node, &(data))

/** \brief Modifies the data of the node at the end of the list.
 *
 * \param list : List to modify.
 * \param data : Pointer to the new data.
 * \return The modified node.
 *
 */
DllNode dllSetBack_base(Dll list, Ptr data);
/** Automatic macro to send the address of data to dllSetBack_base */
#define dllSetBack(list, data) dllSetBack_base(list, &(data))



/** \brief Adds a new node at the beginning of the list.
 *
 * \param list : List to add in.
 * \param data : Pointer to the data to add.
 * \return The added node.
 *
 */
DllNode dllPushFront_base(Dll list, Ptr data);
/** Automatic macro to send the address of data to dllPushFront_base */
#define dllPushFront(list, data) dllPushFront_base(list, &(data))

/** \brief Adds a new node to a list after a given node.
 *
 * \param list : List to add in.
 * \param node : Node to add after.
 * \param data : Pointer to the data to add.
 * \return The added node.
 *
 */
DllNode dllPushAfter_base(Dll list, DllNode node, Ptr data);
/** Automatic macro to send the address of data to dllPushAfter_base */
#define dllPushAfter(list, node, data) dllPushAfter_base(list, node, &(data))

/** \brief Adds a new node to a list before a given node.
 *
 * \param list : List to add in.
 * \param node : Node to add before.
 * \param data : Pointer to the data to add.
 * \return The added node.
 *
 */
DllNode dllPushBefore_base(Dll list, DllNode node, Ptr data);
/** Automatic macro to send the address of data to dllPushBefore_base */
#define dllPushBefore(list, node, data) dllPushBefore_base(list, node, &(data))

/** \brief Adds a new node at the end of the list.
 *
 * \param list : List to add in.
 * \param data : Pointer to the data to add.
 * \return The added node.
 *
 */
DllNode dllPushBack_base(Dll list, Ptr data);
/** Automatic macro to send the address of data to dllPushBack_base */
#define dllPushBack(list, data) dllPushBack_base(list, &(data))



/** \brief Removes the first node of a list.
 *
 * \param list : List to remove in.
 * \return The same list.
 *
 */
Dll dllPopFront(Dll list);

/** \brief Removes the node after a given Node.
 *
 * \param list : List to remove in.
 * \param node : Node to remove after.
 * \return The same list.
 *
 */
Dll dllPopAfter(Dll list, DllNode node);

/** \brief Removes the node before a given Node.
 *
 * \param list : List to remove in.
 * \param node : Node to remove before.
 * \return The same list.
 *
 */
Dll dllPopBefore(Dll list, DllNode node);

/** \brief Removes the given Node.
 *
 * \param list : List to remove in.
 * \param node : Node to remove.
 * \return The same list.
 *
 */
Dll dllPopNode(Dll list, DllNode node);

/** \brief Removes the last node of a list.
 *
 * \param list : List to remove in.
 * \return The same list.
 *
 */
Dll dllPopBack(Dll list);



/** \brief Details the heap usage of a given list
 *
 * \param list : List to seek in
 * \return void
 *
 */
void dllHeap(Dll list);

#endif
