#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "ssbf.h"
#include "kseq.h"
#include "bloom.h"

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

void create_bloom_filter(const gzFile ref_fp, const int k, const int b) {
  /* 
     Create a bloom filter from a FASTA file.
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

extern int index_main(int argc, char *argv[]) {
  int optc, k=KSIZE, b=BITSIZE;
  gzFile ref_fp=NULL;
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
  /* TODO assert only ONE argument provided */
  if (optind < argc) {
    create_bloom_filter(ref_fp, k, b);
  }
  return EXIT_SUCCESS;
}
