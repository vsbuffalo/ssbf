#include "ssbf.h"

/* TODO don't repeat */
#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))

KSEQ_INIT(gzFile, gzread)
static int verbose_flag=0;

static struct option long_options[] = {
  {"k-size", required_argument, 0, 'k'},
  {"bit-size", required_argument, 0, 'b'},
  {"output", required_argument, 0, 'o'},
  {"name", required_argument, 0, 'n'},
  {"verbose", no_argument, &verbose_flag, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

extern int index_usage() {
  fprintf(stderr, "\nUsage: %s index <in.fasta>\n\n", PROGRAM_NAME);
  fprintf(stderr, "Options:     -b INT        number of bits (default %f)\n", BITSIZE);
  fprintf(stderr, "             -k INT        k-mer size (default %d)\n", KSIZE);
  fprintf(stderr, "             -o FILE       output file (default stdout)\n");
  fprintf(stderr, "             -n STR        name\n");
  return EXIT_FAILURE;
}

void bloom_list_dump(bloom_list_t *bfilters) {
  
}

int bloom_filter_write(bloom_t *bloom, FILE *output_fp) {
  /* 
     Write the bitfields and other necessary meta-information to a
     binary file.

     Necessary information includes the number of char for the
     bitfield, and k. This is to be more friendly (and safe!) to make
     sure the user doesn't have to manually (and possibly incorrectly)
     specify them later.

     Binary Bloom Filter (bbf) file:

     [uint32_t nchar][uint32_t k][char MAX_NAME_SIZE][char * nchar bitfield]
  */
  printf("nchar: %u\n", &bloom->nchar);
  fwrite(&bloom->nchar, sizeof(uint32_t), 1, output_fp);
  fwrite(&bloom->k, sizeof(uint32_t), 1, output_fp);
  fwrite(bloom->name, sizeof(char), MAX_NAME_SIZE, output_fp);
  fwrite(bloom->bits, sizeof(char), sizeof(char)*bloom->nchar, output_fp);
  return 1;
}

bloom_t *bloom_filter_create(const gzFile ref_fp, const int k, const int b, char *name) {
  /* 
     Create a bloom filter of b bits, from k-mers of size k from FASTA
     file entries.
  */
  bloom_t *bloom = bloom_init((size_t) b, NFUNCS, HASHFUNCS);
  kseq_t *seq;
  int l, i, tot=0;
  bloom->k = k;
  bloom->size = b;
  bloom->name = xmalloc(sizeof(char)*MAX_NAME_SIZE);
  strcpy(bloom->name, name);
  seq = kseq_init(ref_fp);
  while ((l = kseq_read(seq)) >= 0) {
    /* hash all k-mers */
    for (i=0; i < seq->seq.l-k; i++) {
      bloom_add(bloom, seq->seq.s+i, k);
    }
    if (verbose_flag && tot % 1000 == 0)
      fprintf(stderr, "\t%i sequences added to bloom filter\r", tot);
  }
  kseq_destroy(seq);
  gzclose(ref_fp);
  return bloom;
}

int index_main(int argc, char *argv[]) {
  int i, optc, k=KSIZE, b=BITSIZE;
  gzFile ref_fp=NULL;
  bloom_t *bloom;
  FILE *output_fp=stdout;
  char *name;
  while (1) {
    int option_index=0;
    optc = getopt_long(argc, argv, "vb:k:o:n:", long_options, &option_index);
    if (optc < 0) break;
    switch (optc) {
      if (long_options[option_index].flag != 0) break;
    case 'b':
      b = atoi(optarg);
      break;
    case 'k':
      k = atoi(optarg);
      break;
    case 'n':
      name = optarg;
      if (strlen(name) > MAX_NAME_SIZE)
        fprintf(stderr, "error: option 'name' larger than maximum allow length (%i)", \
                MAX_NAME_SIZE);
      break;
    case 'o':
      output_fp = fopen(optarg, "wb");
      if (!output_fp) {
        fprintf(stderr, "Could not open file '%s' for writing\n", optarg);
        exit(EXIT_FAILURE);
      }
      break;
    }
  }
  
  if (!name) {
    fprintf(stderr, "error: option 'name' is required\n");
    exit(EXIT_FAILURE);
  }

  i = ++optind; /* remove subcommand */
  if (i == argc-1) {
    ref_fp = gzopen(argv[i], "r");
    fprintf(stderr, "[index] creating bloom filter for FASTA file %s...\t", argv[i]);
    bloom = bloom_filter_create(ref_fp, k, b, name);
    fprintf(stderr, "done.\n");
    fprintf(stderr, "[index] writing bloom filter to file %s...\t", argv[i]);
    if (!bloom_filter_write(bloom, output_fp))
      fprintf(stderr, "error: could not write binary file\n");
    fprintf(stderr, "done.\n");
  } else if (i < argc-1) {
    fprintf(stderr, "error: too many arguments provided\n");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stderr, "error: no FASTA file provided\n");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
