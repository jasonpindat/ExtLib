
/**
 * \file extlib.queue.h
 * \brief Primitives functions for queues
 * \author Jason Pindat
 * \version 1.0
 * \date 09/11/2015
 *
 * All the basic functions to manage queues, every function is in constant complexity except queueDel which can be linear if items remains in the queue when destroying it.
 *
 */

#ifndef EXTLIB_QUEUE_H
#define EXTLIB_QUEUE_H

#include "extlib.dll.h"

/** Dll : type for a queue. */
typedef Dll Queue;


#define queueNew(elemSize) dllNew(elemSize)
#define queueDel(queue) dllDel(queue)
#define queueIsEmpty(queue) dllIsEmpty(queue)
#define queueGet(queue, type) dllGetData(dllGetFront(queue), type)
#define queuePush(queue, data) dllPushBack(queue, data)
#define queuePop(queue) dllPopFront(queue)

#define queueHeap(queue) dllHeap(queue)

#endif
