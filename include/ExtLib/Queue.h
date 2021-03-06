/**
 * \file Queue.h
 * \brief Primitives functions for queues
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * All the basic functions to manage queues.
 * Queue is a Collection but is not Iterable
 *
 * Copyright 2014-2016
 *
 */

#ifndef EXTLIB_QUEUE_H
#define EXTLIB_QUEUE_H

#include "List.h"

/** Queue : type for a queue. */
typedef List Queue;


#define queueNew(elemSize)                  listNew(elemSize)
#define queueDel(q)                         listDel(q)
#define queueClone(q)                       listClone(q)

#define queueClear(q)                       listClear(q)
#define queueIsEmpty(q)                     listIsEmpty(q)

#define queueGet(q, type)                   listGetFirst(q, type)
#define queueAdd(q, data)                   listAddLast(q, data)
#define queueAddI(q, data, type)            listAddLastI(q, data, type)
#define queueRemove(q)                      listRemoveFirst(q)

#define queueDump(q)                        listDump(q)

#endif
