/**
 * \file Iterable.c
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#include "Iterable.h"

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

Array toArray(Collection c) {
    ForEachFct forEachFct = getForEachFct(c);

    Array a = arrayNew(collectionGetElemSize(c));

    collectionElementInstanciable((Collection)a, collectionGetCopyFunction(c), collectionGetDelFunction(c));

    forEachFct(c, (ElActFct)toArrayAddElt, a);

    return a;
}



static void toListAddElt(Ptr obj, List l) {
    listAddLast_base(l, obj);
}

List toList(Collection c) {
    ForEachFct forEachFct = getForEachFct(c);

    List l = listNew(collectionGetElemSize(c));

    collectionElementInstanciable((Collection)l, collectionGetCopyFunction(c), collectionGetDelFunction(c));

    forEachFct(c, (ElActFct)toListAddElt, l);

    return l;
}
