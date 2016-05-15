/**
 * \file Iterator.h
 * \brief Management of iterators
 * \author Jason Pindat
 * \version 1.0
 * \date 2016-05-15
 *
 * All the basic functions to manage iterators.
 *
 */

#ifndef EXTLIB_ITERATOR_H
#define EXTLIB_ITERATOR_H

#include "Common.h"
#include "Iterable.h"

struct _Iterator;

typedef bool(*ItExistsFct)(struct _Iterator *it);
typedef void(*ItNextFct)(struct _Iterator *it);
typedef Ptr(*ItGetFct)(struct _Iterator *it);

typedef struct _Iterator {
    //RealType type;

    Iterable col;

    union {
        int arrayData;
    } data;

    ItExistsFct existsFct;
    ItNextFct nextFct;
    ItGetFct getFct;

} Iterator;


Iterator itNew(Iterable col);
bool itExists(Iterator *it);
void itNext(Iterator *it);
Ptr itGet_base(Iterator *it);
#define itGet(it, type) (*(type*)itGet_base(it))

#endif
