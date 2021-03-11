/* all platform-specific includes and defines go in this file */
#ifndef PLATFORM_H
#define PLATFORM_H

/* configurable options */
/* select your host type (or do it in the Makefile):*/
 // #define  UNIX_HOST
 // #define  WIN32  //(predefined on MSVC)
 

#define LARGE_INT_POWER_OF_TEN 1000000000   /* the largest power of ten which fits in an int on this architecture */
#if defined(__hppa__) || defined(__sparc__)
#define ALIGN_TYPE double                   /* the default data type to use for alignment */
#else
#define ALIGN_TYPE void *                   /* the default data type to use for alignment */
#endif

#define GLOBAL_TABLE_SIZE 97                /* global variable table */
#define STRING_TABLE_SIZE 97                /* shared string table size */
#define STRING_LITERAL_TABLE_SIZE 97        /* string literal table size */
#define RESERVED_WORD_TABLE_SIZE 97         /* reserved word table size */
#define PARAMETER_MAX 16                    /* maximum number of parameters to a function */
#define LINEBUFFER_MAX 256                  /* maximum number of characters on a line */
#define LOCAL_TABLE_SIZE 11                 /* size of local variable table (can expand) */
#define STRUCT_TABLE_SIZE 11                /* size of struct/union member table (can expand) */

#define INTERACTIVE_PROMPT_START "starting picoc " PICOC_VERSION "\n"
#define INTERACTIVE_PROMPT_STATEMENT "picoc> "
#define INTERACTIVE_PROMPT_LINE "     > "

/* host platform includes */
#ifdef UNIX_HOST
# define USE_MALLOC_STACK                   /* stack is allocated using malloc() */
# define USE_MALLOC_HEAP                    /* heap is allocated using malloc() */
# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
# include <assert.h>
# include <math.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdarg.h>
# include <setjmp.h>
# ifndef NO_FP
#  define PICOC_MATH_LIBRARY
#  undef BIG_ENDIAN
#  if defined(__powerpc__) || defined(__hppa__) || defined(__sparc__)
#   define BIG_ENDIAN
#  endif
# endif

extern jmp_buf ExitBuf;

#else
# ifdef WIN32
#  define USE_MALLOC_STACK                   /* stack is allocated using malloc() */
#  define USE_MALLOC_HEAP                    /* heap is allocated using malloc() */
#  include <stdio.h>
#  include <stdlib.h>
#  include <ctype.h>
#  include <string.h>
#  include <assert.h>
#  include <math.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <stdarg.h>
#  include <setjmp.h>
#  define PICOC_MATH_LIBRARY
#  undef BIG_ENDIAN

extern jmp_buf ExitBuf;

# endif
#endif

#endif /* PLATFORM_H */
