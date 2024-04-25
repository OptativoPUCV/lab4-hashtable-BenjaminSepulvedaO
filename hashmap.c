#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  int pos = hash(key, map->capacity);
  if (map->buckets[pos] == NULL)
  {
    map->buckets[pos] = createPair(key, value);
    map->size++;
  }
  else
  {
    while (map->buckets[pos] != NULL)
      {
        pos++;
        if (pos == map->capacity)
        {
          pos = 0;
        }
      }
    map->buckets[pos] = createPair(key, value);
    map->size++;
  }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {    
  long pos = hash(key, map->capacity);
  long primerIndex = pos;
  while (map->buckets[pos] != NULL)
    {
      if (is_equal(map->buckets[pos]->key, key))
      {
        map->buckets[pos]->key = NULL;
        map->size--;
        return;
      }
      pos++;
      if (pos == map->capacity)
        pos = 0;
      if (pos == primerIndex)
        return;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
  long pos = hash(key, map->capacity);
  long primerIndex = pos;
  while (1)
    {
      if (map->buckets[pos]==NULL)
        return NULL;
      if (strcmp(key, map->buckets[pos]->key) == 0)
      {
        map->current = pos;
        return map->buckets[pos];
      }
      pos = (pos +1) % map->capacity;
      if (pos == primerIndex)
        return NULL;
    }
}

Pair * firstMap(HashMap * map) {
  long i = 0;
  while (1)
    {
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
      {
        map->current = i;
        return map->buckets[i];
      }
      i++;
      if (i == map->capacity)
      {
        map->current = -1;
        return NULL;
      }
    }
  map->current = -1;
  return NULL;
}

Pair * nextMap(HashMap * map) {
  long i = map->current + 1;
  long primerIndex = i;
  while (1)
    {
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
      {
        map->current = i;
        return map->buckets[i];
      }
      i = (i + 1) % map->capacity;
      if (i == primerIndex)
      {
        return NULL;
      }
    }
  return NULL;
}
