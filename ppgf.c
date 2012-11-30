#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <zlib.h>

#include "kseq.h"

#define VERSION 0.1

__KS_GETC(gzread, BUFFER_SIZE)
__KS_GETUNTIL(gzread, BUFFER_SIZE)
__KSEQ_READ

static int usage() {
  fprintf("\n");
  fprintf("ppgf: streaming contaminant removal with bloom filters\n");
  fprintf("version: %s\n", VERSION);
  fprintf("contact: Vince Buffalo <vsbuffaloAAAAA@gmail.com> (with poly-A tail)\n");
  fprintf("usage: ppgf <subcommand> [options]\n");
  fprintf("command:    hash            hash a reference FASTA file.\n");
  fprintf("            filter          filter contaminants\n");
  return 1;
}

int main(int argc, char *argv[]) {
  return 0;
}
