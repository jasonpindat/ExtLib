/**
 * \file Hash.h
 * \brief Primitives functions for hash tables
 * \author Jason Pindat
 * \date 2016-12-04
 *
 * All the basic functions to manage dynamic hash tables.
 * Hash is an Iterable Collection.
 *
 * Copyright 2014-2016
 *
 */

#ifndef HASH_H
#define HASH_H

#include "Common.h"

/** Hash : type for a hash table. */
typedef struct _Hash *Hash;

/** HashNode : type for a node in a hash table. */
typedef struct _HashNode *HashNode;



/** \brief Creates a new hash table.
 *
 * \param keySize : the size in bytes of each key of the hash table. You can use the EL_* constants for the basic types, this will automatically link the comparison function too.
 * \param elemSize : the size in bytes of each element of the hash table.
 * \param hashFct : a hash function.
 * \return New empty hash table.
 *
 */
Hash hashNew(int keySize, int elemSize, ElHashFct hashFct);

/** \brief Creates a new hash table with string keys.
 *
 * \param elemSize : the size in bytes of each element of the hash table.
 * \return New empty hash table.
 *
 */
Hash hashNewStr(int elemSize);

/** \brief Destroys a hash table and all its content.
 *
 * \param h : Hash to destroy.
 * \return void
 *
 */
void hashDel(Hash h);



/** \brief Sets the function to compare 2 keys of this hash table, note that if you declared the hash table key with EL_*, the comparison function of the specified type is automatically linked.
 *
 * \param h : Hash in which you set the fonction.
 * \param fct : pointer to the function, the function must take 2 pointers to the keys and return an int which is <0 if 1st value is lower tha 2nd, >0 for the opposite and =0 if 1st value equals 2nd value.
 * \return nothing.
 *
 */
void hashComparable(Hash h, ElCmpFct fct);



/** \brief Copies a hash table and all its content.
 *
 * \param h : Hash to copy.
 * \return Copy of the hash table
 *
 */
Hash hashClone(const Hash h);



/** \brief Removes the whole content of a hash table.
 *
 * \param h : Hash to clear.
 * \return nothing.
 *
 */
void hashClear(Hash h);



/** \brief Tells whether an hash table is empty or not
 *
 * \param h : Hash to look in.
 * \return true if empty, false if not.
 *
 */
bool hashIsEmpty(const Hash h);

/** \brief Returns the length of the hash table.
 *
 * \param h : Hash to count elements.
 * \return Number of elements.
 *
 */
int hashLength(const Hash h);



/** \brief Tells whether the hash table contains an element or not, hashComparable must have been called or the hash table must have been created with a EL_* constant for keySize.
 *
 * \param h : Hash to look into.
 * \param key : A key.
 * \return true if found, false otherwise.
 *
 */
bool hashContains_base(const Hash h, const Ptr key);
#define hashContains(h, key) hashContains_base(h, &(key))



/** \brief Returns the element associated to a given key in a hash table.
 *
 * \param h : Hash to seek in.
 * \param key : A key.
 * \return Pointer to data.
 *
 */
const Ptr hashGet_base(const Hash h, const Ptr key);
#define hashGet(h, key, type) (*(type*)hashGet_base(h, &(key)))



/** \brief Sets the element at a given position in a hash table.
 *
 * \param h : Hash to modify.
 * \param key : A key.
 * \param data : Pointer to the data.
 * \return nothing.
 *
 */
void hashSet_base(Hash h, const Ptr key, const Ptr data);
#define hashSet(h, key, data) hashSet_base(h, &(key), &(data))



/** \brief Removes the element for given key in the hash table.
 *
 * \param h : Hash to remove in.
 * \param key : A key.
 * \return nothing.
 *
 */
bool hashUnset_base(Hash h, const Ptr key);
#define hashUnset(h, key) hashUnset_base(h, &(key))



// Iteration

typedef struct {
    Hash hash;
    int index;
    HashNode node;
    HashNode lastNode;
    bool onNext;
} HashIt;



/** \brief Creates an iterator on the hash table (Starting with the first element).
 *
 * \param h : Hash to iterate.
 * \return Iterator on this hash table.
 *
 */
HashIt hashItNew(const Hash h);



/** \brief Determines whether the element pointed by the iterator exists or it is the end of the iteration
 *
 * \param it : Iterator on a hash table.
 * \return true if element exists, false otherwise.
 *
 */
bool hashItExists(const HashIt *it);



/** \brief Positions the iterator on the next element
 *
 * \param it : Iterator on a hash table.
 * \return nothing.
 *
 */
void hashItNext(HashIt *it);



/** \brief Returns the key pointed by the iterator
 *
 * \param it : Iterator on a hash table.
 * \return element.
 *
 */
const Ptr hashItGetKey_base(const HashIt *it);
#define hashItGetKey(it, type) (*(type*)hashItGetKey_base(it))

/** \brief Returns the element pointed by the iterator
 *
 * \param it : Iterator on a hash table.
 * \return element.
 *
 */
const Ptr hashItGet_base(const HashIt *it);
#define hashItGet(it, type) (*(type*)hashItGet_base(it))



/** \brief Updates the element pointed by the iterator
 *
 * \param it : Iterator on a hash table.
 * \return nothing.
 *
 */
void hashItSet_base(HashIt *it, const Ptr data);
#define hashItSet(it, data) hashItSet_base(it, &(data))



/** \brief Removes the element pointed by the iterator.
 *
 * \param it : Iterator on a hash table.
 * \return nothing.
 *
 */
void hashItRemove(HashIt *it);

#endif
