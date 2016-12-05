/**
 * \file Hash.c
 * \author Jason Pindat
 * \date 2016-12-05
 *
 * Copyright 2014-2016
 *
 */

#include "ExtLib/Common.h"
#include "ExtLib/Collection.h"
#include "ExtLib/Hash.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DEFSIZE 13 // prime number

struct _Hash {
    RealType type;
    ElCmpFct cmpFct;

    int elemSize;
    ElCopyFct copyFct;
    ElDelFct delFct;
    bool needsAllocation;
    Ptr (*ptrTransform)(Ptr);

    int length;

    int keySize;
    ElCopyFct keyCopyFct;
    ElDelFct keyDelFct;
    bool keyNeedsAllocation;
    Ptr (*keyPtrTransform)(Ptr);

    ElHashFct hashFct;
    int size;
    HashNode *ct;
};

struct _HashNode {
    HashNode next;
    void *data;
};



static bool isPrime(int number) { // number must be >= 4
    if (number%2==0 || number%3==0)
        return false;     // check if number is divisible by 2 or 3
    else {
        int bounder = sqrt(number);

        for (int i=5; i<=bounder; i+=6)
            if (number % i == 0 || number%(i + 2) == 0)
                return false;

        return true;
    }
}

static void hashResize(const Hash h, int minSize) {
    struct _Hash h2 = *h;

    h2.length = 0;

    if(minSize < DEFSIZE)
        h2.size = DEFSIZE;
    else {
        int number = minSize;
        while(!isPrime(number))
            number++;
        h2.size = number;
    }

    h2.ct = calloc(h2.size, sizeof(HashNode));

    for(HashIt it = hashItNew(h); hashItExists(&it); hashItNext(&it)) {
        hashSet_base(&h2, hashItGetKey_base(&it), hashItGet_base(&it));
        hashItRemove(&it);
    }

    free(h->ct);

    h->ct = h2.ct;
}



static unsigned long hashString(unsigned char **strp)
{
    unsigned long hash = 5381;
    int c;
    unsigned char *str = *strp;

    while((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static int compareString(char **str1, char **str2) {
    return strcmp(*str1, *str2);
}

static void copyString(char **dest, char **src) {
    int length = strlen(*src);

    *dest = malloc((length+1) * sizeof(char));

    strcpy(*dest, *src);
}

static void delString(char **obj) {
    free(*obj);
}



Hash hashNew(int keySize, int elemSize, ElHashFct hashFct) {
    Hash h = malloc(sizeof(struct _Hash));

    h->type = HASH;

    if(keySize<=0) {
        h->keySize=_elSizeFct(keySize);
        h->cmpFct=_elCompareFct(keySize);
    }
    else {
        h->keySize=keySize;
        h->cmpFct=NULL;
    }

    if(elemSize<=0)
        h->elemSize=_elSizeFct(elemSize);
    else
        h->elemSize=elemSize;

    collectionElementInstanciable((Collection)h, NULL, NULL);
    int offset = &h->keySize - &h->elemSize;
    collectionElementInstanciable((Collection)(h + offset), NULL, NULL);

    h->length = 0;

    h->hashFct = hashFct;
    h->size = DEFSIZE;
    h->ct = calloc(DEFSIZE, sizeof(HashNode));

    return h;
}

Hash hashNewStr(int elemSize) {
    Hash h = hashNew(sizeof(char *), elemSize, (ElHashFct)hashString);

    hashComparable(h, (ElCmpFct)compareString);

    int offset = &h->keySize - &h->elemSize;
    collectionElementInstanciable((Collection)(h + offset), (ElCopyFct)copyString, (ElDelFct)delString);

    return h;
}

void hashDel(Hash h) {

    /*for(int i=0; i<h->size; ++i) {

        HashNode p=h->ct[i];

        while(p) {
            HashNode ptr = p->next;

            if(release!=NULL)
                release(p->data);

            free(p);

            p = ptr;
        }
    }*/

    hashClear(h);

    free(h->ct);
    free(h);
}



void hashComparable(Hash h, ElCmpFct fct) {
    h->cmpFct = fct;
}



Hash hashClone(const Hash h) {
    Hash h2 = malloc(sizeof(struct _Hash));

    h2->type = HASH;
    h2->cmpFct = h->cmpFct;

    h2->elemSize = h->elemSize;
    h2->copyFct = h->copyFct;
    h2->delFct = h->delFct;
    h2->needsAllocation = h->needsAllocation;
    h2->ptrTransform = h->ptrTransform;

    h2->length = 0;

    h2->keySize = h->keySize;
    h2->keyCopyFct = h->keyCopyFct;
    h2->keyDelFct = h->keyDelFct;
    h2->keyNeedsAllocation = h->keyNeedsAllocation;
    h2->keyPtrTransform = h->keyPtrTransform;

    h2->hashFct = h->hashFct;

    h2->size = h->length * 2;
    if(h2->size < DEFSIZE)
        h2->size = DEFSIZE;
    else {
        int number = h2->size;
        while(!isPrime(number))
            number++;
        h2->size = number;
    }

    h2->ct = calloc(h2->size, sizeof(HashNode));

    for(HashIt it = hashItNew(h); hashItExists(&it); hashItNext(&it))
        hashSet_base(h2, hashItGetKey_base(&it), hashItGet_base(&it));

    return h2;
}



void hashClear(Hash h) {
    for(HashIt it = hashItNew(h); hashItExists(&it); hashItNext(&it))
        hashItRemove(&it);

    h->length = 0;

    h->size = DEFSIZE;
    h->ct = realloc(h->ct, DEFSIZE * sizeof(HashNode));
}



bool hashIsEmpty(const Hash h) {
    return h->length == 0;
}

int hashLength(const Hash h) {
    return h->length;
}



bool hashContains_base(const Hash h, const Ptr key) {
    return hashGet_base(h, key) != NULL;
}



const Ptr hashGet_base(const Hash h, const Ptr key) {
    int cmpRes;

    HashNode node = h->ct[h->hashFct(key)%h->size];

    while((node != NULL) && ((cmpRes=h->cmpFct(key, node+sizeof(struct _HashNode))) > 0))
        node = node->next;

    if(node != NULL && cmpRes == 0)
        return node->data;

    return NULL;
}



void hashSet_base(Hash h, const Ptr key, const Ptr data) {
    int index = h->hashFct(key)%h->size;

    HashNode nodeSave = NULL;
    HashNode node = h->ct[index];
    int cmpRes;

    while((node != NULL) && ((cmpRes=h->cmpFct(key, node+sizeof(struct _HashNode))) > 0)) {
        nodeSave = node;
        node = node->next;
    }

    if(node == NULL || cmpRes != 0) { // Node at this key does not exist

        if(10*h->length >= 8*h->size) {
            hashResize(h, 2*h->size);
            hashSet_base(h, key, data);
            return;
        }

        HashNode newnode = malloc(sizeof(struct _HashNode) + h->keySize + h->elemSize);
        newnode->data = node + sizeof(struct _HashNode) + h->keySize;
        newnode->next = node;

        if(nodeSave == NULL)
            h->ct[index] = newnode;
        else
            nodeSave->next = newnode;

        node = newnode;

        if(h->keyCopyFct)
            h->keyCopyFct(node+sizeof(struct _HashNode), key);
        else
            memcpy(node+sizeof(struct _HashNode), key, h->keySize);
    }
    else if(h->delFct)
        h->delFct(node->data);


    if(h->copyFct)
        h->copyFct(node->data, data);
    else
        memcpy(node->data, data, h->elemSize);
}



bool hashUnset_base(Hash h, const Ptr key) {
    int index = h->hashFct(key)%h->size;

    HashNode nodeSave = NULL;
    HashNode node = h->ct[index];
    int cmpRes;

    while((node != NULL) && ((cmpRes=h->cmpFct(key, node+sizeof(struct _HashNode))) > 0)) {
        nodeSave = node;
        node = node->next;
    }

    if(node == NULL || cmpRes != 0)
        return false;

    if(nodeSave == NULL)
        h->ct[index] = node->next;
    else
        nodeSave->next = node->next;

    if(h->keyDelFct)
        h->keyDelFct(node+sizeof(struct _HashNode));

    if(h->delFct)
        h->delFct(node->data);

    free(node);

    return true;
}



// Iteration

HashIt hashItNew(const Hash h) {
    HashIt it;

    it.hash = h;
    it.index = 0;

    while(it.index < h->size && !h->ct[it.index])
        it.index++;

    if(it.index >= h->size)
        it.node = NULL;
    else
        it.node = h->ct[it.index];

    it.lastNode = NULL;
    it.onNext = false;

    return it;
}



bool hashItExists(const HashIt *it) {
    return it->node != NULL;
}



void hashItNext(HashIt *it) {
    if(it->onNext)
        it->onNext = false;
    else {
        it->lastNode = it->node;

        if(it->node->next)
            it->node = it->node->next;
        else {
            do {
                it->index++;
            } while(it->index < it->hash->size && !it->hash->ct[it->index]);

            if(it->index >= it->hash->size)
                it->node = NULL;
            else
                it->node = it->hash->ct[it->index];
        }
    }
}



const Ptr hashItGetKey_base(const HashIt *it) {
    return it->node + sizeof(struct _HashNode);
}

const Ptr hashItGet_base(const HashIt *it) {
    return it->node->data;
}



void hashItSet_base(HashIt *it, const Ptr data) {
    if(it->hash->delFct)
        it->hash->delFct(it->node->data);

    if(it->hash->copyFct)
        it->hash->copyFct(it->node->data, data);
    else
        memcpy(it->node->data, data, it->hash->elemSize);
}



void hashItRemove(HashIt *it) {
    HashNode node = it->node;

    if(it->lastNode == NULL || it->lastNode->next != node)
        it->hash->ct[it->index] = node->next;
    else
        it->lastNode->next = node->next;

    hashItNext(it);

    if(it->hash->keyDelFct)
        it->hash->keyDelFct(node+sizeof(struct _HashNode));

    if(it->hash->delFct)
        it->hash->delFct(node->data);

    free(node);

    it->onNext = true;
}
