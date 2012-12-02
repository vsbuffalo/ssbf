#include "ssbf.h"

static int verbose_flag=0;

/* KSEQ_INIT(gzFile, gzread) */

static struct option long_options[] = {
  {"num-kmers", required_argument, 0, 'n'},
  {"indices", required_argument, 0, 'i'},
  {"verbose", required_argument, &verbose_flag, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

extern int filter_usage() {
  fprintf(stderr, "\nUsage: ppgf filter <in.fasta>\n\n");
  fprintf(stderr, "Options:     -n INT        number of k-mer matches for\n");
  fprintf(stderr, "                           classification (default %d)\n", NUMKMERS);
  fprintf(stderr, "             -i FILE(S)    binary indices of contaminants\n");
  fprintf(stderr, "Arguments:   <in.fastx>    FASTA or FASTQ input file, '-' \n");
  fprintf(stderr, "                           for stdin (optionally gzipped compressed)\n");
  return EXIT_FAILURE;
}

void load_bloom_filters(bloom_list_t *bfilters) {
  /*
    Load bloom filters and their files from a list of filenames and
    names of these files.

    Note that we malloc argc-1 blocks of memory in filter_main() for
    the bloom filters. This is an over allocation by at most two
    blocks. If needed, we could resize these.
  */
  int i, k[1], nchar[1];
  FILE *tmp_fp;
  bfilters->blooms = xmalloc(sizeof(bloom_t)*(bfilters->size));
  for (i=0; i < bfilters->size; i++) {
    tmp_fp = fopen(bfilters->filenames[i], "rb");
    bfilters->blooms[0] = xmalloc(sizeof(bloom_t));
    bfilters->blooms[0]->name = xmalloc(sizeof(char)*MAX_NAME_SIZE);

    fread(nchar, sizeof(uint32_t), 1, tmp_fp);
    bfilters->blooms[0]->nchar = *nchar;
    fread(k, sizeof(uint32_t), 1, tmp_fp);
    bfilters->blooms[0]->k = *k;
    fread(bfilters->blooms[0]->name, sizeof(char), MAX_NAME_SIZE, tmp_fp);
    bfilters->blooms[0]->bits = xmalloc(sizeof(char) * (size_t) *nchar);
    fread(bfilters->blooms[0]->bits, sizeof(char), *nchar, tmp_fp);
    printf("nchar: %u\n", *nchar);
  }
}

size_t filter_reads(bloom_list_t *bfilters, FILE *input_fp) {
  printf("match: %u\n", bloom_check(bfilters->blooms[0], "GGTAZZZZZTTCGCTGCTGGGGTCGCTATCGGTGTGCACTGCATGATGACAGGTATCGACCCGCGCCTCTCTAACCTAGGGGGGCTCTGGGCGCTCGGGACT", 33));
}

extern int filter_main(int argc, char *argv[]) {
  int n=NUMKMERS, ni=0, i;
  int optc;
  FILE *input_fp=stdin;
  bloom_list_t *bfilters = xmalloc(sizeof(bloom_list_t));
  bfilters->size=0;
  bfilters->filenames = xmalloc(sizeof(bloom_t)*(argc-1));

  while (1) {
    int option_index=0;
    optc = getopt_long(argc, argv, "vn:i:", long_options, &option_index);
    if (optc < 0) break;
    switch (optc) {
      if (long_options[option_index].flag != 0) break;
    case 'n':
      n = atoi(optarg);
      break;
    case 'i':
      bfilters->filenames[ni] = xmalloc(sizeof(char)*strlen(optarg));
      strcpy(bfilters->filenames[ni], optarg);
      bfilters->size++;
      break;
    case_GETOPT_HELP_CHAR;
    case_GETOPT_VERSION_CHAR(PROGRAM_NAME, VERSION, AUTHORS);
    case '?':
    default:
      return filter_usage();
    }
  }

  /* take filenames and load their binary bff files' data */
  fprintf(stderr, "[filter] loading bloom filters...\t");
  load_bloom_filters(bfilters);
  fprintf(stderr, "done.\n");

  fprintf(stderr, "[filter] filtering reads...\t");
  filter_reads(bfilters, input_fp);
  fprintf(stderr, "done.\n");
  
  /* filter reads */
  return EXIT_SUCCESS;
}
