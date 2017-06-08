#include "hashtable.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static unsigned hash(const char * str) // Rot13 hash-func
{

	unsigned hash = 0;

	for(; *str; str++)
	{
		hash += (unsigned char)(*str);
		hash -= (hash << 13) | (hash >> 19);
	}

	return hash % BUCKET_COUNT;

}

HtItem* ht_search(HashTable* ht, const char* s) {
    HtItem*  ptr  = NULL;
    unsigned h    = hash(s);

    for (ptr = ht->buckets[h]; ptr != NULL; ptr = ptr->next)
        if (strcmp(s, ptr->str) == 0) {
            //printf("%s found!\n", s);
            return ptr;
        }
    return NULL;
}

int ht_insert(HashTable* ht, const char* s) {
    if (ht_search(ht, s) == NULL) {
        HtItem* item = (HtItem*)calloc(1, sizeof(*item));
        unsigned h = hash(s);
        item->next = ht->buckets[h];
        item->str  = s;
        ht->buckets[h] = item;
    }
    return 0;
}

