/**
 * \file extlib.vect.h
 * \brief Primitives functions for stacks
 * \author Jason Pindat
 * \version 1.0
 * \date 09/11/2015
 *
 * All the basic functions to manage dynamic stacks.
 *
 */

#ifndef EXTLIB_STACK_H
#define EXTLIB_STACK_H

#include "extlib.vect.h"

/** Stack : type for a stack. */
typedef Vect Stack;


#define stackNew(elemSize) vectNew(elemSize)
#define stackDel(stack) vectDel(stack)
#define stackCopy(stack) vectCopy(stack)
#define stackClear(stack) vectClear(stack)
#define stackIsEmpty(stack) vectIsEmpty(stack)
#define stackGet(stack, type) vectGetBack(stack, type)
#define stackPush(stack, data) vectPushBack(stack, data)
#define stackPop(stack) vectPopBack(stack)

#define stackHeap(stack) vectHeap(stack)

#endif

