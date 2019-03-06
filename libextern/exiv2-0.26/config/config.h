/* ./config/config.h.  Generated from config.h.in by configure.  */
#pragma once
#ifndef __CONFIG__H__
#define __CONFIG__H__

/* Define to 1 if you want to use libssh */
/* #undef USE_SSH */

/* Define to 1 if you want to use libcurl in httpIo */
/* #undef USE_CURL */

/* Define to 1 if you have the `alarm' function. */
/* #undef HAVE_ALARM */

/* Define to 1 if you have the declaration of `strerror_r', and to 0 if you
   don't. */
#define HAVE_DECL_STRERROR_R 0

/* Define to 1 if you have the `gmtime_r' function. */
/* #undef HAVE_GMTIME_R */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <libintl.h> header file. */
#define HAVE_LIBINTL_H 1

#ifndef EXV_COMMERCIAL_VERSION

/* Define to 1 to enable translation of Nikon lens names. */
#define HAVE_LENSDATA 1

/* Define to 1 if translation of program messages to the user's
   native language is requested. */
#define ENABLE_NLS 1

#endif /* !EXV_COMMERCIAL_VERSION */

/* Define to 1 to include video code in the library */
#define ENABLE_VIDEO 1

/* Define to 1 to include webready code in the library */
/* #undef ENABLE_WEBREADY */

/* Define to 1 if you have the `iconv' function. */
#define HAVE_ICONV 1

/* Define to `const' or to empty, depending on the second argument of `iconv'. */
#define ICONV_CONST 

/* Define to 1 if you have the <libintl.h> header file. */
#define HAVE_LIBINTL_H 1

/* Define to 1 if you have the <regex.h> header file. */
#define HAVE_REGEX 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
/* #undef HAVE_MALLOC */

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the `lstat' function. */
/* #undef HAVE_LSTAT */

/* Define to 1 if your system has a GNU libc compatible `realloc' function,
   and to 0 otherwise. */
/* #undef HAVE_REALLOC */

/* Define to 1 if stdbool.h conforms to C99. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the `strerror_r' function. */
/* #undef HAVE_STRERROR_R */

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL 1

/* Define to 1 if you have the `mmap' function. */
/* #undef HAVE_MMAP */

/* Define to 1 if you have the `munmap' function. */
/* #undef HAVE_MUNMAP */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
/* #undef HAVE_SYS_TIME_H */

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `timegm' function. */
/* #undef HAVE_TIMEGM */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vprintf' function. */
/* #undef HAVE_VPRINTF */

/* Define to 1 if you have the <wchar.h> header file. */
/* #undef HAVE_WCHAR_H */

/* Define to 1 if you have the <sys/mman.h> header file. */
/* #undef HAVE_SYS_MMAN_H */

/* Define to 1 if you have the `zlib' library. */
#define HAVE_LIBZ 1

/* Define to 1 if you have the Adobe XMP Toolkit. */
#define HAVE_XMP_TOOLKIT 1

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define to 1 if `lstat' dereferences a symlink specified with a trailing
   slash. */
/* #undef LSTAT_FOLLOWS_SLASHED_SYMLINK */

/* Define if C++ visibility support is enabled */
/* #undef WANT_VISIBILITY_SUPPORT */

/* Define if we have / are building a shared library (DLL) */
/* #undef HAVE_DLL */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "ahuggel@gmx.net"

/* Define to the name of this package. */
#define PACKAGE "exiv2"

/* Define to the full name of this package. */
#define PACKAGE_NAME "exiv2"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "exiv2 0.26"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "exiv2"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.26"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if strerror_r returns char *. */
/* #undef STRERROR_R_CHAR_P */

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
/* #undef TIME_WITH_SYS_TIME */

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to rpl_realloc if the replacement function should be used. */
/* #undef realloc */

/* Define to `unsigned' if <sys/types.h> does not define. */
/* #undef size_t */

#endif
