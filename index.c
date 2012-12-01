#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "ppgf.h"
#include "kseq.h"
#include "bloom.h"

KSEQ_INIT(gzFile, gzread)

extern int index_usage() {
  fprintf(stderr, "\nusage: %s index <in.fasta>\n\n", PROGRAM_NAME);
  fprintf(stderr, "options:     -b INT        number of bits (default 64)\n");
  fprintf(stderr, "             -k INT        k-mer size (default 33)\n");
  return EXIT_FAILURE;
}

int index_main(int argc, int *argv[]) {
  
  return EXIT_SUCCESS;
}
