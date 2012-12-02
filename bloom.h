#ifndef BLOOM_H
#define BLOOM_H

typedef unsigned int (*hashfuncs_l_t) (const char *, size_t len);
typedef unsigned int (*hashfuncs_t) (const char *);

typedef struct {
  size_t size;
  size_t nhash;
  size_t nfuncs;
  hashfuncs_l_t *hashfuncs;
  char *bits;
} bloom_t;

unsigned int sax_hash(const char *key);
unsigned int sax_hash_l(const char *key, size_t len);
unsigned int fnv_hash(const char *key);
unsigned int fnv_hash_l(const char *key, size_t len);
unsigned int djb2_hash(const char *key);
unsigned int djb2_hash_l(const char *key, size_t len);
bloom_t *bloom_init(size_t size, size_t nfunc, int use_l, ...);
void bloom_destroy(bloom_t *bloom);
int bloom_check(bloom_t *bloom, const char *s);
int bloom_check_t(bloom_t *bloom, const char *s, size_t len);
void bloom_add(bloom_t *bloom, const char *s);
void bloom_add_l(bloom_t *bloom, const char *s, size_t len);

#endif
