#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define HASH_SIZE 13 // prime number

struct _HashNode { 
    HashNode next;
    void *data;
};

struct _Hash {
    int (*cmp)(void *, void *);
    int (*hashFct)(void *);
    int size;
    HashNode *ct;
};



Hash hash_init(int (*cmp)(void *, void *), int (*hashFct)(void *)) {

    assert(cmp != NULL);
    assert(hashFct != NULL);

    Hash h = malloc(sizeof(struct _Hash));
    h->cmp = cmp;
    h->hashFct = hashFct;
    h->size=HASH_SIZE;
    h->ct = malloc(sizeof(HashNode) * HASH_SIZE);

    for(int i=0; i<HASH_SIZE; ++i)
        h->ct[i] = NULL;

    return h;
}

bool hash_add(Hash h, void *data) {
    
    int index = h->hashFct(data)%h->size;

    HashNode nodeSave = NULL;
    HashNode node = h->ct[index];
    int cmpRes;

    while((node != NULL) && ((cmpRes=h->cmp(data, node->data)) > 0)) {
        nodeSave = node;
        node = node->next;
    }

    if(node != NULL && cmpRes == 0)
        return false;

    HashNode newnode = malloc(sizeof(struct _HashNode));
    newnode->data = data;
    newnode->next = node;

    if(nodeSave == NULL)
        h->ct[index] = newnode;
    else
        nodeSave->next = newnode;

    return true;
}

void *hash_get(Hash h, void *data) {
    int cmpRes;

    HashNode node = h->ct[h->hashFct(data)%h->size];

    while((node != NULL) && ((cmpRes=h->cmp(data, node->data)) > 0))
        node = node->next;

    if(node != NULL && cmpRes == 0)
        return node->data;

    return NULL;
}

bool hash_remove(Hash h, void *data) {

    int index = h->hashFct(data)%h->size;

    HashNode nodeSave = NULL;
    HashNode node = h->ct[index];
    int cmpRes;

    while((node != NULL) && ((cmpRes=h->cmp(data, node->data)) > 0)) {
        nodeSave = node;
        node = node->next;
    }

    if(node == NULL || cmpRes != 0)
        return false;

    if(nodeSave == NULL)
        h->ct[index] = node->next;
    else
        nodeSave->next = node->next;

    free(node);

    return true;
}

void hash_free(Hash h, void (*release)(void *)) {

    for(int i=0; i<h->size; ++i) {
        
        HashNode p=h->ct[i];

        while(p) {
            HashNode ptr = p->next;

            if(release!=NULL)
                release(p->data);

            free(p);

            p = ptr;
        }
    }

    free(h->ct);
    free(h);
}
