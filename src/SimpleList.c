/**
 * \file SimpleList.c
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Common.h"
#include "ExtLib/Collection.h"
#include "ExtLib/SimpleList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _SimpleList {
    RealType type;
    int elemSize;
    ElCmpFct cmpFct;
    ElCopyFct copyFct;
    ElDelFct delFct;
    bool needsAllocation;
    Ptr (*ptrTransform)(Ptr);

    int length;

    SimpleListNode first;
};

struct _SimpleListNode {
    SimpleListNode next;
};



SimpleList simpleListNew(int elemSize) {
    SimpleList l = malloc(sizeof(struct _SimpleList));

    l->type = SIMPLELIST;

    if(elemSize<=0) {
        l->elemSize=_elSizeFct(elemSize);
        l->cmpFct=_elCompareFct(elemSize);
    }
    else {
        l->elemSize=elemSize;
        l->cmpFct=NULL;
    }

    collectionElementInstanciable((Collection)l, NULL, NULL);

    l->length = 0;

    l->first=NULL;

    return l;
}

void simpleListDel(SimpleList l) {
    simpleListClear(l);

    free(l);
}



void simpleListComparable(SimpleList l, ElCmpFct fct) {
    l->cmpFct = fct;
}



SimpleList simpleListClone(const SimpleList l) {
    SimpleList l2 = simpleListNew(l->elemSize);

    l2->cmpFct = l->cmpFct;

    collectionElementInstanciable((Collection)l2, l->copyFct, l->delFct);

    SimpleListNode node = l->first;
    bool first = true;
    SimpleListIt it;

    while(node) {
        if(first) {
            simpleListAddFirst_base(l2, node+sizeof(struct _SimpleListNode));
            it = simpleListItNew(l2);
            first = false;
        }
        else {
            simpleListItAddAfter_base(&it, node+sizeof(struct _SimpleListNode));
            simpleListItNext(&it);
        }

        node = node->next;
    }

    return l2;
}



void simpleListClear(SimpleList l) {
    SimpleListNode node = l->first;
    SimpleListNode nodeSave;

    while(node) {

        if(l->delFct)
            l->delFct(node+sizeof(struct _SimpleListNode));

        nodeSave = node;
        node = node->next;
        free(nodeSave);
    }

    l->length = 0;
    l->first = NULL;
}



bool simpleListIsEmpty(const SimpleList l) {
    return !l->first;
}

int simpleListLength(const SimpleList l) {
    return l->length;
}



bool simpleListContains(const SimpleList l, const Ptr data) {
    SimpleListNode node = l->first;

    while(node) {
        if(l->cmpFct(node+sizeof(struct _SimpleListNode), data) == 0)
            return true;

        node = node->next;
    }

    return false;
}



const Ptr simpleListGetFirst_base(const SimpleList l) {
    return l->first+sizeof(struct _SimpleListNode);
}



void simpleListSetFirst_base(SimpleList l, const Ptr data) {
    if(l->delFct)
        l->delFct(l->first+sizeof(struct _SimpleListNode));

    if(l->copyFct)
        l->copyFct(l->first+sizeof(struct _SimpleListNode), data);
    else
        memcpy(l->first+sizeof(struct _SimpleListNode), data, l->elemSize);
}



void simpleListAddFirst_base(SimpleList l, const Ptr data) {
    SimpleListNode node = malloc(sizeof(struct _SimpleListNode) + l->elemSize);

    node->next = l->first;
    l->first = node;

    if(l->copyFct)
        l->copyFct(node+sizeof(struct _SimpleListNode), data);
    else
        memcpy(node+sizeof(struct _SimpleListNode), data, l->elemSize);

    l->length++;
}



void simpleListRemoveFirst(SimpleList l) {
    SimpleListNode node = l->first;

    l->first = node->next;

    if(l->delFct)
        l->delFct(node+sizeof(struct _SimpleListNode));

    free(node);

    l->length--;
}



void simpleListSort(SimpleList l, int method) {
    SimpleListNode first=l->first;
    ElCmpFct fct=l->cmpFct;
    int listSize=1, numMerges, leftSize, rightSize;
    SimpleListNode tail, left, right, next;

    if (first && first->next) {
        do { // For each power of two<=list length
            numMerges=0,left=first;
            tail=first=0; // Start at the start

            while (left) { // Do this list_len/listSize times:
                numMerges++,right=left,leftSize=0,rightSize=listSize;
                // Cut list into two halves (but don't overrun)
                while (right && leftSize<listSize) leftSize++,right=right->next;
                // Run through the lists appending onto what we have so far.
                while (leftSize>0 || (rightSize>0 && right)) {
                    // Left empty, take right OR Right empty, take left, OR compare.
                    if (!leftSize)                  {next=right;right=right->next;rightSize--;}
                    else if (!rightSize || !right)  {next=left;left=left->next;leftSize--;}
                    else if (method*fct(left+sizeof(struct _SimpleListNode),right+sizeof(struct _SimpleListNode))<0)     {next=left;left=left->next;leftSize--;}
                    else                            {next=right;right=right->next;rightSize--;}
                    // Update pointers to keep track of where we are:
                    if (tail) tail->next=next;  else first=next;
                    // Sort prev pointer
                    //next->prev=tail; // Optional.
                    tail=next;
                }
                // Right is now AFTER the list we just sorted, so start the next sort there.
                left=right;
            }
            // Terminate the list, double the list-sort size.
            tail->next=0,listSize<<=1;
        } while (numMerges>1); // If we only did one merge, then we just sorted the whole list.
        l->first=first;
        //l->last=tail; // Optional
    }
}



void simpleListDump(const SimpleList l) {
    int elts = l->length;
    int effcost = elts*l->elemSize;
    int opcost = sizeof(struct _SimpleList) + elts*sizeof(struct _SimpleListNode);

    printf("Singly-linked list at %p\n", l);
    printf("\t%d elements, each using %d bytes\n", elts, l->elemSize);
    printf("\t%d bytes used for elements\n", effcost);
    printf("\t%d bytes used as operating cost\n", opcost);
    printf("\t%d bytes total used\n", opcost+effcost);
}



// Iteration

SimpleListIt simpleListItNew(const SimpleList l) {
    SimpleListIt it;

    it.list = l;
    it.prevNode = NULL;
    it.node = l->first;

    return it;
}



bool simpleListItExists(const SimpleListIt *it) {
    return it->node != NULL;
}



void simpleListItNext(SimpleListIt *it) {
    it->prevNode = it->node;
    it->node = it->node->next;
}



const Ptr simpleListItGet_base(const SimpleListIt *it) {
    return it->node+sizeof(struct _SimpleListNode);
}



void simpleListItSet_base(SimpleListIt *it, const Ptr data) {
    if(it->list->delFct)
        it->list->delFct(it->node+sizeof(struct _SimpleListNode));

    if(it->list->copyFct)
        it->list->copyFct(it->node+sizeof(struct _SimpleListNode), data);
    else
        memcpy(it->node+sizeof(struct _SimpleListNode), data, it->list->elemSize);
}



void simpleListItAddAfter_base(SimpleListIt *it, const Ptr data) {
    SimpleListNode newnode = malloc(sizeof(struct _SimpleListNode) + it->list->elemSize);

    newnode->next = it->node->next;
    it->node->next = newnode;

    if(it->list->copyFct)
        it->list->copyFct(newnode+sizeof(struct _SimpleListNode), data);
    else
        memcpy(newnode+sizeof(struct _SimpleListNode), data, it->list->elemSize);

    it->list->length++;
}



void simpleListItRemove(SimpleListIt *it) {
    SimpleListNode node = it->node;

    if(it->prevNode)
        it->prevNode->next = node->next;
    else
        it->list->first = node->next;

    it->node = node->next;

    if(it->list->delFct)
        it->list->delFct(node+sizeof(struct _SimpleListNode));

    free(node);
}



void simpleListForEach(SimpleList l, ElActFct actFct, Ptr infos) {
    SimpleListNode node = l->first;

    while(node) {
        actFct(node+sizeof(struct _SimpleListNode), infos);
        node = node->next;
    }
}
