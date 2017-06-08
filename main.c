/*!
 * \file
 * Unoptimized hashtable benchmark.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "hashtable.h"

static const char* FILENAME = "war_and_peace.txt";

enum {
    MAX_WORD_COUNT = 64000,
    MAX_WORD_SIZE  = 64,
};

/*! Error codes */
enum {
    ERR_OK    = 0,
    ERR_FOPEN = 1
};

/*!
 * Fill the allocated buffer with words from file.
 */
static int load_wordbuf(const char* filename, char buf[][MAX_WORD_SIZE], size_t* cnt);

enum { 
    TEST_COUNT = 1000000
};

int main() {
    clock_t begin = clock();
    char buf[MAX_WORD_COUNT][MAX_WORD_SIZE] = {};
    HashTable ht  = {};
    size_t    cnt = 0;
    srand(time(NULL));

    if (load_wordbuf(FILENAME, buf, &cnt) != ERR_OK)
        fputs("Error while reading file!", stdout);
    for (size_t i = 0; i < cnt; i++)
        ht_insert(&ht, buf[i]);
    for (unsigned i = 0; i < TEST_COUNT; i++)
        ht_search(&ht, buf[rand() % BUCKET_COUNT]);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Total run time: %lg\n", time_spent);

    return 0;
}

int load_wordbuf(const char* filename, char buf[][MAX_WORD_SIZE], size_t* cnt) {
    assert(filename);
    assert(buf);

    FILE* f = fopen(filename, "r");
    if (!f)
        return ERR_FOPEN;

    size_t i = 0;
    while (i < MAX_WORD_COUNT && fscanf(f, "%63s", buf[i]) == 1) {
        i++;
    }
    *cnt = i;

    fclose(f);

    return ERR_OK;
}
