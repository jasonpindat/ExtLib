/**
 * \file Iterable.h
 * \brief Interface for iterable elements
 * \author Jason Pindat
 * \version 1.0
 * \date 2016-05-15
 *
 */

#ifndef EXTLIB_ITERABLE_H
#define EXTLIB_ITERABLE_H

#include "Common.h"

struct _Iterable {
    RealType type;
};

typedef struct _Iterable *Iterable;

#endif
