//========================================================================
//
// StdinPDFDocBuilder.cc
//
// This file is licensed under the GPLv2 or later
//
// Copyright 2010 Hib Eris <hib@hiberis.nl>
// Copyright 2010, 2017 Albert Astals Cid <aacid@kde.org>
//
//========================================================================

#include <config.h>

#include "StdinPDFDocBuilder.h"
#include "CachedFile.h"
#include "StdinCachedFile.h"

//------------------------------------------------------------------------
// StdinPDFDocBuilder
//------------------------------------------------------------------------

PDFDoc *
StdinPDFDocBuilder::buildPDFDoc(const GooString &uri, GooString *ownerPassword,
                                    GooString *userPassword, void *guiDataA)
{
  CachedFile *cachedFile = new CachedFile(new StdinCacheLoader(), nullptr);
  return new PDFDoc(new CachedFileStream(cachedFile, 0, false,
                                         cachedFile->getLength(), Object(objNull)),
                    ownerPassword, userPassword);
}

bool StdinPDFDocBuilder::supports(const GooString &uri)
{
  if (uri.cmpN("fd://0", 6) == 0) {
    return true;
  } else {
    return false;
  }
}

