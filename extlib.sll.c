#include "extlib.def.h"
#include "extlib.sll.h"
#include "extlib.sll.more.h"

struct _Sll {
    ElCmpFct cmpFct;
    SllNode first;
    int elemSize;
};

struct _SllNode {
    Ptr data;
    SllNode next;
};



Sll sllNew(int elemSize) {
    Sll list = malloc(sizeof(struct _Sll));

    if(elemSize<=0) {
        list->elemSize=_elSizeFct(elemSize);
        list->cmpFct=_elCompareFct(elemSize);
    }
    else {
        list->elemSize=elemSize;
        list->cmpFct=NULL;
    }

    list->first=NULL;
    return list;
}

void sllDel(Sll list) {
    sllClear(list);
    free(list);
}



bool sllIsEmpty(Sll list) {
    return !list->first;
}

Sll sllClear(Sll list) {
    SllNode node = list->first;
    SllNode noden;

    while(node) {
        noden = node;
        node=node->next;
        free(noden->data);
        free(noden);
    }
    list->first=NULL;
    return list;
}

int sllCount(Sll list) {
    int size=0;
    SllNode node = list->first;

    while(node) {
        size++;
        node = node->next;
    }
    return size;
}

Ptr sllGetData_base(SllNode node) {
    return node->data;
}



SllNode sllGetFront(Sll list) {
    return list->first;
}

SllNode sllGetNext(SllNode node) {
    return node->next;
}

SllNode sllGetBack(Sll list) {
    SllNode node = list->first;

    if(!node)
        return NULL;

    while(node->next)
        node=node->next;

    return node;
}

SllNode sllGetAt(Sll list, int pos) {
    SllNode node = list->first;

    if(!node)
        return NULL;

    for(int i=0; i<pos; i++) {
        node=node->next;
        if(!node)
            return NULL;
    }

    return node;
}



SllNode sllSetFront_base(Sll list, Ptr data) {
    SllNode node = list->first;

    memcpy(node->data, data, list->elemSize);

    return node;
}

SllNode sllSetNode_base(Sll list, SllNode node, Ptr data) {
    memcpy(node->data, data, list->elemSize);

    return node;
}

SllNode sllSetBack_base(Sll list, Ptr data) {
    SllNode node = sllGetBack(list);

    sllSetNode(list, node, data);

    return node;
}

SllNode sllSetAt_base(Sll list, int pos, Ptr data) {
    SllNode node = sllGetAt(list, pos);

    sllSetNode(list, node, data);

    return node;
}



SllNode sllPushFront_base(Sll list, Ptr data) {
    SllNode newnode = malloc(sizeof(struct _SllNode));

    newnode->next=list->first;
    list->first=newnode;

    newnode->data=malloc(list->elemSize);
    memcpy(newnode->data, data, list->elemSize);

    //newnode->data=data;

    return newnode;
}

SllNode sllPushAfter_base(Sll list, SllNode node, Ptr data) {
    SllNode newnode = malloc(sizeof(struct _SllNode));

    newnode->next=node->next;
    node->next=newnode;

    newnode->data=malloc(list->elemSize);
    memcpy(newnode->data, data, list->elemSize);

    //newnode->data=data;

    return newnode;
}

SllNode sllPushBack_base(Sll list, Ptr data) {
    if(!list->first)
        return sllPushFront(list, data);
    else
        return sllPushAfter(list, sllGetBack(list), data);
}

SllNode sllPushAt_base(Sll list, int pos, Ptr data) {
    if(pos==0)
        return sllPushFront(list, data);

    //SllNode node = sllGetAt(list, pos-1);

    /*if(!node)
        throwExc("ExtLib.Sll.sllAddAt", "Given position is out of bounds.");*/

    return sllPushAfter(list, sllGetAt(list, pos-1), data);
}



Sll sllPopFront(Sll list) {
    SllNode oldnode = list->first;

    list->first=oldnode->next;

    free(oldnode->data);
    free(oldnode);

    return list;
}

SllNode sllPopAfter(SllNode node) {
    SllNode oldnode = node->next;

    node->next=oldnode->next;

    free(oldnode->data);
    free(oldnode);

    return node;
}

Sll sllPopBack(Sll list) {
    if(!list->first->next)
        sllPopFront(list);
    else {
        SllNode node = list->first->next;
        SllNode nodeb = list->first;

        while(node->next) {
            nodeb=node;
            node=node->next;
        }

        sllPopAfter(nodeb);
    }

    return list;
}

Sll sllPopAt(Sll list, int pos) {
    if(pos==0)
        return sllPopFront(list);

    sllPopAfter(sllGetAt(list, pos-1));

    return list;
}



Sll sllComparable(Sll list, ElCmpFct fct) {
    list->cmpFct=fct;
    return list;
}

Sll sllSort(Sll list, int method) {
    SllNode first=list->first;
    ElCmpFct fct=list->cmpFct;
    int listSize=1, numMerges, leftSize, rightSize;
    SllNode tail, left, right, next;
    if (!first || !first->next) return list;  // Trivial case

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
                //next->prev=tail; // Optional.
                tail=next;
            }
            // Right is now AFTER the list we just sorted, so start the next sort there.
            left=right;
        }
        // Terminate the list, double the list-sort size.
        tail->next=0,listSize<<=1;
    } while (numMerges>1); // If we only did one merge, then we just sorted the whole list.
    list->first=first;
    return list;
}



void sllHeap(Sll list) {
    int elts=sllCount(list);
    int effcost=elts*list->elemSize;
    int opcost=sizeof(struct _Sll)+elts*sizeof(struct _SllNode);

    printf("Singly-linked list at %p\n", list);
    printf("\t%d elements, each using %d bytes\n", elts, list->elemSize);
    printf("\t%d bytes used for elemets\n", effcost);
    printf("\t%d bytes used as operating cost\n", opcost);
    printf("\t%d bytes total used\n", opcost+effcost);
}
