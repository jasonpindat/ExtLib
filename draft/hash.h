#ifndef HASH_H
#define HASH_H

struct Cell;
struct Hash_Table;

/**
 * \fn struct Hash_Table *hash_init(int (*cmp)(void *, void *), int (*hash)(void *))
 * \brief Creates a new hash table.
 * \param cmp Comparison fuction.
 * \param hash Hash function.
 * \return A new hash table.
 */
struct Hash_Table *hash_init(int (*cmp)(void *, void *), int (*hash)(void *));

/**
 * \fn int hash_add(struct Hash_Table *hash, void *data)
 * \brief Adds an element to a hash table.
 * \param hash A hash table.
 * \param data A pointer to the data.
 * \return 0 if ok, anything else if an error occurs.
 */
int hash_add(struct Hash_Table *hash, void *data);

/**
 * \fn int hash_get(struct Hash_Table *hash, void *data)
 * \brief Gets an element from a hash table.
 * \param hash A hash table.
 * \param data A pointer to copy the data.
 * \return 0 if ok, anything else if an error occurs.
 */
int hash_get(struct Hash_Table *hash, void *data);

int hash_remove(struct Hash_Table *hash, void *data);

/**
 * \fn void hash_free(struct Hash_Table *hash, void (*release)(void *))
 * \brief Destroys a hash table.
 * \param hash A hash table.
 * \param release Function to free the elements.
 */
void hash_free(struct Hash_Table *hash, void (*release)(void *));

#endif
