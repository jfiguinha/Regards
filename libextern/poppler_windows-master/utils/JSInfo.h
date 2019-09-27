//========================================================================
//
// JSInfo.h
//
// This file is licensed under the GPLv2 or later
//
// Copyright (C) 2013 Adrian Johnson <ajohnson@redneon.com>
// Copyright (C) 2016 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2018 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>. Work sponsored by the LiMux project of the city of Munich
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#ifndef JS_INFO_H
#define JS_INFO_H

#include <stdio.h>
#include "Object.h"
#include "PDFDoc.h"

#include "Link.h"
#include "UnicodeMap.h"

class PDFDoc;

class JSInfo {
public:

  // Constructor.
  JSInfo(PDFDoc *doc, int firstPage = 0);

  // Destructor.
  ~JSInfo();

  // scan for JS in the PDF
  void scanJS(int nPages);

  // scan and print JS in the PDF
  void scanJS(int nPages, FILE *fout, UnicodeMap *uMap);

  // return true if PDF contains JavaScript
  bool containsJS();

private:

  PDFDoc *doc;
  int currentPage;
  bool hasJS;
  bool print;
  FILE *file;
  UnicodeMap *uniMap;

  void scan(int nPages);
  void scanLinkAction(LinkAction *link, const char *action, bool deleteLink = true);
  void printJS(const GooString *js);

};

#endif
