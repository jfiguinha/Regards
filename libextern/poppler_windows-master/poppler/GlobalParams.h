//========================================================================
//
// GlobalParams.h
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
// Copyright (C) 2005, 2007-2010, 2012, 2015, 2017, 2018 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2005 Jonathan Blandford <jrb@redhat.com>
// Copyright (C) 2006 Takashi Iwai <tiwai@suse.de>
// Copyright (C) 2006 Kristian Høgsberg <krh@redhat.com>
// Copyright (C) 2007 Krzysztof Kowalczyk <kkowalczyk@gmail.com>
// Copyright (C) 2009 Jonathan Kew <jonathan_kew@sil.org>
// Copyright (C) 2009 Petr Gajdos <pgajdos@novell.com>
// Copyright (C) 2009, 2011, 2012, 2014, 2015 William Bader <williambader@hotmail.com>
// Copyright (C) 2010 Hib Eris <hib@hiberis.nl>
// Copyright (C) 2011 Pino Toscano <pino@kde.org>
// Copyright (C) 2012, 2017 Adrian Johnson <ajohnson@redneon.com>
// Copyright (C) 2012 Thomas Freitag <Thomas.Freitag@alfa.de>
// Copyright (C) 2013 Jason Crain <jason@aquaticape.us>
// Copyright (C) 2018 Adam Reichold <adam.reichold@t-online.de>
// Copyright (C) 2019 Oliver Sander <oliver.sander@tu-dresden.de>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#ifndef GLOBALPARAMS_H
#define GLOBALPARAMS_H

#include <assert.h>
#include "poppler-config.h"
#include <stdio.h>
#include "CharTypes.h"
#include "UnicodeMap.h"
#include <unordered_map>
#include <string>
#include <mutex>
#include <vector>

class GooString;
class NameToCharCode;
class CharCodeToUnicode;
class CharCodeToUnicodeCache;
class UnicodeMapCache;
class CMap;
class CMapCache;
struct XpdfSecurityHandler;
class GlobalParams;
class GfxFont;
class Stream;
class SysFontList;

//------------------------------------------------------------------------

// The global parameters object.
extern GlobalParams *globalParams;

//------------------------------------------------------------------------

enum SysFontType {
  sysFontPFA,
  sysFontPFB,
  sysFontTTF,
  sysFontTTC
};

//------------------------------------------------------------------------

enum PSLevel {
  psLevel1,
  psLevel1Sep,
  psLevel2,
  psLevel2Sep,
  psLevel3,
  psLevel3Sep
};

//------------------------------------------------------------------------

enum EndOfLineKind {
  eolUnix,			// LF
  eolDOS,			// CR+LF
  eolMac			// CR
};

//------------------------------------------------------------------------

class GlobalParams {
public:

  // Initialize the global parameters
  GlobalParams(const char *customPopplerDataDir = nullptr);

  ~GlobalParams();

  GlobalParams(const GlobalParams &) = delete;
  GlobalParams& operator=(const GlobalParams &) = delete;

  void setupBaseFonts(char *dir);

  //----- accessors

  CharCode getMacRomanCharCode(char *charName);

  // Return Unicode values for character names.  Used for general text
  // extraction.
  Unicode mapNameToUnicodeText(const char *charName);

  // Return Unicode values for character names.  Used for glyph
  // lookups or text extraction with ZapfDingbats fonts.
  Unicode mapNameToUnicodeAll(const char *charName);

  UnicodeMap *getResidentUnicodeMap(const GooString *encodingName);
  FILE *getUnicodeMapFile(const GooString *encodingName);
  FILE *findCMapFile(const GooString *collection, const GooString *cMapName);
  FILE *findToUnicodeFile(const GooString *name);
  GooString *findFontFile(const GooString *fontName);
  GooString *findBase14FontFile(const GooString *base14Name, GfxFont *font);
  GooString *findSystemFontFile(GfxFont *font, SysFontType *type,
			      int *fontNum, GooString *substituteFontName = nullptr,
		              const GooString *base14Name = nullptr);
  bool getPSExpandSmaller();
  bool getPSShrinkLarger();
  PSLevel getPSLevel();
  GooString *getTextEncodingName();
  EndOfLineKind getTextEOL();
  bool getTextPageBreaks();
  bool getEnableFreeType();
  bool getOverprintPreview() { return overprintPreview; }
  bool getPrintCommands();
  bool getProfileCommands();
  bool getErrQuiet();

  CharCodeToUnicode *getCIDToUnicode(GooString *collection);
  UnicodeMap *getUnicodeMap(GooString *encodingName);
  CMap *getCMap(const GooString *collection, GooString *cMapName, Stream *stream = nullptr);
  UnicodeMap *getTextEncoding();

  std::vector<GooString*> *getEncodingNames();

  //----- functions to set parameters
  void addFontFile(GooString *fontName, GooString *path);
  void setPSExpandSmaller(bool expand);
  void setPSShrinkLarger(bool shrink);
  void setPSLevel(PSLevel level);
  void setTextEncoding(char *encodingName);
  bool setTextEOL(char *s);
  void setTextPageBreaks(bool pageBreaks);
  bool setEnableFreeType(char *s);
  void setOverprintPreview(bool overprintPreviewA);
  void setPrintCommands(bool printCommandsA);
  void setProfileCommands(bool profileCommandsA);
  void setErrQuiet(bool errQuietA);

  static bool parseYesNo2(const char *token, bool *flag);

private:

  void parseNameToUnicode(const GooString *name);
  UnicodeMap *getUnicodeMap2(GooString *encodingName);

  void scanEncodingDirs();
  void addCIDToUnicode(const GooString *collection, const GooString *fileName);
  void addUnicodeMap(const GooString *encodingName, const GooString *fileName);
  void addCMapDir(const GooString *collection, const GooString *dir);

  //----- static tables

  NameToCharCode *		// mapping from char name to
    macRomanReverseMap;		//   MacRomanEncoding index

  //----- user-modifiable settings

  NameToCharCode *		// mapping from char name to Unicode for ZapfDingbats
    nameToUnicodeZapfDingbats;
  NameToCharCode *		// mapping from char name to Unicode for text
    nameToUnicodeText;		// extraction
  // files for mappings from char collections
  // to Unicode, indexed by collection name
  std::unordered_map<std::string, std::string> cidToUnicodes;
  // mappings from Unicode to char codes,
  // indexed by encoding name
  std::unordered_map<std::string, UnicodeMap> residentUnicodeMaps;
  // files for mappings from Unicode to char
  // codes, indexed by encoding name
  std::unordered_map<std::string, std::string> unicodeMaps;
  // list of CMap dirs, indexed by collection
  std::unordered_multimap<std::string, std::string> cMapDirs;
  std::vector<GooString*> *toUnicodeDirs;		// list of ToUnicode CMap dirs
  bool baseFontsInitialized;
#ifdef _WIN32
  // windows font substitutes (for CID fonts)
  std::unordered_map<std::string, std::string> substFiles;
#endif
  // font files: font name mapped to path
  std::unordered_map<std::string, std::string> fontFiles;
  SysFontList *sysFonts;	// system fonts
  bool psExpandSmaller;	// expand smaller pages to fill paper
  bool psShrinkLarger;		// shrink larger pages to fit paper
  PSLevel psLevel;		// PostScript level to generate
  GooString *textEncoding;	// encoding (unicodeMap) to use for text
				//   output
  EndOfLineKind textEOL;	// type of EOL marker to use for text
				//   output
  bool textPageBreaks;		// insert end-of-page markers?
  bool enableFreeType;		// FreeType enable flag
  bool overprintPreview;	// enable overprint preview
  bool printCommands;		// print the drawing commands
  bool profileCommands;	// profile the drawing commands
  bool errQuiet;		// suppress error messages?

  CharCodeToUnicodeCache *cidToUnicodeCache;
  CharCodeToUnicodeCache *unicodeToUnicodeCache;
  UnicodeMapCache *unicodeMapCache;
  CMapCache *cMapCache;
  
  mutable std::recursive_mutex mutex;
  mutable std::recursive_mutex unicodeMapCacheMutex;
  mutable std::recursive_mutex cMapCacheMutex;

  const char *popplerDataDir;
};

#endif
