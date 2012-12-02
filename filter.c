#include "ssbf.h"

static int verbose_flag=0;

KSEQ_INIT(gzFile, gzread)

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
  int i;
  uint32_t size, nchar;
  FILE *tmp_fp;
  for (i=0; i < bfilters->size; i++) {
    tmp_fp = fopen(bfilters->filenames[i], "rb");
    fread(&size, sizeof(uint32_t), 1, tmp_fp);
    bfilters->blooms[0] = bloom_init(size, NFUNCS, HASHFUNCS);
    fread(&bfilters->blooms[0]->k, sizeof(uint32_t), 1, tmp_fp);
    fread(&bfilters->blooms[0]->nseqs, sizeof(uint32_t), 1, tmp_fp);
    nchar = bfilters->blooms[0]->nchar;
    bfilters->blooms[0]->name = xmalloc(sizeof(char)*MAX_NAME_SIZE);
    fread(bfilters->blooms[0]->name, sizeof(char), MAX_NAME_SIZE, tmp_fp);
    bfilters->blooms[0]->bits = xmalloc(sizeof(char)*nchar);
    memset(bfilters->blooms[0]->bits, 0, sizeof(char)*nchar);
    fread(bfilters->blooms[0]->bits, sizeof(char), nchar, tmp_fp);
    fprintf(stderr, "[filter] loaded bloom filter '%s' (k=%u, nchar=%u, bitsize=%u)\n", \
            bfilters->blooms[0]->name, bfilters->blooms[0]->k, bfilters->blooms[0]->nchar, \
            bfilters->blooms[0]->size);
  }
}

unsigned int filter_reads(bloom_list_t *bfilters, FILE *input_fp, int n_kmer) {
  kseq_t *seq;
  int i, k, l;
  unsigned int nremoved=0, n;
  seq = kseq_init(input_fp);
  k = bfilters->blooms[0]->k;
  while ((l = kseq_read(seq)) >= 0) {
    n = 0;
    /* hash all k-mers */
    for (i=0; i < seq->seq.l-k; i++) {
      n += bloom_check(bfilters->blooms[0], seq->seq.s+i, k);
    }
    if (n < n_kmer) {
      if (seq->comment.l > 0)
         fprintf(stdout, ">%s %s\n%s\n", seq->name.s, seq->comment.s, seq->seq.s);
      else
        fprintf(stdout, ">%s\n%s\n", seq->name.s, seq->seq.s);
    } else {
      if (verbose_flag)
        fprintf(stderr, "'%s' had %d k-mer matches\n", seq->name.s, n);
      nremoved++;
    }
  }
  kseq_destroy(seq);
  gzclose(input_fp);
  return nremoved;
}

extern int filter_main(int argc, char *argv[]) {
  int n=NUMKMERS, ni=0, i;
  int optc;
  gzFile input_fp;
  bloom_list_t *bfilters = xmalloc(sizeof(bloom_list_t));
  bfilters->size=0;
  bfilters->filenames = xmalloc(sizeof(char*)*(argc-1));
  bfilters->blooms = xmalloc(sizeof(bloom_t*)*(argc-1));
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
      bfilters->blooms[ni] = xmalloc(sizeof(bloom_t));
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
  fprintf(stderr, "[filter] loading bloom filters...\n");
  load_bloom_filters(bfilters);

  i = ++optind; /* remove subcommand */
  if (i == argc-1) {
    input_fp = (strcmp(argv[i], "-") == 0) ? gzdopen(fileno(stdin), "r") : gzopen(argv[i], "r");
    if (!input_fp)
      fprintf(stderr, "error: could open file '%s'\n", argv[i]);
  } else if (i < argc-1) {
    fprintf(stderr, "error: too many arguments provided\n");
    exit(EXIT_FAILURE);
  } else {
    fprintf(stderr, "error: no FASTA file provided; to use stdin, specify '-'\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "[filter] filtering reads...\n");
  filter_reads(bfilters, input_fp, n);
  
  /* filter reads */
  return EXIT_SUCCESS;
}
