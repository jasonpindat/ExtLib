#include "Iterator.h"
#include "Array.h"
#include <stdio.h>

Iterator itNew(Iterable col) {
    Iterator it;

    it.col = col;

    switch(col->type) {
    case ARRAY:
        it.existsFct = arrayItExists;
        it.nextFct = arrayItNext;
        it.getFct = arrayItGet;
        it.data.arrayData = 0;
        break;
    default:
        fprintf(stderr, "Unable to create iterator : Collection is not iterable.");
        exit(1);
    }

    return it;
}

bool itExists(Iterator *it) {
    return it->existsFct(it);
}

void itNext(Iterator *it) {
    return it->nextFct(it);
}

Ptr itGet_base(Iterator *it) {
    return it->getFct(it);
}
