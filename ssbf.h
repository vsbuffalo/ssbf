#ifndef PPGF_H
#define PPGF_H
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

typedef struct {
  gzFile **files;
  char **filenames;
  bloom_t **blooms;
} bloom_list_t;

int usage();


int index_usage();
int filter_usage();

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
