/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2016-2018 Roman Lebedev

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#if defined(__SSE2__)
#define WITH_SSE2
#else
/* #undef WITH_SSE2 */
#endif

static constexpr const auto RAWSPEED_CACHELINESIZE = 64ULL;
static_assert(RAWSPEED_CACHELINESIZE > 0 && ((RAWSPEED_CACHELINESIZE & (RAWSPEED_CACHELINESIZE - 1)) == 0), "Expected to know CPU L1d cache line size.");

static constexpr const auto RAWSPEED_PAGESIZE = 64ULL;
static_assert(RAWSPEED_PAGESIZE > 0 && ((RAWSPEED_PAGESIZE & (RAWSPEED_PAGESIZE - 1)) == 0), "Expected to know (minimal) CPU page size.");

#define HAVE_PUGIXML

/* #undef HAVE_OPENMP */

/* #undef HAVE_PUGIXML */
#define HAVE_OPENMP
#define HAVE_ZLIB
#define HAVE_JPEG
/* #undef HAVE_JPEG_MEM_SRC */

#define HAVE_CXX_THREAD_LOCAL
/* #undef HAVE_GCC_THREAD_LOCAL */

// which aligned memory allocation function is available?
// only the first one found will be enabled
//#define HAVE_ALIGNED_ALLOC
/* #undef HAVE_POSIX_MEMALIGN */
#define HAVE_ALIGNED_MALLOC

#define RAWSPEED_STANDALONE_BUILD
#ifdef RAWSPEED_STANDALONE_BUILD
#define RAWSPEED_SOURCE_DIR "C:/developpement/git/Regards/libextern/rawspeed-3.0"
#else
// If rawspeed is being built as part of some larger build, we can not retain
// the RAWSPEED_SOURCE_DIR, because that would affect the reproducible builds.
#endif


#ifdef HAVE_OPENMP
/* #undef OPENMP_SHARED_CLAUSE_IS_BROKEN_FOR_CONST_VARIABLES */
#ifdef OPENMP_SHARED_CLAUSE_IS_BROKEN_FOR_CONST_VARIABLES
// See https://godbolt.org/z/AiyuX9
#define OMPSHAREDCLAUSE(...)
#else
#define OMPSHAREDCLAUSE(...) shared(__VA_ARGS__)
#endif
#undef OPENMP_SHARED_CLAUSE_IS_BROKEN_FOR_CONST_VARIABLES
#endif // HAVE_OPENMP

// see http://clang.llvm.org/docs/LanguageExtensions.html
#ifndef __has_feature      // Optional of course.
#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif
#ifndef __has_extension
#define __has_extension __has_feature // Compatibility with pre-3.0 compilers.
#endif

#define RAWSPEED_UNLIKELY_FUNCTION __attribute__((cold))
#define RAWSPEED_NOINLINE __attribute__((noinline))