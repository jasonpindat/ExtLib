/**
 * \file Iterable.c
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Iterable.h"
#include "ExtLib/Common.h"
#include "ExtLib/Collection.h"
#include "ExtLib/Array.h"
#include "ExtLib/List.h"
#include "ExtLib/String.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void(*ForEachFct)(Collection, ElActFct, Ptr);

static ForEachFct getForEachFct(Collection c) {
    switch(collectionGetType(c)) {
    case ARRAY:
        return (ForEachFct)arrayForEach;
    case SIMPLELIST:
        return (ForEachFct)simpleListForEach;
    case LIST:
        return (ForEachFct)listForEach;
    case STRING:
        return (ForEachFct)stringForEach;
    default:
        return NULL;
    }
}

typedef struct {
    SimpleList list;
    SimpleListIt it;
} SimpleListAddData;

static void toArrayAddElt(Ptr obj, Array a) {
    arrayPush_base(a, obj);
}

static void toSimpleListAddElt(Ptr obj, SimpleListAddData *data) {
    if(simpleListGetFirst_base(data->list)) {
        simpleListAddFirst_base(data->list, obj);
        data->it = simpleListItNew(data->list);
    }
    else {
        simpleListItAddAfter_base(&(data->it), obj);
        simpleListItNext(&(data->it));
    }
}

static void toListAddElt(Ptr obj, List l) {
    listAddLast_base(l, obj);
}

static void toHeapAddElt(Ptr obj, Heap h) {
    heapPush_base(h, obj);
}

static ElActFct getAddFct(Collection c) {
    switch(collectionGetType(c)) {
    case ARRAY:
        return (ElActFct)toArrayAddElt;
    case SIMPLELIST:
        return (ElActFct)toSimpleListAddElt;
    case LIST:
        return (ElActFct)toListAddElt;
    case HEAP:
        return (ElActFct)toHeapAddElt;
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

SimpleList toSimpleList(Collection src) {
    SimpleList l = simpleListNew(collectionGetElemSize(src));
    collectionElementInstanciable((Collection)l, collectionGetCopyFunction(src), collectionGetDelFunction(src));

    collectionAddAll((Collection)l, src);

    return l;
}

List toList(Collection src) {
    List l = listNew(collectionGetElemSize(src));
    collectionElementInstanciable((Collection)l, collectionGetCopyFunction(src), collectionGetDelFunction(src));

    collectionAddAll((Collection)l, src);

    return l;
}

Heap toHeap(Collection src, ElCmpFct cmpFct) {
    Heap h = heapNew(collectionGetElemSize(src), cmpFct);
    collectionElementInstanciable((Collection)h, collectionGetCopyFunction(src), collectionGetDelFunction(src));

    collectionAddAll((Collection)h, src);

    return h;
}



void collectionAddAll(Collection dest, Collection src) {
    ForEachFct forEachFct = getForEachFct(src);
    ElActFct addFct = getAddFct(dest);

    SimpleListAddData simpleListAddData;
    Ptr data;

    if(collectionInstanceOf(dest, SIMPLELIST)) {
        simpleListAddData.list = (SimpleList)dest;
        data = &simpleListAddData;
    }
    else
        data = dest;

    forEachFct(src, addFct, data);
}



void collectionAddRaw(Collection dest, Ptr rawData, int nbElements) {
    ElActFct addFct = getAddFct(dest);
    int elemSize = collectionGetElemSize(dest);

    SimpleListAddData simpleListAddData;
    Ptr data;

    if(collectionInstanceOf(dest, SIMPLELIST)) {
        simpleListAddData.list = (SimpleList)dest;
        data = &simpleListAddData;
    }
    else
        data = dest;

    for(int i=0; i<nbElements; i++)
        addFct(rawData + i*elemSize, data);
}
