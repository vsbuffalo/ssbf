#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "ssbf.h"
#include "kseq.h"
#include "bloom.h"

/* KSEQ_INIT(gzFile, gzread) */

extern int index_usage() {
  fprintf(stderr, "\nUsage: %s index <in.fasta>\n\n", PROGRAM_NAME);
  fprintf(stderr, "Options:     -b INT        number of bits (default 64)\n");
  fprintf(stderr, "             -k INT        k-mer size (default 33)\n");
  return EXIT_FAILURE;
}

extern int index_main(int argc, char *argv[]) {
  
  return EXIT_SUCCESS;
}
