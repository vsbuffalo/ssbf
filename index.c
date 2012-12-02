#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "ssbf.h"
#include "kseq.h"
#include "bloom.h"

/* TODO don't repeat */
#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))

KSEQ_INIT(gzFile, gzread)
static int verbose_flag=0;

static struct option long_options[] = {
  {"k-size", required_argument, 0, 'k'},
  {"bit-size", required_argument, 0, 'b'},
  {"verbose", no_argument, &verbose_flag, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

extern int index_usage() {
  fprintf(stderr, "\nUsage: %s index <in.fasta>\n\n", PROGRAM_NAME);
  fprintf(stderr, "Options:     -b INT        number of bits (default %d)\n", BITSIZE);
  fprintf(stderr, "             -k INT        k-mer size (default %d)\n", KSIZE);
  return EXIT_FAILURE;
}

void bloom_list_dump(bloom_list_t *bfilters) {
  
}

bloom_t *bloom_list_write(bloom_t *bloom, const char *filename) {
  int n=2;
  char *test = calloc(n, sizeof(char));
  strcpy(test, "aa");
  SETBIT(test, 1);
  SETBIT(test, 10);
  FILE *file = fopen(filename, "wb");
  if (!file) {
    fprintf(stderr, "Could not open file '%s' for writing\n", filename);
    exit(EXIT_FAILURE);
  }
  fwrite(test, sizeof(char), n*sizeof(char), file);
}

bloom_t *create_bloom_filter(const gzFile ref_fp, const int k, const int b) {
  /* 
     Create a bloom filter of b bits, from k-mers of size k from FASTA
     file entries.
  */
  bloom_t *bloom = bloom_init((size_t) b, NFUNCS, HASHFUNCS);
  kseq_t *seq;
  int l, i;
  seq = kseq_init(ref_fp);
  while ((l = kseq_read(seq)) >= 0) {
    /* hash all k-mers */
    for (i=0; i < seq->seq.l-k; i++) {
      bloom_add(bloom, seq->seq.s+i, k);
    }
  }
  kseq_destroy(seq);
  gzclose(ref_fp);
}

int index_main(int argc, char *argv[]) {
  int i, optc, k=KSIZE, b=BITSIZE;
  gzFile ref_fp=NULL;
  bloom_t *bloom;
  while (1) {
    int option_index=0;
    optc = getopt_long(argc, argv, "vb:k:", long_options, &option_index);
    if (optc < 0) break;
    switch (optc) {
      if (long_options[option_index].flag != 0) break;
    case 'b':
      b = atoi(optarg);
      break;
    case 'k':
      k = atoi(optarg);
      break;
    }
  }

  i = ++optind; /* remove subcommand */
  if (i == argc-1) {
    ref_fp = gzopen(argv[i], "r");
    fprintf(stderr, "[index] creating bloom filter for FASTA file %s...\t", argv[i]);
    bloom = create_bloom_filter(ref_fp, k, b);
    fprintf(stderr, "done\n", argv[i]);
    bloom_list_write(bloom, "test.bbf");
  } else if (i < argc-1) {
    fprintf(stderr, "error: too many arguments provided\n");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stderr, "error: no FASTA file provided\n");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
