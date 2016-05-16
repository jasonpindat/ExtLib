/**
 * \file Iterable.c
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Iterable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void(*ForEachFct)(Collection, ElActFct, Ptr);

static ForEachFct getForEachFct(Collection c) {
    switch(collectionGetType(c)) {
    case ARRAY:
        return (ForEachFct)arrayForEach;
    case LIST:
        return (ForEachFct)listForEach;
    default:
        return NULL;
    }
}



static void toArrayAddElt(Ptr obj, Array a) {
    arrayPush_base(a, obj);
}

static void toListAddElt(Ptr obj, List l) {
    listAddLast_base(l, obj);
}

static ElActFct getAddFct(Collection c) {
    switch(collectionGetType(c)) {
    case ARRAY:
        return (ElActFct)toArrayAddElt;
    case LIST:
        return (ElActFct)toListAddElt;
    default:
        return NULL;
    }
}



Array toArray(Collection src) {
    Array a = arrayNew(collectionGetElemSize(src));
    collectionElementInstanciable((Collection)a, collectionGetCopyFunction(src), collectionGetDelFunction(src));

    collectionAddAll((Collection)a, src);

    return a;
}

List toList(Collection src) {
    List l = listNew(collectionGetElemSize(src));
    collectionElementInstanciable((Collection)l, collectionGetCopyFunction(src), collectionGetDelFunction(src));

    collectionAddAll((Collection)l, src);

    return l;
}



void collectionAddAll(Collection dest, Collection src) {
    ForEachFct forEachFct = getForEachFct(src);
    ElActFct addFct = getAddFct(dest);
    forEachFct(src, addFct, dest);
}



void collectionAddRaw(Collection dest, Ptr data, int nbElements) {
    ElActFct addFct = getAddFct(dest);
    int elemSize = collectionGetElemSize(dest);

    for(int i=0; i<nbElements; i++)
        addFct(data + i*elemSize, dest);
}
