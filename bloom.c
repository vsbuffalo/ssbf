/*
  Bloom Character for Strings

  Code adapated from
  <http://en.literateprograms.org/Bloom_filter_(C)>. I've added hash
  functions from other sources and tried to increase readability.

  Copyright (C) 2012 Vince Buffalo

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
  
*/


#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ssbf.h"
#include "bloom.h"

#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))

/* hash functions */

unsigned int sax_hash(const char *key) {
  unsigned int h=0;
  while(*key) h^=(h<<5)+(h>>2)+(unsigned char)*key++;
  return h;
}

unsigned int fnv_hash(const char *key) {
  unsigned int h=2166136261;
  while (*key)
    h = (h*16777619)^(unsigned char)*key++;
  return h;
}

unsigned int djb2_hash(const char *key) {
  unsigned int h=5381;
  int c;
  while ((c = *key++))
    h = ((h << 5) + h) + c;
  return h;
}

/* bloom filter functions */

bloom_t *bloom_init(size_t size, size_t nfuncs, ...) {
  int n;
  va_list args;
  bloom_t *bloom = xmalloc(sizeof(bloom_t));
  bloom->hashfuncs = malloc(sizeof(hashfuncs_t)*nfuncs);
  bloom->bits = calloc((size+CHAR_BIT-1)/CHAR_BIT, sizeof(char));
  CHECK_MALLOC((bloom->hashfuncs && bloom->bits));

  va_start(args, nfuncs);
  for(n=0; n<nfuncs; n++) {
    bloom->hashfuncs[n]=va_arg(args, hashfuncs_t);
  }
  va_end(args);
  bloom->nfuncs = nfuncs;
  bloom->size = size;
  return bloom;
}

void bloom_destroy(bloom_t *bloom) {
  free(bloom->bits);
  free(bloom->hashfuncs);
  free(bloom);
}

int bloom_check(bloom_t *bloom, const char *s) {
  size_t n;
  for(n=0; n<bloom->nfuncs; n++) {
    if (!(GETBIT(bloom->bits, bloom->hashfuncs[n](s)%bloom->size)))
      return 0;
  }
  return 1;
}

void bloom_add(bloom_t *bloom, const char *s) {
  size_t n;
  for(n=0; n<bloom->nfuncs; ++n) {
    SETBIT(bloom->bits, bloom->hashfuncs[n](s)%bloom->size);
  }
}

/* int main(int argc, char *argv[]) { */
/*   /\* testing functions *\/ */
/*   int i; */
/*   bloom_t *bloom = bloom_init(1000, 4, sax_hash, fnv_hash, djb2_hash); */
  
/*   bloom_add(bloom, "in bloom filter"); */
/*   bloom_add(bloom, "also in bloom filter"); */
  
/*   printf("test 1: %i\n", bloom_check(bloom, "in bloom filter")); */
/*   printf("test 2: %i\n", bloom_check(bloom, "also in bloom filter")); */
/*   printf("test 3: %i\n", bloom_check(bloom, "not in bloom filter")); */
/*   bloom_destroy(bloom); */
/* } */
