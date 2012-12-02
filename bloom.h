#ifndef BLOOM_H
#define BLOOM_H

#include <stdint.h>

typedef unsigned int (*hashfuncs_t) (const char *, size_t len);

/* fields to be written to binary file are fixed width */
typedef struct {
  uint32_t size; /* number of bits */
  size_t nfuncs; /* number of hash functions */
  uint32_t nchar; /* number of char-sized blocks used in creating bit
                     field */
  uint32_t k; /* k-mer size */
  uint32_t nseqs; /* number of sequences in bloom filter */
  hashfuncs_t *hashfuncs; /* hash functions; i.e. unsigned (char *key,
                             size_t len) */
  char *bits; /* bit field */
  char *name; /* name metainformation (size=MAX_NAME_SIZE) */
} bloom_t;


void *xmalloc(size_t size);
unsigned int sax_hash(const char *key);
unsigned int sax_hash_l(const char *key, size_t len);
unsigned int fnv_hash(const char *key);
unsigned int fnv_hash_l(const char *key, size_t len);
unsigned int djb2_hash(const char *key);
unsigned int djb2_hash_l(const char *key, size_t len);
bloom_t *bloom_init(size_t size, size_t nfunc, ...);
void bloom_destroy(bloom_t *bloom);
int bloom_check(bloom_t *bloom, const char *s, size_t len);
void bloom_add(bloom_t *bloom, const char *s, size_t len);

#endif
