/**
 * \file Collection.c
 * \author Jason Pindat
 * \date 2016-05-16
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
    ElCopyFct copyFct;
    ElDelFct delFct;
};

RealType collectionGetType(Collection c) {
    return c->type;
}

bool collectionInstanceOf(Collection c, RealType r) {
    return c->type == r;
}

int collectionGetElemSize(Collection c) {
    return c->elemSize;
}

void collectionElementInstanciable(Collection c, ElCopyFct copyFct, ElDelFct delFct) {
    c->copyFct = copyFct;
    c->delFct = delFct;
}

ElCopyFct collectionGetCopyFunction(Collection c) {
    return c->copyFct;
}

ElDelFct collectionGetDelFunction(Collection c) {
    return c->delFct;
}
