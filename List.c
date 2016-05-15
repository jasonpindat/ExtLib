#include "List.h"

void listDel(List l) {
    switch(l->type) {
    case ARRAY:
        arrayDel((Array)l);
        break;
    }
}

List listClone(List l) {
    switch(l->type) {
    case ARRAY:
        return (Array)arrayClone((Array)l);
        break;
    }
}

void listClear(List l) {
    switch(l->type) {
    case ARRAY:
        arrayClear((Array)l);
        break;
    }
}

bool listIsEmpty(List l) {
    switch(l->type) {
    case ARRAY:
        return arrayIsEmpty((Array)l);
        break;
    }
}

bool listContains(List l, Ptr data) {
    switch(l->type) {
    case ARRAY:
        return arrayContains((Array)l, data);
        break;
    }
}
