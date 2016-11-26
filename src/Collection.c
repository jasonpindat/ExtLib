/**
 * \file Collection.c
 * \author Jason Pindat
 * \date 2016-11-26
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _Collection {
    RealType type;
    int elemSize;
    ElCmpFct cmpFct;
    ElCopyFct copyFct;
    ElDelFct delFct;
    bool needsAllocation;
    Ptr (*ptrTransform)(Ptr);
};



static Ptr noAllocationTransform(Ptr obj) {
    return obj;
}

static Ptr allocationTransform(Ptr obj) {
    return *((Ptr *)obj);
}

static inline bool objNeedsAllocation(Collection c) {
    return c->elemSize > sizeof(Ptr) || c->copyFct || c->delFct;
}

static inline void collectionUpdAllocationPolicy(Collection c) {
    c->needsAllocation = objNeedsAllocation(c);
    c->ptrTransform = c->needsAllocation ? allocationTransform : noAllocationTransform;
}



RealType collectionGetType(Collection c) {
    return c->type;
}

bool collectionInstanceOf(Collection c, RealType r) {
    return c->type == r;
}



int collectionGetElemSize(Collection c) {
    if(c->type == STRING)
        return sizeof(char);

    return c->elemSize;
}



ElCmpFct collectionGetCmpFct(Collection c) {
    return c->cmpFct;
}



void collectionElementInstanciable(Collection c, ElCopyFct copyFct, ElDelFct delFct) {
    c->copyFct = copyFct;
    c->delFct = delFct;
    collectionUpdAllocationPolicy(c);
}

ElCopyFct collectionGetCopyFunction(Collection c) {
    if(c->type == STRING)
        return NULL;

    return c->copyFct;
}

ElDelFct collectionGetDelFunction(Collection c) {
    if(c->type == STRING)
        return NULL;

    return c->delFct;
}
