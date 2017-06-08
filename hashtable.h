#ifndef _HASHTABLE_H
#define _HASHTABLE_H

enum { 
    BUCKET_COUNT = 3007
};

struct HtItem {
    struct HtItem*     next;
    const char* str;
};
typedef struct HtItem HtItem;

typedef struct {
    HtItem* buckets[BUCKET_COUNT];
} HashTable;

HtItem* ht_search(HashTable* ht, const char* s);
int ht_insert(HashTable* ht, const char* s);

#endif
