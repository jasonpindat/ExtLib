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
 * \param c : collection.
 * \return an array.
 *
 */
Array toArray(Collection c);

/** \brief Returns a new list containing all the elements of the collection. The copy and delete functions are forwarded to the list.
 *
 * \param c : collection.
 * \return a list.
 *
 */
List toList(Collection c);

#endif
