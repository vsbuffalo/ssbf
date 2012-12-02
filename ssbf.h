#ifndef PPGF_H
#define PPGF_H

/* For bloom.h - changes bloom_t slightly so that there is k-mer size
   (k) field as well as the bitsize field; these are necessaary to
   read the binary file. This is to increase portability; a BBF file
   should have all necessary parameters to run correctly (without the
   user having to recall and specify those. */

#define FORZOID

#include <getopt.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <zlib.h>
#include <limits.h>
#include "kseq.h"
#include "bloom.h"

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "ssbf"
#endif

#ifndef VERSION
#define VERSION 0.1
#endif

#ifndef DEFAULTS
#define DEFAULTS
#define KSIZE 33
#define BITSIZE 1000000
#define NUMKMERS 5
#endif

/* definitions for our hash functions */
#ifndef HASHFUNCS
#define HASHFUNCS sax_hash_l, fnv_hash_l, djb2_hash_l
#define NFUNCS 3
#endif

/* maximum size of names (must be fixed width for binary file
   handling)*/
#define MAX_NAME_SIZE 60

/* array structure for storing a list of bloom filters */
typedef struct {
  unsigned int size;
  gzFile **files;
  char **filenames;
  bloom_t **blooms;
} bloom_list_t;

/* function templates */
int usage();
extern int index_usage();
extern int filter_usage();
extern int index_main(int argc, char *argv[]);
extern int filter_main(int argc, char *argv[]);

/* macro for checking (and exiting) if malloc returns null; use
   xmallox mostly, this is for calloc, etc. */
#define CHECK_MALLOC(ptr)  if (!ptr) {                  \
    fprintf(stderr, "Out of memory; malloc failed\n");  \
    exit(EXIT_FAILURE);                                 \
  }                                                     \

/* Options drawn from GNU's coreutils/src/system.h */
/* These options are defined so as to avoid conflicting with option
   values used by commands */
enum {
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};
#define GETOPT_HELP_OPTION_DECL \
  "help", no_argument, NULL, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, NULL, GETOPT_VERSION_CHAR
#define case_GETOPT_HELP_CHAR                   \
  case GETOPT_HELP_CHAR:                        \
    usage(EXIT_SUCCESS);                        \
    break;
#define case_GETOPT_VERSION_CHAR(Program_name, Version, Authors)        \
  case GETOPT_VERSION_CHAR:                                             \
  fprintf(stdout, "%s version %0.3f\nCopyright (c) 2012 Vince Buffalo\n" \
          "Distributed under the GPL v2.0 License.\n",                  \
          Program_name, (double) Version);                              \
  exit(EXIT_SUCCESS);                                                   \
  break;
/* end code drawn from system.h */

#endif
