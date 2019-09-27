//========================================================================
//
// FlateEncoder.h
//
// Copyright (C) 2016, William Bader <williambader@hotmail.com>
// Copyright (C) 2018 Albert Astals Cid <aacid@kde.org>
//
// This file is under the GPLv2 or later license
//
//========================================================================

#ifndef FLATEENCODE_H
#define FLATEENCODE_H

#include "poppler-config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <ctype.h>
#include "goo/gmem.h"
#include "goo/gfile.h"
#include "Error.h"
#include "Object.h"
#include "Decrypt.h"

#include "Stream.h"

extern "C" {
#include <zlib.h>
}

//------------------------------------------------------------------------
// FlateEncoder
//------------------------------------------------------------------------

class FlateEncoder: public FilterStream {
public:

  FlateEncoder(Stream *strA);
  ~FlateEncoder();
  StreamKind getKind() override { return strWeird; }
  void reset() override;
  int getChar() override
    { return (outBufPtr >= outBufEnd && !fillBuf()) ? EOF : (*outBufPtr++ & 0xff); }
  int lookChar() override
    { return (outBufPtr >= outBufEnd && !fillBuf()) ? EOF : (*outBufPtr & 0xff); }
  GooString *getPSFilter(int psLevel, const char *indent) override { return nullptr; }
  bool isBinary(bool last = true) override { return true; }
  bool isEncoder() override { return true; }

private:

  static const int inBufSize = 16384;
  static const int outBufSize = inBufSize;
  unsigned char inBuf[ inBufSize ];
  unsigned char outBuf[ outBufSize ];
  unsigned char *outBufPtr;
  unsigned char *outBufEnd;
  bool inBufEof;
  bool outBufEof;
  z_stream zlib_stream;

  bool fillBuf();
};

#endif
