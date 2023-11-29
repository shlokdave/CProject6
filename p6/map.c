/**
    @file map.c
    @author Shlok Dave (ssdave)
    Implementation for the map component, a hash map.
  */

#include "map.h"
#include <stdlib.h>
#include "value.h"

typedef struct MapPairStruct MapPair;

/** Key/Value pair to put in a hash map. */
struct MapPairStruct
{
  /** Key part of this node, stored right in the node to improve locality. */
  Value key;

  /** Value part of this node, stored right in the node to improve locality. */
  Value val;

  /** Pointer to the next node at the same element of this table. */
  MapPair *next;
};

/** Representation of a hash table implementation of a map. */
struct MapStruct
{
  /** Table of key / value pairs. */
  MapPair **table;

  /** Length of the table. */
  int tlen;

  /** Number of key / value pairs in the map. */
  int size;
};

/**
  Helper function that is designed to help implement a new empty hash table. This table
  takes in a specific size as an integer to develop the table. It is mainly created to help
  with the implementation of the makeMap function. Calloc is used to help automatically initialize
  the required memory to zero.
  @param size the size of the hash table that needs to be created.
  @return pointer to the new hash table that is created.
 */
static MapPair **implementNewTable(int size)
{
  MapPair **newTable = calloc(size, sizeof(MapPair *));
  return newTable;
}

/**
  This function is responsible for creating an empty, dynamically allocated Map. The function
  initializes its fields and helps return a pointer of the new map created. The len parameter
  helps give the initial size of the hash table. In all, the function helps creates a new map
  with a certain specified length.
  @param len the length of the hash table that is created within the new map.
  @return pointer of the new map that is created.
*/
Map *makeMap(int len)
{
  // Memory is allocated for the map struct.
  Map *newMap = calloc(1, sizeof(Map));
  if (!newMap || !(newMap->table = implementNewTable(len)))
  {
    // Free memory if allocation is failed
    free(newMap);
    return NULL;
  }

  newMap->tlen = len;

  // Pointer returned after initializing fields.
  return newMap;
}

/**
  This function is responsible for returning the current number of key/value
  pairs that are in the provided map. It uses the ternary operator for directly
  checking if the map is non-null.
  @param m pointer to the map to check what the size is.
  @return the size of the map, 0 is returned if the map is null.
*/
int mapSize(Map *m)
{
  // Returning 0 if the pointer to map is null, if not, return size of map.
  return m ? m->size : 0;
}

/**
  This function is responsible for adding the given key/value pair to the provided
  map. For the function, if the key is already in the map, it is replaced with the given
  value. The map will gain ownership of the key and value objects and can move the values
  into its representation if needed.
  @param m pointer to the map to help set the key and values.
  @param key pointer to the key value that it needs to be set.
  @param val pointer to the value that needs to be together with the key.
*/
void mapSet(Map *m, Value *key, Value *val)
{
  // Simple check to see if there are null pointers.
  if (!m || !key || !val)
    return;

  // Hash value is calculated for key.
  unsigned int newHash = key->hash(key);
  int mapIdx = newHash % m->tlen;

  // Double pointer is used to traverse properly.
  MapPair **currPairs = &m->table[mapIdx];
  while (*currPairs)
  {
    // If the current key matches the given key, the value is replaced.
    if (key->equals(&(*currPairs)->key, key))
    {
      // The previous key is freed.
      (*currPairs)->val.empty(&(*currPairs)->val);
      val->move(val, &(*currPairs)->val);
      return;
    }
    currPairs = &(*currPairs)->next;
  }

  // If the key is not found, memory is allocated.
  MapPair *keySearch = malloc(sizeof(MapPair));
  if (!keySearch)
    return;

  // Initialize the map pair with the given value and key.
  key->move(key, &keySearch->key);
  val->move(val, &keySearch->val);
  keySearch->next = m->table[mapIdx];
  m->table[mapIdx] = keySearch;

  m->size++;
}

/**
  This function is implemented to return the value associated with the given key.
  If the key is not part of the map, it is returned as null. The new value that is returned
  is still part of the map representation, as the value should not be changed. In general, the
  function is responsible for getting the value that is part of the specific key in the map.
  @param m pointer to the map to retrieve the specific value.
  @param key pointer to the key whose value the function will get.
  @return pointer to the value that is part of the specific key.
*/
Value *mapGet(Map *m, Value *key)
{
  // Simple check to see if there are null pointers.
  if (!m || !key)
    return NULL;

  // Hash value is calculated for key.
  unsigned int newHash = key->hash(key);
  int idx = newHash % m->tlen;

  // Double pointer is used to traverse properly.
  MapPair **currPairs = &m->table[idx];
  while (*currPairs)
  {
    // If the current key matches the given key, get the value.
    if (key->equals(&(*currPairs)->key, key))
    {
      return &(*currPairs)->val;
    }
    currPairs = &(*currPairs)->next;
  }

  // Return null if the key is not found.
  return NULL;
}

/**
  This function acts to remove the key/pair for the given key from the
  provided map. The function's implementation is similar to the previous functions,
  expect that we are removing the specific key/value. It returns true if there was
  a matching key for removal and false otherwise.
  @param m pointer to the map for where the key/value will be removed.
  @param key pointer to the key that is going to be removed from the map.
*/
bool mapRemove(Map *m, Value *key)
{
  // Simple check to see if there are null pointers.
  if (!m || !key)
    return false;

  // Hash value is calculated for key.
  unsigned int newHash = key->hash(key);
  int idx = newHash % m->tlen;

  // Double pointer is used to traverse properly.
  MapPair **currPairs = &m->table[idx];
  MapPair *valRem = NULL;

  while (*currPairs != NULL)
  {
    // If key is found, prepare to remove it.
    if (key->equals(&(*currPairs)->key, key))
    {
      valRem = *currPairs;
      *currPairs = valRem->next;

      // Key and value are freed along with the map pair.
      valRem->key.empty(&valRem->key);
      valRem->val.empty(&valRem->val);
      free(valRem);

      m->size--;
      return true;
    }
    currPairs = &(*currPairs)->next;
  }

  // Return false if the key needed for removal is not found.
  return false;
}

/**
  This function is responsible for freeing all of the memory that is used
  to store the provided map. In this process, it includes freeing the memory
  for the hash table and all of the map pairs.
  @param m pointer to the map that needs to be freed.
*/
void freeMap(Map *m)
{
  // Simple check to see if there are null pointers.
  if (m == NULL)
    return;

  // Go through each part of the hash table.
  for (int idx = 0; idx < m->tlen; idx++)
  {
    MapPair *currPairs = m->table[idx];

    // Free memory for each of the keys and values.
    while (currPairs != NULL)
    {
      MapPair *valFree = currPairs;
      currPairs = currPairs->next;

      // Key and value are freed along with the map pair.
      valFree->key.empty(&valFree->key);
      valFree->val.empty(&valFree->val);
      free(valFree);
    }
  }

  // Hash table and map are freed.
  free(m->table);
  free(m);
}