/**
 * \file Collection.h
 * \brief Interface for collections
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 */

#ifndef EXTLIB_COLLECTION_H
#define EXTLIB_COLLECTION_H

#include "Common.h"

/** Collection : type for an collection. */
typedef struct _Collection *Collection;



/** \brief Returns the real type of a collection.
 *
 * \param c : collection.
 * \return the type of the collection.
 *
 */
RealType collectionGetType(Collection c);

/** \brief Determines whether a collection is instance of a type or not.
 *
 * \param c : collection.
 * \param r : type.
 * \return true if the type of c is r, false otherwise.
 *
 */
bool collectionInstanceOf(Collection c, RealType r);



/** \brief Returns the size of an element in the collection.
 *
 * \param c : collection.
 * \return size of an element in c.
 *
 */
int collectionGetElemSize(Collection c);



/** \brief Sets the functions to copy an element and to delete an element of this collection. If NULL or collectionElementCopy is not called, the elements will be copied bit by bit.
 *
 * \param c : Collection in which you set the fonction.
 * \param copyFct : pointer to the copy function, the function must take 2 pointers, the first is the source element and the second is the new allocated element to initialize and return nothing.
 * \param delFct : pointer to the deletion function, the function must take a pointer to the element to destroy. Note that this pointer will be automatically freed, so delFct must'nt do this.
 * \return nothing.
 *
 */
void collectionElementInstanciable(Collection c, ElCopyFct copyFct, ElDelFct delFct);

/** \brief Returns the function to copy an element in the collection (NULL if not defined).
 *
 * \param c : collection.
 * \return a copy function or NULL.
 *
 */
ElCopyFct collectionGetCopyFunction(Collection c);

/** \brief Returns the function to delete an element in the collection (NULL if not defined).
 *
 * \param c : collection.
 * \return a deletion function or NULL.
 *
 */
ElDelFct collectionGetDelFunction(Collection c);

#endif
