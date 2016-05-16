/**
 * \file Iterable.h
 * \brief Functions for iterable collections
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#ifndef EXTLIB_ITERABLE_H
#define EXTLIB_ITERABLE_H

#include "Common.h"
#include "Collection.h"
#include "Array.h"
#include "List.h"

/** \brief Returns a new array containing all the elements of the collection. The copy and delete functions are forwarded to the array.
 *
 * \param src : collection.
 * \return an array.
 *
 */
Array toArray(Collection src);

/** \brief Returns a new list containing all the elements of the collection. The copy and delete functions are forwarded to the list.
 *
 * \param src : collection.
 * \return a list.
 *
 */
List toList(Collection src);



/** \brief Adds all the elements of src in dest. Elements must be the same type in src and dest.
 *
 * \param dest : destination collection.
 * \param src : source collection.
 * \return nothing.
 *
 */
void collectionAddAll(Collection dest, Collection src);



/** \brief Adds all the elements from data which is a contiguous array of nbElements elements to dest.
 *
 * \param dest : destination collection.
 * \param data : source array.
 * \param nbElements : number of elements in data.
 * \return nothing.
 *
 */
void collectionAddRaw(Collection dest, Ptr data, int nbElements);

#endif
