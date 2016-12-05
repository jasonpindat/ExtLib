/**
 * \file List.c
 * \author Jason Pindat
 * \date 2016-11-26
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Common.h"
#include "ExtLib/Collection.h"
#include "ExtLib/List.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _List {
    RealType type;
    ElCmpFct cmpFct;

    int elemSize;
    ElCopyFct copyFct;
    ElDelFct delFct;
    bool needsAllocation;
    Ptr (*ptrTransform)(Ptr);

    int length;

    ListNode first;
    ListNode last;
};

struct _ListNode {
    ListNode next;
    ListNode prev;
};



List listNew(int elemSize) {
    List l = malloc(sizeof(struct _List));

    l->type = LIST;

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

    l->first = NULL;
    l->last = NULL;

    return l;
}

void listDel(List l) {
    listClear(l);

    free(l);
}



void listComparable(List l, ElCmpFct fct) {
    l->cmpFct = fct;
}



List listClone(const List l) {
    List l2 = listNew(l->elemSize);

    l2->cmpFct = l->cmpFct;

    collectionElementInstanciable((Collection)l2, l->copyFct, l->delFct);

    ListNode node = l->first;

    while(node) {
        listAddLast_base(l2, (void *)node+sizeof(struct _ListNode));
        node = node->next;
    }

    return l2;
}



void listClear(List l) {
    ListNode node = l->first;
    ListNode nodeSave;

    while(node) {

        if(l->delFct)
            l->delFct((void *)node+sizeof(struct _ListNode));

        nodeSave = node;
        node = node->next;
        free(nodeSave);
    }

    l->length = 0;
    l->first = NULL;
    l->last = NULL;
}



bool listIsEmpty(const List l) {
    return !l->first;
}

int listLength(const List l) {
    return l->length;
}



bool listContains(const List l, const Ptr data) {
    ListNode node = l->first;

    while(node) {
        if(l->cmpFct((void *)node+sizeof(struct _ListNode), data) == 0)
            return true;

        node = node->next;
    }

    return false;
}



const Ptr listGetFirst_base(const List l) {
    return (void *)l->first+sizeof(struct _ListNode);
}

const Ptr listGetLast_base(const List l) {
    return (void *)l->last+sizeof(struct _ListNode);
}



void listSetFirst_base(List l, const Ptr data) {
    if(l->delFct)
        l->delFct((void *)l->first+sizeof(struct _ListNode));

    if(l->copyFct)
        l->copyFct((void *)l->first+sizeof(struct _ListNode), data);
    else
        memcpy((void *)l->first+sizeof(struct _ListNode), data, l->elemSize);
}

void listSetLast_base(List l, const Ptr data) {
    if(l->delFct)
        l->delFct((void *)l->last+sizeof(struct _ListNode));

    if(l->copyFct)
        l->copyFct((void *)l->last+sizeof(struct _ListNode), data);
    else
        memcpy((void *)l->last+sizeof(struct _ListNode), data, l->elemSize);
}



void listAddFirst_base(List l, const Ptr data) {
    ListNode node = malloc(sizeof(struct _ListNode) + l->elemSize);

    node->next = l->first;
    l->first = node;
    node->prev = NULL;

    if(node->next)
        node->next->prev = node;
    else
        l->last = node;

    if(l->copyFct)
        l->copyFct((void *)node+sizeof(struct _ListNode), data);
    else
        memcpy((void *)node+sizeof(struct _ListNode), data, l->elemSize);

    l->length++;
}

void listAddLast_base(List l, const Ptr data) {
    ListNode node = malloc(sizeof(struct _ListNode) + l->elemSize);

    node->prev = l->last;
    l->last = node;
    node->next = NULL;

    if(node->prev)
        node->prev->next = node;
    else
        l->first = node;

    if(l->copyFct)
        l->copyFct((void *)node+sizeof(struct _ListNode), data);
    else
        memcpy((void *)node+sizeof(struct _ListNode), data, l->elemSize);

    l->length++;
}



void listRemoveFirst(List l) {
    ListNode node = l->first;

    l->first = node->next;

    if(node->next)
        node->next->prev = NULL;
    else
        l->last = NULL;

    if(l->delFct)
        l->delFct((void *)node+sizeof(struct _ListNode));

    free(node);

    l->length--;
}

void listRemoveLast(List l) {
    ListNode node = l->last;

    l->last = node->prev;

    if(node->prev)
        node->prev->next = NULL;
    else
        l->first = NULL;

    if(l->delFct)
        l->delFct((void *)node+sizeof(struct _ListNode));

    free(node);

    l->length--;
}



void listSort(List l, int method) {
    ListNode first=l->first;
    ElCmpFct fct=l->cmpFct;
    int listSize=1, numMerges, leftSize, rightSize;
    ListNode tail, left, right, next;

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
                    else if (method*fct((void *)left+sizeof(struct _ListNode),(void *)right+sizeof(struct _ListNode))<0)     {next=left;left=left->next;leftSize--;}
                    else                            {next=right;right=right->next;rightSize--;}
                    // Update pointers to keep track of where we are:
                    if (tail) tail->next=next;  else first=next;
                    // Sort prev pointer
                    next->prev=tail; // Optional.
                    tail=next;
                }
                // Right is now AFTER the list we just sorted, so start the next sort there.
                left=right;
            }
            // Terminate the list, double the list-sort size.
            tail->next=0,listSize<<=1;
        } while (numMerges>1); // If we only did one merge, then we just sorted the whole list.
        l->first=first;
        l->last=tail; // Optional
    }
}



void listDump(const List l) {
    int elts = l->length;
    int effcost = elts*l->elemSize;
    int opcost = sizeof(struct _List) + elts*sizeof(struct _ListNode);

    printf("Doubly-linked list at %p\n", l);
    printf("\t%d elements, each using %d bytes\n", elts, l->elemSize);
    printf("\t%d bytes used for elements\n", effcost);
    printf("\t%d bytes used as operating cost\n", opcost);
    printf("\t%d bytes total used\n", opcost+effcost);
}



// Iteration

ListIt listItNew(const List l) {
    ListIt it;

    it.list = l;
    it.node = l->first;
    it.onNext = false;

    return it;
}

ListIt listItNewBack(const List l) {
    ListIt it;

    it.list = l;
    it.node = l->last;
    it.onNext = false;

    return it;
}



bool listItExists(const ListIt *it) {
    return it->node != NULL;
}



void listItNext(ListIt *it) {
    if(it->onNext)
        it->onNext = false;
    else
        it->node = it->node->next;
}

void listItPrev(ListIt *it) {
    it->node = it->node->prev;
}



const Ptr listItGet_base(const ListIt *it) {
    return (void *)it->node+sizeof(struct _ListNode);
}



void listItSet_base(ListIt *it, const Ptr data) {
    if(it->list->delFct)
        it->list->delFct((void *)it->node+sizeof(struct _ListNode));

    if(it->list->copyFct)
        it->list->copyFct((void *)it->node+sizeof(struct _ListNode), data);
    else
        memcpy((void *)it->node+sizeof(struct _ListNode), data, it->list->elemSize);
}



void listItAddAfter_base(ListIt *it, const Ptr data) {
    ListNode newnode = malloc(sizeof(struct _ListNode) + it->list->elemSize);

    newnode->next = it->node->next;
    it->node->next = newnode;
    newnode->prev = it->node;

    if(newnode->next)
        newnode->next->prev = newnode;
    else
        it->list->last = newnode;

    if(it->list->copyFct)
        it->list->copyFct((void *)newnode+sizeof(struct _ListNode), data);
    else
        memcpy((void *)newnode+sizeof(struct _ListNode), data, it->list->elemSize);

    it->list->length++;
}

void listItAddBefore_base(ListIt *it, const Ptr data) {
    ListNode newnode = malloc(sizeof(struct _ListNode) + it->list->elemSize);

    newnode->prev = it->node->prev;
    it->node->prev = newnode;
    newnode->next = it->node;

    if(newnode->prev)
        newnode->prev->next = newnode;
    else
        it->list->first = newnode;

    if(it->list->copyFct)
        it->list->copyFct((void *)newnode+sizeof(struct _ListNode), data);
    else
        memcpy((void *)newnode+sizeof(struct _ListNode), data, it->list->elemSize);

    it->list->length++;
}



void listItRemove(ListIt *it) {
    ListNode node = it->node;

    if(node->next)
        node->next->prev = node->prev;
    else
        it->list->last = node->prev;

    if(node->prev)
        node->prev->next = node->next;
    else
        it->list->first = node->next;

    it->node = node->next;

    if(it->list->delFct)
        it->list->delFct((void *)node+sizeof(struct _ListNode));

    free(node);

    it->onNext = true;
}



void listForEach(List l, ElActFct actFct, Ptr infos) {
    ListNode node = l->first;

    while(node) {
        actFct((void *)node+sizeof(struct _ListNode), infos);
        node = node->next;
    }
}
