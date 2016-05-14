#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define HASH_SIZE 13 // prime number

struct Cell {
    void *data;
    struct Cell *next, *prev;
};

struct Hash_Table {
    int (*cmp)(void *, void *);
    int (*hash)(void *);
    struct Cell **entry;
    int size;
};

static struct Cell *hash_new_cell(void *data) {
    struct Cell *c = malloc(sizeof(*c));
    c->data = data;
    c->next = c->prev = NULL;
    return c;
}

struct Hash_Table *hash_init(int (*cmp)(void *, void *), int (*hash)(void *)) {
    struct Hash_Table *hasht;
    int i;

    assert(cmp != NULL);
    assert(hash != NULL);

    hasht = malloc(sizeof(*hasht));
    if (hasht == NULL)
        return NULL;

    hasht->cmp = cmp;
    hasht->hash = hash;
    hasht->entry = malloc(sizeof(*(hasht->entry)) * HASH_SIZE);

    if (hasht->entry == NULL) {
        free(hasht);
        return NULL;
    }

    hasht->size=HASH_SIZE;

    for(i=0; i<hasht->size; ++i)
        hasht->entry[i] = NULL;

    return hasht;
}

int hash_add(struct Hash_Table *hash, void *data) { // A faire : gerer le cas si l'élément existe déja
    int value;
    int c;

    assert(hash != NULL);
    value = hash->hash(data);
    fprintf(stderr,"DEBUG: hash value is %d \n",value);

    if (hash->entry[value%hash->size] == NULL) {
        hash->entry[value%hash->size] = hash_new_cell(data);
    }
    else {
        struct Cell *q=NULL, *p=hash->entry[value%hash->size];

        while((p != NULL) && ((c=hash->cmp(data,p->data)) > 0)) {
            q=p;
            p=p->next;
        }
        if (c == 0)
            return 1; // element is already in the table

        struct Cell *cell = hash_new_cell(data);

        if (q == NULL) {
            p->prev = cell;
            cell->next = p;
            hash->entry[value%hash->size] = cell;
        }
        else {
            q->next = cell;
            cell->prev = q;
        }
    }
    return 0;
}

int hash_get(struct Hash_Table *hash, void *data) {
    int r=0, value;

    assert(hash!=NULL);

    value=hash->hash(data);
    struct Cell *p = hash->entry[value%hash->size];

    while((p!=NULL) && (r=hash->cmp(data,p->data)<0))
        p=p->next;
    if (r==0)
        return 0;
    /*if (p==NULL)
        return 1;*/
    return 1;
}

int hash_remove(struct Hash_Table *hash, void *data) {
    int r=0, value;

    assert(hash!=NULL);

    value=hash->hash(data);
    struct Cell *p = hash->entry[value%hash->size];

    while((p!=NULL) && (r=hash->cmp(data,p->data)<0))
        p=p->next;
    if (r==0)
        return 0;
    /*if (p==NULL)
        return 1;*/
    return 1;
}

void hash_free(struct Hash_Table *hash, void (*release)(void *)) {
    int i;

    assert(hash!=NULL);

    for(i=0; i<hash->size; ++i)
    {
        struct Cell *p=hash->entry[i];

        while(p) {
            struct Cell *ptr = p->next;

            if(release!=NULL)
                release(p->data);

            free(p);

            p = ptr;
        }
    }

    free(hash->entry);
    free(hash);
}
