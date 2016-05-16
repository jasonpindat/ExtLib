/**
 * \file List.c
 * \author Jason Pindat
 * \date 2016-05-16
 *
 * Copyright 2014-2016
 *
 */

#include "Common.h"
#include "List.h"

struct _List {
    RealType type;
    int elemSize;
    ElCopyFct copyFct;
    ElDelFct delFct;

    ElCmpFct cmpFct;
    Ptr temp;
    int length;

    ListNode first;
    ListNode last;
};

struct _ListNode {
    Ptr data;
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

    l->copyFct = NULL;
    l->delFct = NULL;

    l->temp = malloc(l->elemSize);
    l->length = 0;

    l->first = NULL;
    l->last = NULL;

    return l;
}

void listDel(List l) {
    listClear(l);
    free(l->temp);
    free(l);
}



void listComparable(List l, ElCmpFct fct) {
    l->cmpFct = fct;
}



List listClone(List l) {
    List l2 = listNew(l->elemSize);

    l2->cmpFct = l->cmpFct;
    l2->copyFct = l->copyFct;
    l2->delFct = l->delFct;

    for(ListIt it=listItNew(l); listItExists(&it); listItNext(&it)) {
        Ptr data = listItGet_base(&it);
        listAddLast_base(l2, data);
        if(l->delFct)
            l->delFct(data);
    }

    return l2;
}



void listClear(List l) {
    ListNode node = l->first;
    ListNode nodeSave;

    while(node) {
        if(l->delFct)
            l->delFct(node->data);

        nodeSave = node;
        node = node->next;
        free(nodeSave);
    }

    l->length = 0;
    l->first = NULL;
    l->last = NULL;
}



bool listIsEmpty(List l) {
    return !l->first;
}

int listLength(List l) {
    return l->length;
}



bool listContains(List l, Ptr data) {
    ListNode node = l->first;

    while(node) {
        if(l->cmpFct(node->data, data) == 0)
            return true;

        node = node->next;
    }

    return false;
}



Ptr listGetFirst_base(List l) {
    if(l->copyFct) {
        l->copyFct(l->first->data, l->temp);
        return l->temp;
    }
    else
        return l->first->data;
}

Ptr listGetLast_base(List l) {
    if(l->copyFct) {
        l->copyFct(l->last->data, l->temp);
        return l->temp;
    }
    else
        return l->last->data;
}



void listSetFirst_base(List l, Ptr data) {
    if(l->delFct)
        l->delFct(l->first->data);

    if(l->copyFct)
        l->copyFct(data, l->first->data);
    else
        memcpy(l->first->data, data, l->elemSize);
}

void listSetLast_base(List l, Ptr data) {
    if(l->delFct)
        l->delFct(l->last->data);

    if(l->copyFct)
        l->copyFct(data, l->last->data);
    else
        memcpy(l->last->data, data, l->elemSize);
}



void listAddFirst_base(List l, Ptr data) {
    void *memory = malloc(sizeof(struct _ListNode) + l->elemSize);

    ListNode node = memory;

    node->next = l->first;
    l->first = node;
    node->prev = NULL;

    if(node->next)
        node->next->prev = node;
    else
        l->last = node;

    node->data = memory + sizeof(struct _ListNode);

    if(l->copyFct)
        l->copyFct(data, node->data);
    else
        memcpy(node->data, data, l->elemSize);

    l->length++;
}

void listAddLast_base(List l, Ptr data) {
    void *memory = malloc(sizeof(struct _ListNode) + l->elemSize);

    ListNode node = memory;

    node->prev = l->last;
    l->last = node;
    node->next = NULL;

    if(node->prev)
        node->prev->next = node;
    else
        l->first = node;

    node->data = memory + sizeof(struct _ListNode);

    if(l->copyFct)
        l->copyFct(data, node->data);
    else
        memcpy(node->data, data, l->elemSize);

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
        l->delFct(node->data);

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
        l->delFct(node->data);

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
                    else if (method*fct(left->data,right->data)<0)     {next=left;left=left->next;leftSize--;}
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
        l->last=tail;
    }
}



void listHeap(List l) {
    int elts = l->length;
    int effcost = elts*l->elemSize;
    int opcost = sizeof(struct _List)+elts * sizeof(struct _ListNode);

    printf("Doubly-linked list at %p\n", l);
    printf("\t%d elements, each using %d bytes\n", elts, l->elemSize);
    printf("\t%d bytes used for elemets\n", effcost);
    printf("\t%d bytes used as operating cost\n", opcost);
    printf("\t%d bytes total used\n", opcost+effcost);
}



// Iteration

ListIt listItNew(List l) {
    ListIt it;

    it.list = l;
    it.node = l->first;

    return it;
}

ListIt listItNewBack(List l) {
    ListIt it;

    it.list = l;
    it.node = l->last;

    return it;
}



bool listItExists(ListIt *it) {
    return it->node != NULL;
}



void listItNext(ListIt *it) {
    it->node = it->node->next;
}

void listItPrev(ListIt *it) {
    it->node = it->node->prev;
}



Ptr listItGet_base(ListIt *it) {
    if(it->list->copyFct) {
        it->list->copyFct(it->node->data, it->list->temp);
        return it->list->temp;
    }
    else
        return it->node->data;
}



void listItSet_base(ListIt *it, Ptr data) {
    if(it->list->delFct)
        it->list->delFct(it->node->data);

    if(it->list->copyFct)
        it->list->copyFct(data, it->node->data);
    else
        memcpy(it->node->data, data, it->list->elemSize);
}



void listItAddAfter_base(ListIt *it, Ptr data) {
    void *memory = malloc(sizeof(struct _ListNode) + it->list->elemSize);

    ListNode newnode = memory;

    newnode->next = it->node->next;
    it->node->next = newnode;
    newnode->prev = it->node;

    if(newnode->next)
        newnode->next->prev = newnode;
    else
        it->list->last = newnode;

    newnode->data = memory + sizeof(struct _ListNode);

    if(it->list->copyFct)
        it->list->copyFct(data, newnode->data);
    else
        memcpy(newnode->data, data, it->list->elemSize);

    it->list->length++;
}

void listItAddBefore_base(ListIt *it, Ptr data) {
    void *memory = malloc(sizeof(struct _ListNode) + it->list->elemSize);

    ListNode newnode = memory;

    newnode->prev = it->node->prev;
    it->node->prev = newnode;
    newnode->next = it->node;

    if(newnode->prev)
        newnode->prev->next = newnode;
    else
        it->list->first = newnode;

    newnode->data = memory + sizeof(struct _ListNode);

    if(it->list->copyFct)
        it->list->copyFct(data, newnode->data);
    else
        memcpy(newnode->data, data, it->list->elemSize);

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
        it->list->delFct(node->data);

    free(node);
}



void listForEach(List l, ElActFct actFct, Ptr infos) {
    ListNode node = l->first;

    while(node) {
        actFct(node->data, infos);
        node = node->next;
    }
}