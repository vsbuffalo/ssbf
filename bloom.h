#ifndef BLOOM_H
#define BLOOM_H

#include <stdint.h>

typedef unsigned int (*hashfuncs_t) (const char *, size_t len);

typedef struct {
  uint32_t size;
  size_t nhash;
  size_t nfuncs;
  uint32_t nchar;
  uint32_t k;
  hashfuncs_t *hashfuncs;
  char *bits;
  char *name;
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
