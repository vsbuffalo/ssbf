#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <getopt.h>
#include "kseq.h"
#include "bloom.h"
#include "ssbf.h"

/* KSEQ_INIT(gzFile, gzread) */

static struct option long_options[] = {
  {"k-size", required_argument, 0, 'k'},
  {"indices", required_argument, 0, 'i'},
  {"verbose", required_argument, 0, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

extern int filter_usage() {
  fprintf(stderr, "\nUsage: ppgf filter <in.fasta>\n\n");
  fprintf(stderr, "Options:     -k INT        number of k-mer matches for\n");
  fprintf(stderr, "                           classification (default 5)\n");
  fprintf(stderr, "             -i FILE(S)    binary indices of contaminants, in\n");
  fprintf(stderr, "                           format \"species_a=a.bbf,species_b=b.bbf\"\n");
  fprintf(stderr, "Arguments:   <in.fastx>    FASTA or FASTQ input file, '-' \n");
  fprintf(stderr, "                           for stdin (optionally gzipped compressed)\n");
  return EXIT_FAILURE;
}



void load_bloom_filters(bloom_list_t *bfilters) {
  /*
    Load bloom filters from a list of filenames and names of these
    files.

    Note that we malloc argc-1 blocks of memory in filter_main() for
    the bloom filters. This is an over allocation by at most two
    blocks. If needed, we could resize these.
  */
  int i;
  bfilters->blooms = xmalloc(sizeof(bloom_t)*(bfilters->size));
  bfilters->files = xmalloc(sizeof(gzFile)*(bfilters->size));
  for (i=0; i < bfilters->size; i++) {
    bfilters->files[i] = gzopen(bfilters->filenames[i], "rb");
    
  }
}

extern int filter_main(int argc, char *argv[]) {
  int verbose=0, k=33, ni=0;
  size_t len;
  int optc;
  char *subopts, *chunk, *scratch;
  bloom_list_t *bfilters = xmalloc(sizeof(bloom_list_t));
  bfilters->size=0;
  bfilters->filenames = xmalloc(sizeof(bloom_t)*(argc-1));
  bfilters->names = xmalloc(sizeof(bloom_t)*(argc-1));

  while (1) {
    int option_index=0;
    optc = getopt_long(argc, argv, "vk:i:", long_options, &option_index);
    if (optc < 0) break;
    switch (optc) {
      /* if (long_options[option_index].flag != 0) break; */
    case 'v':
      verbose = 1;
      break;
    case 'k':
      k = atoi(optarg);
      break;
    case 'i':
      subopts = optarg;
      len = strlen(subopts);
      fprintf(stdout, "%s\n", subopts);
      chunk = strtok_r(subopts, "=", &scratch);
      bfilters->filenames[ni] = xmalloc(sizeof(char)*strlen(chunk));
      bfilters->names[ni] = xmalloc(sizeof(char)*strlen(scratch));
      strcpy(bfilters->filenames[ni], chunk);
      strcpy(bfilters->names[ni], scratch);
      ni++;
      bfilters->size++;
      break;
    case_GETOPT_HELP_CHAR;
    case_GETOPT_VERSION_CHAR(PROGRAM_NAME, VERSION, AUTHORS);
    case '?':
    default:
      return filter_usage();
    }
  }
  return EXIT_SUCCESS;
}
