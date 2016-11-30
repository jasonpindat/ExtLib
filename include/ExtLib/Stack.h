/**
 * \file Stack.h
 * \brief Primitives functions for stacks
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * All the basic functions to manage stacks.
 * Stack is a Collection but is not Iterable
 *
 * Copyright 2014-2016
 *
 */

#ifndef EXTLIB_STACK_H
#define EXTLIB_STACK_H

#include "Array.h"

/** Stack : type for a stack. */
typedef Array Stack;


#define stackNew(elemSize)                  arrayNew(elemSize)
#define stackDel(s)                         arrayDel(s)
#define stackMultithread(s, multithread)    arrayMultithread(s, multithread)
#define stackClone(s)                       arrayClone(s)

#define stackClear(s)                       arrayClear(s)
#define stackIsEmpty(s)                     arrayIsEmpty(s)

#define stackTop(s, type)                   arrayGet(s, arrayLength(s)-1, type)
#define stackPush(s, data)                  arrayPush(s, data)
#define stackPop(s)                         arrayPop(s)

#define stackDump(s)                        arrayDump(s)

#endif

