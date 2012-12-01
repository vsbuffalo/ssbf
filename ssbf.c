#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <zlib.h>
#include "ssbf.h"

extern int usage() {
  fprintf(stderr, "\n");
  fprintf(stderr, "%s: streaming contaminant removal with bloom filters\n", PROGRAM_NAME);
  fprintf(stderr, "version: %.1f\n", VERSION);
  fprintf(stderr, "contact: Vince Buffalo <vsbuffaloAAAAA@gmail.com> (with poly-A tail)\n");
  fprintf(stderr, "usage: %s <subcommand> [options]\n", PROGRAM_NAME);
  fprintf(stderr, "command:     index         create a bloom filter from a FASTA file.\n");
  fprintf(stderr, "             filter        filter contaminants\n");
  return 1;
}


int main(int argc, char *argv[]) {

  if (argc < 2) {
    return usage();
  }

  if (strcmp(argv[1], "index")) {
      if (argc < 3)
        return index_usage();
  } else if (strcmp(argv[1], "filter")) {
      if (argc < 3)
        return filter_usage();
  } else {
    fprintf(stderr, "error: command must be either 'hash' or 'filter'.\n");
    return usage();
  }
  return 0;
}
