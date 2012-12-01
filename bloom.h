#ifndef BLOOM_H
#define BLOOM_H

typedef unsigned int (*hashfuncs_t) (const char *);

typedef struct {
  char *name;
  size_t size;
  size_t nhash;
  size_t nfuncs;
  hashfuncs_t *hashfuncs;
  char *bits;
} bloom_t;

bloom_t *bloom_init(size_t size, size_t nfunc, ...);
void bloom_destroy(bloom_t *bloom);
int bloom_check(bloom_t *bloom, const char *s);
void bloom_add(bloom_t *bloom, const char *s);

#endif
