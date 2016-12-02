#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

/** List : type for a doubly-linked list. */
typedef struct _Hash *Hash;

/** ListNode : type for a node in a doubly-linked list. */
typedef struct _HashNode *HashNode;

/**
 * \fn Hash hash_init(int (*cmp)(void *, void *), int (*hash)(void *))
 * \brief Creates a new hash table.
 * \param cmp Comparison fuction.
 * \param hash Hash function.
 * \return A new hash table.
 */
Hash hashNew(int (*cmp)(void *, void *), int (*hash)(void *));

/**
 * \fn int hash_add(Hash hash, void *data)
 * \brief Adds an element to a hash table.
 * \param hash A hash table.
 * \param data A pointer to the data.
 * \return 0 if ok, anything else if an error occurs.
 */
bool hashAdd(Hash hash, void *data);

/**
 * \fn int hash_get(Hash hash, void *data)
 * \brief Gets an element from a hash table.
 * \param hash A hash table.
 * \param data A pointer to copy the data.
 * \return 0 if ok, anything else if an error occurs.
 */
void *hashGet(Hash hash, void *data);

bool hashRemove(Hash hash, void *data);

/**
 * \fn void hash_free(Hash hash, void (*release)(void *))
 * \brief Destroys a hash table.
 * \param hash A hash table.
 * \param release Function to free the elements.
 */
void hashDel(Hash hash, void (*release)(void *));

#endif
