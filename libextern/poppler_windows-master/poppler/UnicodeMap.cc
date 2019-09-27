//========================================================================
//
// UnicodeMap.cc
//
// Copyright 2001-2003 Glyph & Cog, LLC
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// Copyright (C) 2010 Jakub Wilk <jwilk@jwilk.net>
// Copyright (C) 2017, 2018 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2017 Adrian Johnson <ajohnson@redneon.com>
// Copyright (C) 2017 Jean Ghali <jghali@libertysurf.fr>
// Copyright (C) 2018 Adam Reichold <adam.reichold@t-online.de>
// Copyright (C) 2019 Oliver Sander <oliver.sander@tu-dresden.de>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#include <config.h>

#include <stdio.h>
#include <string.h>
#include "goo/glibc.h"
#include "goo/gmem.h"
#include "goo/gfile.h"
#include "goo/GooString.h"
#include "Error.h"
#include "GlobalParams.h"
#include "UnicodeMap.h"

//------------------------------------------------------------------------

#define maxExtCode 16

struct UnicodeMapExt {
  Unicode u;			// Unicode char
  char code[maxExtCode];
  unsigned int nBytes;
};

//------------------------------------------------------------------------

UnicodeMap *UnicodeMap::parse(GooString *encodingNameA) {
  FILE *f;
  UnicodeMap *map;
  UnicodeMapRange *range;
  UnicodeMapExt *eMap;
  int size, eMapsSize;
  char buf[256];
  int line, nBytes, i;
  char *tok1, *tok2, *tok3;
  char *tokptr;

  if (!(f = globalParams->getUnicodeMapFile(encodingNameA))) {
    error(errSyntaxError, -1,
	  "Couldn't find unicodeMap file for the '{0:t}' encoding",
	  encodingNameA);
    return nullptr;
  }

  map = new UnicodeMap(encodingNameA->copy());

  size = 8;
  map->ranges = (UnicodeMapRange *)gmallocn(size, sizeof(UnicodeMapRange));
  eMapsSize = 0;

  line = 1;
  while (getLine(buf, sizeof(buf), f)) {
    if ((tok1 = strtok_r(buf, " \t\r\n", &tokptr)) &&
	(tok2 = strtok_r(nullptr, " \t\r\n", &tokptr))) {
      if (!(tok3 = strtok_r(nullptr, " \t\r\n", &tokptr))) {
	tok3 = tok2;
	tok2 = tok1;
      }
      nBytes = strlen(tok3) / 2;
      if (nBytes <= 4) {
	if (map->len == size) {
	  size *= 2;
	  map->ranges = (UnicodeMapRange *)
	    greallocn(map->ranges, size, sizeof(UnicodeMapRange));
	}
	range = &map->ranges[map->len];
	sscanf(tok1, "%x", &range->start);
	sscanf(tok2, "%x", &range->end);
	sscanf(tok3, "%x", &range->code);
	range->nBytes = nBytes;
	++map->len;
      } else if (tok2 == tok1) {
	if (map->eMapsLen == eMapsSize) {
	  eMapsSize += 16;
	  map->eMaps = (UnicodeMapExt *)
	    greallocn(map->eMaps, eMapsSize, sizeof(UnicodeMapExt));
	}
	eMap = &map->eMaps[map->eMapsLen];
	sscanf(tok1, "%x", &eMap->u);
	for (i = 0; i < nBytes; ++i) {
	  unsigned int x;
	  sscanf(tok3 + i*2, "%2x", &x);
	  eMap->code[i] = (char)x;
	}
	eMap->nBytes = nBytes;
	++map->eMapsLen;
      } else {
	error(errSyntaxError, -1,
	      "Bad line ({0:d}) in unicodeMap file for the '{1:t}' encoding",
	      line, encodingNameA);
      }
    } else {
      error(errSyntaxError, -1,
	    "Bad line ({0:d}) in unicodeMap file for the '{1:t}' encoding",
	    line, encodingNameA);
    }
    ++line;
  }

  fclose(f);

  return map;
}

UnicodeMap::UnicodeMap(GooString *encodingNameA) {
  encodingName = encodingNameA;
  unicodeOut = false;
  kind = unicodeMapUser;
  ranges = nullptr;
  len = 0;
  eMaps = nullptr;
  eMapsLen = 0;
  refCnt = 1;
}

UnicodeMap::UnicodeMap(const char *encodingNameA, bool unicodeOutA,
		       UnicodeMapRange *rangesA, int lenA) {
  encodingName = new GooString(encodingNameA);
  unicodeOut = unicodeOutA;
  kind = unicodeMapResident;
  ranges = rangesA;
  len = lenA;
  eMaps = nullptr;
  eMapsLen = 0;
  refCnt = 1;
}

UnicodeMap::UnicodeMap(const char *encodingNameA, bool unicodeOutA,
		       UnicodeMapFunc funcA) {
  encodingName = new GooString(encodingNameA);
  unicodeOut = unicodeOutA;
  kind = unicodeMapFunc;
  func = funcA;
  eMaps = nullptr;
  eMapsLen = 0;
  refCnt = 1;
}

UnicodeMap::~UnicodeMap() {
  delete encodingName;
  if (kind == unicodeMapUser && ranges) {
    gfree(ranges);
  }
  if (eMaps) {
    gfree(eMaps);
  }
}

UnicodeMap::UnicodeMap(UnicodeMap &&other) noexcept
  : encodingName{other.encodingName}
  , kind{other.kind}
  , unicodeOut{other.unicodeOut}
  , len{other.len}
  , eMaps{other.eMaps}
  , eMapsLen{other.eMapsLen}
  , refCnt{1}
{
  switch (kind) {
  case unicodeMapUser:
  case unicodeMapResident:
    ranges = other.ranges;
    other.ranges = nullptr;
    break;
  case unicodeMapFunc:
    func = other.func;
    break;
  }
  other.encodingName = nullptr;
  other.eMaps = nullptr;
}

UnicodeMap& UnicodeMap::operator=(UnicodeMap &&other) noexcept
{
  if (this != &other)
    swap(other);
  return *this;
}

void UnicodeMap::swap(UnicodeMap &other) noexcept
{
  using std::swap;
  swap(encodingName, other.encodingName);
  swap(unicodeOut, other.unicodeOut);
  switch (kind) {
  case unicodeMapUser:
  case unicodeMapResident:
    switch (other.kind) {
    case unicodeMapUser:
    case unicodeMapResident:
      swap(ranges, other.ranges);
      break;
    case unicodeMapFunc:
    {
      const auto tmp = ranges;
      func = other.func;
      other.ranges = tmp;
      break;
    }
    }
    break;
  case unicodeMapFunc:
    switch (other.kind) {
    case unicodeMapUser:
    case unicodeMapResident:
    {
      const auto tmp = func;
      ranges = other.ranges;
      other.func = tmp;
      break;
    }
    case unicodeMapFunc:
      swap(func, other.func);
      break;
    }
    break;
  }
  swap(kind, other.kind);
  swap(len, other.len);
  swap(eMaps, other.eMaps);
  swap(eMapsLen, other.eMapsLen);
}

void UnicodeMap::incRefCnt() {
  refCnt.fetch_add(1);
}

void UnicodeMap::decRefCnt() {
  if (refCnt.fetch_sub(1) == 1) {
    delete this;
  }
}

bool UnicodeMap::match(const GooString *encodingNameA) const {
  return !encodingName->cmp(encodingNameA);
}

int UnicodeMap::mapUnicode(Unicode u, char *buf, int bufSize) {
  int a, b, m, n, i, j;
  unsigned int code;

  if (kind == unicodeMapFunc) {
    return (*func)(u, buf, bufSize);
  }

  a = 0;
  b = len;
  if (u >= ranges[a].start) {
    // invariant: ranges[a].start <= u < ranges[b].start
    while (b - a > 1) {
      m = (a + b) / 2;
      if (u >= ranges[m].start) {
	a = m;
      } else if (u < ranges[m].start) {
	b = m;
      }
    }
    if (u <= ranges[a].end) {
      n = ranges[a].nBytes;
      if (n > bufSize) {
	return 0;
      }
      code = ranges[a].code + (u - ranges[a].start);
      for (i = n - 1; i >= 0; --i) {
	buf[i] = (char)(code & 0xff);
	code >>= 8;
      }
      return n;
    }
  }

  for (i = 0; i < eMapsLen; ++i) {
    if (eMaps[i].u == u) {
      n = eMaps[i].nBytes;
      for (j = 0; j < n; ++j) {
	buf[j] = eMaps[i].code[j];
      }
      return n;
    }
  }

  return 0;
}

//------------------------------------------------------------------------

UnicodeMapCache::UnicodeMapCache() {
  int i;

  for (i = 0; i < unicodeMapCacheSize; ++i) {
    cache[i] = nullptr;
  }
}

UnicodeMapCache::~UnicodeMapCache() {
  int i;

  for (i = 0; i < unicodeMapCacheSize; ++i) {
    if (cache[i]) {
      cache[i]->decRefCnt();
    }
  }
}

UnicodeMap *UnicodeMapCache::getUnicodeMap(GooString *encodingName) {
  UnicodeMap *map;
  int i, j;

  if (cache[0] && cache[0]->match(encodingName)) {
    cache[0]->incRefCnt();
    return cache[0];
  }
  for (i = 1; i < unicodeMapCacheSize; ++i) {
    if (cache[i] && cache[i]->match(encodingName)) {
      map = cache[i];
      for (j = i; j >= 1; --j) {
	cache[j] = cache[j - 1];
      }
      cache[0] = map;
      map->incRefCnt();
      return map;
    }
  }
  if ((map = UnicodeMap::parse(encodingName))) {
    if (cache[unicodeMapCacheSize - 1]) {
      cache[unicodeMapCacheSize - 1]->decRefCnt();
    }
    for (j = unicodeMapCacheSize - 1; j >= 1; --j) {
      cache[j] = cache[j - 1];
    }
    cache[0] = map;
    map->incRefCnt();
    return map;
  }
  return nullptr;
}
