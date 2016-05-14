#include "extlib.def.h"
#include "extlib.stack.h"
#include "extlib.stack.more.h"

struct _Stack {
    StackNode first;
    int elemSize;
};

struct _StackNode {
    Ptr data;
    StackNode next;
};



Stack stackNew(int elemSize) {
    Stack stack = malloc(sizeof(struct _Stack));

    if(elemSize<=0)
        stack->elemSize=_elSizeFct(elemSize);
    else
        stack->elemSize=elemSize;

    stack->first=NULL;
    return stack;
}

void stackDel(Stack stack) {
    stackClear(stack);
    free(stack);
}



bool stackIsEmpty(Stack stack) {
    return !stack->first;
}

Stack stackClear(Stack stack) {
    StackNode node = stack->first;
    StackNode noden;

    while(node) {
        noden = node;
        node=node->next;
        free(noden->data);
        free(noden);
    }
    stack->first=NULL;
    return stack;
}

int stackCount(Stack stack) {
    int size=0;
    StackNode node = stack->first;

    while(node) {
        size++;
        node = node->next;
    }
    return size;
}



Ptr stackPeek(Stack stack) {
    return stack->first->data;
}



Stack stackPush(Stack stack, Ptr data) {
    StackNode newnode = malloc(sizeof(struct _StackNode));

    newnode->next=stack->first;
    stack->first=newnode;

    newnode->data=malloc(stack->elemSize);
    memcpy(newnode->data, data, stack->elemSize);

    return stack;
}



Stack stackPop(Stack stack) {
    StackNode oldnode = stack->first;

    stack->first=oldnode->next;

    free(oldnode->data);
    free(oldnode);

    return stack;
}



void stackHeap(Stack stack) {
    int elts=stackCount(stack);
    int effcost=elts*stack->elemSize;
    int opcost=sizeof(struct _Stack)+elts*sizeof(struct _StackNode);

    printf("Stack at %p\n", stack);
    printf("\t%d elements, each using %d bytes\n", elts, stack->elemSize);
    printf("\t%d bytes used for elemets\n", effcost);
    printf("\t%d bytes used as operating cost\n", opcost);
    printf("\t%d bytes total used\n", opcost+effcost);
}
