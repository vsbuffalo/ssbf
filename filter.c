#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <getopt.h>
#include "kseq.h"
#include "bloom.h"
#include "ppgf.h"

KSEQ_INIT(gzFile, gzread)

static struct option long_options[] = {
  {"k-size", required_argument, 0, 'k'},
  {"indices", required_argument, 0, 'i'},
  {"verbose", required_argument, 0, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

extern int filter_usage() {
  fprintf(stderr, "\nusage: ppgf filter <in.fasta>\n\n");
  fprintf(stderr, "options:     -k INT        number of k-mer matches for\n");
  fprintf(stderr, "                           classification (default 5)\n");
  fprintf(stderr, "             -f FILE(S)    binary indices of contaminants, in\n");
  fprintf(stderr, "                           format \"species_a=a.bbf,species_b=b.bbf\"\n");
  fprintf(stderr, "arguments:   <in.fastx>    FASTA or FASTQ input file, '-' \n");
  fprintf(stderr, "                           for stdin (optionally gzipped compressed)\n");
  return EXIT_FAILURE;
}



void init_bloom_filters(const char *filter_str) {
  /*
    Take the string in format:

    "human:/path/to/human.bbf;fungal:/path/to/fungal.bbf"

    and load bloom filters from binary file.
  */
  bloom_list_t *bfilters = malloc(sizeof(bloom_list_t));
  CHECK_MALLOC(bfilters);
}

int filter_main(int argc, char *argv[]) {
  int verbose=0, k=33;
  int optc;
  
  while (1) {
    int option_index=0;
    optc = getopt_long(argc, argv, "vk:i:", long_options, &option_index);
    if (optc < 0) break;
    switch (optc) {
      if (long_options[option_index].flag != 0) break;
    case 'v':
      verbose = 1;
      break;
    case 'k':
      k = atoi(optarg);
      break;
    case 'i':
      break;
    case_GETOPT_HELP_CHAR;
    case_GETOPT_VERSION_CHAR(PROGRAM_NAME, VERSION, AUTHORS);
    default:
      return filter_usage();
    }
  }
  return EXIT_SUCCESS;
}
