#include <header.h>
#include "ExportPDF.h"

#include "config.h"
#include <poppler-config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include <time.h>
#include "parseargs.h"
#include "goo/GooString.h"
#include "goo/gbase64.h"
#include "goo/gbasename.h"
#include "goo/gmem.h"
#include "Object.h"
#include "Stream.h"
#include "Array.h"
#include "Dict.h"
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"
#include "Outline.h"
#include "PDFDoc.h"
#include "PDFDocFactory.h"
#include "HtmlOutputDev.h"
#ifdef HAVE_SPLASH
#include "SplashOutputDev.h"
#include "splash/SplashBitmap.h"
#endif
#include "GlobalParams.h"
#include "PDFDocEncoding.h"
#include "Error.h"
#include "DateInfo.h"
#include "goo/gfile.h"
#include "InMemoryFile.h"
#include "TextOutputDev.h"


static double resolution = 72.0;
static int x = 0;
static int y = 0;
static int w = 0;
static int h = 0;
static bool bbox = false;
static bool bboxLayout = false;
static bool physLayout = false;
static double fixedPitch = 0;
static bool discardDiag = false;
static bool htmlMeta = false;
static char textEOL[16] = "";
static bool noPageBreaks = false;
static bool quiet = false;
static bool printEnc = false;


static int firstPage = 1;
static int lastPage = 0;
static bool rawOrder = true;
bool printCommands = true;
static bool printHelp = false;
bool printHtml = false;
bool complexMode = false;
bool singleHtml = false; // singleHtml
bool dataUrls = false;
bool localignore = false;
static char extension[5] = "png";
static double scale = 1.5;
bool noframes = false;
bool stout = false;
bool xml = false;
bool noRoundedCoordinates = false;
static bool errQuiet = false;
static bool noDrm = false;
double wordBreakThreshold = 10;  // 10%, below converted into a coefficient - 0.1

bool showHidden = false;
bool noMerge = false;
bool fontFullName = false;
static char ownerPassword[33] = "";
static char userPassword[33] = "";
static bool printVersion = false;

static GooString* getInfoString(Dict *infoDict, const char *key);
static GooString* getInfoDate(Dict *infoDict, const char *key);

static void printInfoString(FILE *f, Dict *infoDict, const char *key,
	const char *text1, const char *text2, UnicodeMap *uMap);
static void printInfoDate(FILE *f, Dict *infoDict, const char *key, const char *fmt);
void printDocBBox(FILE *f, PDFDoc *doc, TextOutputDev *textOut, int first, int last);
void printWordBBox(FILE *f, PDFDoc *doc, TextOutputDev *textOut, int first, int last);

static char textEncName[128] = "";

static const ArgDesc argDesc[] = {
  {"-f",      argInt,      &firstPage,     0,
   "first page to convert"},
  {"-l",      argInt,      &lastPage,      0,
   "last page to convert"},
   /*{"-raw",    argFlag,     &rawOrder,      0,
	 "keep strings in content stream order"},*/
   {"-q",      argFlag,     &errQuiet,      0,
	"don't print any messages or errors"},
   {"-h",      argFlag,     &printHelp,     0,
	"print usage information"},
   {"-?",      argFlag,     &printHelp,     0,
	"print usage information"},
   {"-help",   argFlag,     &printHelp,     0,
	"print usage information"},
   {"--help",  argFlag,     &printHelp,     0,
	"print usage information"},
   {"-p",      argFlag,     &printHtml,     0,
	"exchange .pdf links by .html"},
   {"-c",      argFlag,     &complexMode,          0,
	"generate complex document"},
   {"-s",      argFlag,     &singleHtml,          0,
	"generate single document that includes all pages"},
 #ifdef HAVE_IN_MEMORY_FILE
   {"-dataurls", argFlag,   &dataUrls,      0,
	"use data URLs instead of external images in HTML"},
 #endif
   {"-i",      argFlag,     &localignore,        0,
	"ignore images"},
   {"-noframes", argFlag,   &noframes,      0,
	"generate no frames"},
   {"-stdout"  ,argFlag,    &stout,         0,
	"use standard output"},
   {"-zoom",   argFP,    &scale,         0,
	"zoom the pdf document (default 1.5)"},
   {"-xml",    argFlag,    &xml,         0,
	"output for XML post-processing"},
   {"-noroundcoord", argFlag, &noRoundedCoordinates, 0,
	 "do not round coordinates (with XML output only)"},
   {"-hidden", argFlag,   &showHidden,   0,
	"output hidden text"},
   {"-nomerge", argFlag, &noMerge, 0,
	"do not merge paragraphs"},
   {"-enc",    argString,   textEncName,    sizeof(textEncName),
	"output text encoding name"},
   {"-fmt",    argString,   extension,      sizeof(extension),
	"image file format for Splash output (png or jpg)"},
   {"-v",      argFlag,     &printVersion,  0,
	"print copyright and version info"},
   {"-opw",    argString,   ownerPassword,  sizeof(ownerPassword),
	"owner password (for encrypted files)"},
   {"-upw",    argString,   userPassword,   sizeof(userPassword),
	"user password (for encrypted files)"},
   {"-nodrm", argFlag, &noDrm, 0,
	"override document DRM settings"},
   {"-wbt",    argFP,    &wordBreakThreshold, 0,
	"word break threshold (default 10 percent)"},
   {"-fontfullname", argFlag, &fontFullName, 0,
	"outputs font full name"},
   {}
};


#ifdef HAVE_SPLASH
class SplashOutputDevNoText : public SplashOutputDev {
public:
	SplashOutputDevNoText(SplashColorMode colorModeA, int bitmapRowPadA,
		bool reverseVideoA, SplashColorPtr paperColorA,
		bool bitmapTopDownA = true) : SplashOutputDev(colorModeA,
			bitmapRowPadA, reverseVideoA, paperColorA, bitmapTopDownA) { }
	virtual ~SplashOutputDevNoText() { }

	void drawChar(GfxState *state, double x, double y,
		double dx, double dy,
		double originX, double originY,
		CharCode code, int nBytes, Unicode *u, int uLen) override { }
	bool beginType3Char(GfxState *state, double x, double y,
		double dx, double dy,
		CharCode code, Unicode *u, int uLen) override {
		return false;
	}
	void endType3Char(GfxState *state) override { }
	void beginTextObject(GfxState *state) override { }
	void endTextObject(GfxState *state) override { }
	bool interpretType3Chars() override { return false; }
};
#endif

CExportPDF::CExportPDF()
{

}

CExportPDF::~CExportPDF()
{

}

int CExportPDF::ExportPDFToHTML(wxString pdfFile, wxString fileout)
{
	PDFDoc *doc = nullptr;
	GooString *fileName = nullptr;
	GooString *docTitle = nullptr;
	GooString *author = nullptr, *keywords = nullptr, *subject = nullptr, *date = nullptr;
	GooString *htmlFileName = nullptr;
	HtmlOutputDev *htmlOut = nullptr;
#ifdef HAVE_SPLASH
	SplashOutputDev *splashOut = nullptr;
#endif
	bool doOutline;
	bool ok = true;
	GooString *ownerPW, *userPW;
	Object info;
	int exit_status = EXIT_FAILURE;

	// init error file
	//errorInit();

	// read config file
	globalParams = new GlobalParams();

	try
	{
		if (errQuiet) {
			globalParams->setErrQuiet(errQuiet);
			printCommands = false; // I'm not 100% what is the difference between them
		}

		if (textEncName[0]) {
			globalParams->setTextEncoding(textEncName);
			if (!globalParams->getTextEncoding()) {
				throw("error");
			}
		}

		// convert from user-friendly percents into a coefficient
		wordBreakThreshold /= 100.0;

		// open PDF file
		if (ownerPassword[0]) {
			ownerPW = new GooString(ownerPassword);
		}
		else {
			ownerPW = nullptr;
		}
		if (userPassword[0]) {
			userPW = new GooString(userPassword);
		}
		else {
			userPW = nullptr;
		}

		fileName = new GooString(pdfFile.ToStdString());

		if (fileName->cmp("-") == 0) {
			delete fileName;
			fileName = new GooString("fd://0");
		}

		doc = PDFDocFactory().createPDFDoc(*fileName, ownerPW, userPW);

		if (userPW) {
			delete userPW;
		}
		if (ownerPW) {
			delete ownerPW;
		}
		if (!doc->isOk()) {
			throw("error");
		}

		// check for copy permission
		if (!doc->okToCopy()) {
			if (!noDrm) {
				error(errNotAllowed, -1, "Copying of text from this document is not allowed.");
				throw("error");
			}
			fprintf(stderr, "Document has copy-protection bit set.\n");
		}

		// construct text file name
		GooString* tmp = new GooString(fileout.ToStdString());
		if (!xml) {
			if (tmp->getLength() >= 5) {
				const char *p = tmp->c_str() + tmp->getLength() - 5;
				if (!strcmp(p, ".html") || !strcmp(p, ".HTML")) {
					htmlFileName = new GooString(tmp->c_str(), tmp->getLength() - 5);
				}
			}
		}
		else {
			if (tmp->getLength() >= 4) {
				const char *p = tmp->c_str() + tmp->getLength() - 4;
				if (!strcmp(p, ".xml") || !strcmp(p, ".XML")) {
					htmlFileName = new GooString(tmp->c_str(), tmp->getLength() - 4);
				}
			}
		}
		if (!htmlFileName) {
			htmlFileName = new GooString(tmp);
		}
		delete tmp;


		if (scale > 3.0) scale = 3.0;
		if (scale < 0.5) scale = 0.5;

		if (complexMode) {
			//noframes=false;
			stout = false;
		}

		if (stout) {
			noframes = true;
			complexMode = false;
		}

		if (xml)
		{
			complexMode = true;
			singleHtml = false;
			noframes = true;
			noMerge = true;
		}

		// get page range
		if (firstPage < 1)
			firstPage = 1;
		if (lastPage < 1 || lastPage > doc->getNumPages())
			lastPage = doc->getNumPages();
		if (lastPage < firstPage) {
			error(errCommandLine, -1,
				"Wrong page range given: the first page ({0:d}) can not be after the last page ({1:d}).",
				firstPage, lastPage);
			throw(errCommandLine);
		}

		info = doc->getDocInfo();
		if (info.isDict()) {
			docTitle = getInfoString(info.getDict(), "Title");
			author = getInfoString(info.getDict(), "Author");
			keywords = getInfoString(info.getDict(), "Keywords");
			subject = getInfoString(info.getDict(), "Subject");
			date = getInfoDate(info.getDict(), "ModDate");
			if (!date)
				date = getInfoDate(info.getDict(), "CreationDate");
		}
		if (!docTitle) docTitle = new GooString(htmlFileName);

		if (!singleHtml)
			rawOrder = complexMode; // todo: figure out what exactly rawOrder do :)
		else
			rawOrder = singleHtml;

		doOutline = doc->getOutline()->getItems() != nullptr;
		// write text file
		htmlOut = new HtmlOutputDev(doc->getCatalog(), htmlFileName->c_str(),
			docTitle->c_str(),
			author ? author->c_str() : nullptr,
			keywords ? keywords->c_str() : nullptr,
			subject ? subject->c_str() : nullptr,
			date ? date->c_str() : nullptr,
			rawOrder,
			firstPage,
			doOutline);
		delete docTitle;
		if (author)
		{
			delete author;
		}
		if (keywords)
		{
			delete keywords;
		}
		if (subject)
		{
			delete subject;
		}
		if (date)
		{
			delete date;
		}

		if ((complexMode || singleHtml) && !xml && !localignore) {
#ifdef HAVE_SPLASH
			GooString *imgFileName = nullptr;
			// White paper color
			SplashColor color;
			color[0] = color[1] = color[2] = 255;
			// If the user specified "jpg" use JPEG, otherwise PNG
			SplashImageFileFormat format = strcmp(extension, "jpg") ?
				splashFormatPng : splashFormatJpeg;

			splashOut = new SplashOutputDevNoText(splashModeRGB8, 4, false, color);
			splashOut->startDoc(doc);

			for (int pg = firstPage; pg <= lastPage; ++pg) {
				InMemoryFile imf;
				doc->displayPage(splashOut, pg,
					72 * scale, 72 * scale,
					0, true, false, false);
				SplashBitmap *bitmap = splashOut->getBitmap();

				imgFileName = GooString::format("{0:s}{1:03d}.{2:s}",
					htmlFileName->c_str(), pg, extension);
				auto f1 = dataUrls ? imf.open("wb") : fopen(imgFileName->c_str(), "wb");
				if (!f1) {
					fprintf(stderr, "Could not open %s\n", imgFileName->c_str());
					delete imgFileName;
					continue;
				}
				bitmap->writeImgFile(format, f1, 72 * scale, 72 * scale);
				fclose(f1);
				if (dataUrls) {
					htmlOut->addBackgroundImage(
						std::string((format == splashFormatJpeg) ? "data:image/jpeg;base64," : "data:image/png;base64,") +
						gbase64Encode(imf.getBuffer())
					);
				}
				else {
					htmlOut->addBackgroundImage(gbasename(imgFileName->c_str()));
				}
				delete imgFileName;
			}

			delete splashOut;
#else
			fprintf(stderr, "Your pdftohtml was built without splash backend support. It is needed for the option you want to use.\n");
			delete htmlOut;
			delete htmlFileName;
			delete globalParams;
			delete fileName;
			delete doc;
			return -1;
#endif
		}

		if (htmlOut->isOk())
		{
			doc->displayPages(htmlOut, firstPage, lastPage, 72 * scale, 72 * scale, 0,
				true, false, false);
			htmlOut->dumpDocOutline(doc);
		}

		delete htmlOut;


		exit_status = EXIT_SUCCESS;
	}
	catch (...)
	{

	}

	// clean up
	if (doc) delete doc;
	delete fileName;
	if (globalParams) delete globalParams;

	if (htmlFileName) delete htmlFileName;

	return exit_status;
}

static GooString* getInfoString(Dict *infoDict, const char *key) {
	Object obj;
	// Raw value as read from PDF (may be in pdfDocEncoding or UCS2)
	const GooString *rawString;
	// Value converted to unicode
	Unicode *unicodeString;
	int unicodeLength;
	// Value HTML escaped and converted to desired encoding
	GooString *encodedString = nullptr;
	// Is rawString UCS2 (as opposed to pdfDocEncoding)
	bool isUnicode;

	obj = infoDict->lookup(key);
	if (obj.isString()) {
		rawString = obj.getString();

		// Convert rawString to unicode
		if (rawString->hasUnicodeMarker()) {
			isUnicode = true;
			unicodeLength = (obj.getString()->getLength() - 2) / 2;
		}
		else {
			isUnicode = false;
			unicodeLength = obj.getString()->getLength();
		}
		unicodeString = new Unicode[unicodeLength];

		for (int i = 0; i < unicodeLength; i++) {
			if (isUnicode) {
				unicodeString[i] = ((rawString->getChar((i + 1) * 2) & 0xff) << 8) |
					(rawString->getChar(((i + 1) * 2) + 1) & 0xff);
			}
			else {
				unicodeString[i] = pdfDocEncoding[rawString->getChar(i) & 0xff];
			}
		}

		// HTML escape and encode unicode
		encodedString = HtmlFont::HtmlFilter(unicodeString, unicodeLength);
		delete[] unicodeString;
	}

	return encodedString;
}

static GooString* getInfoDate(Dict *infoDict, const char *key) {
	Object obj;
	const char *s;
	int year, mon, day, hour, min, sec, tz_hour, tz_minute;
	char tz;
	struct tm tmStruct;
	GooString *result = nullptr;
	char buf[256];

	obj = infoDict->lookup(key);
	if (obj.isString()) {
		s = obj.getString()->c_str();
		// TODO do something with the timezone info
		if (parseDateString(s, &year, &mon, &day, &hour, &min, &sec, &tz, &tz_hour, &tz_minute)) {
			tmStruct.tm_year = year - 1900;
			tmStruct.tm_mon = mon - 1;
			tmStruct.tm_mday = day;
			tmStruct.tm_hour = hour;
			tmStruct.tm_min = min;
			tmStruct.tm_sec = sec;
			tmStruct.tm_wday = -1;
			tmStruct.tm_yday = -1;
			tmStruct.tm_isdst = -1;
			mktime(&tmStruct); // compute the tm_wday and tm_yday fields
			if (strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S+00:00", &tmStruct)) {
				result = new GooString(buf);
			}
			else {
				result = new GooString(s);
			}
		}
		else {
			result = new GooString(s);
		}
	}
	return result;
}





static std::string myStringReplace(const std::string &inString, const std::string &oldToken, const std::string &newToken) {
	std::string result = inString;
	size_t foundLoc;
	int advance = 0;
	do {
		foundLoc = result.find(oldToken, advance);
		if (foundLoc != std::string::npos) {
			result.replace(foundLoc, oldToken.length(), newToken);
			advance = foundLoc + newToken.length();
		}
	} while (foundLoc != std::string::npos);
	return result;
}

static std::string myXmlTokenReplace(const char *inString) {
	std::string myString(inString);
	myString = myStringReplace(myString, "&", "&amp;");
	myString = myStringReplace(myString, "'", "&apos;");
	myString = myStringReplace(myString, "\"", "&quot;");
	myString = myStringReplace(myString, "<", "&lt;");
	myString = myStringReplace(myString, ">", "&gt;");
	return myString;
}


int CExportPDF::ExportPDFToTXT(wxString pdfFile, wxString fileout)
{
	PDFDoc *doc;
	GooString *fileName;
	GooString *textFileName;
	GooString *ownerPW, *userPW;
	TextOutputDev *textOut;
	FILE *f = nullptr;
	UnicodeMap *uMap;
	Object info;
	bool ok = true;
	int exitCode;
	exitCode = 99;

	if (bboxLayout) {
		bbox = true;
	}
	if (bbox) {
		htmlMeta = true;
	}

	// read config file
	globalParams = new GlobalParams();

	fileName = new GooString(pdfFile.ToStdString());
	if (fixedPitch) {
		physLayout = true;
	}

	if (textEncName[0]) {
		globalParams->setTextEncoding(textEncName);
	}
	if (textEOL[0]) {
		if (!globalParams->setTextEOL(textEOL)) {
			fprintf(stderr, "Bad '-eol' value on command line\n");
		}
	}
	if (noPageBreaks) {
		globalParams->setTextPageBreaks(false);
	}
	if (quiet) {
		globalParams->setErrQuiet(quiet);
	}

	// get mapping to output encoding
	if (!(uMap = globalParams->getTextEncoding())) {
		error(errCommandLine, -1, "Couldn't get text encoding");
		delete fileName;
		goto err1;
	}

	// open PDF file
	if (ownerPassword[0] != '\001') {
		ownerPW = new GooString(ownerPassword);
	}
	else {
		ownerPW = nullptr;
	}
	if (userPassword[0] != '\001') {
		userPW = new GooString(userPassword);
	}
	else {
		userPW = nullptr;
	}

	if (fileName->cmp("-") == 0) {
		delete fileName;
		fileName = new GooString("fd://0");
	}

	doc = PDFDocFactory().createPDFDoc(*fileName, ownerPW, userPW);

	if (userPW) {
		delete userPW;
	}
	if (ownerPW) {
		delete ownerPW;
	}
	if (!doc->isOk()) {
		exitCode = 1;
		goto err2;
	}

#ifdef ENFORCE_PERMISSIONS
	// check for copy permission
	if (!doc->okToCopy()) {
		error(errNotAllowed, -1, "Copying of text from this document is not allowed.");
		exitCode = 3;
		goto err2;
	}
#endif

	// construct text file name
	textFileName = new GooString(fileout.ToStdString());

	// get page range
	if (firstPage < 1) {
		firstPage = 1;
	}
	if (lastPage < 1 || lastPage > doc->getNumPages()) {
		lastPage = doc->getNumPages();
	}
	if (lastPage < firstPage) {
		error(errCommandLine, -1,
			"Wrong page range given: the first page ({0:d}) can not be after the last page ({1:d}).",
			firstPage, lastPage);
		goto err3;
	}

	// write HTML header
	if (htmlMeta) {
		if (!textFileName->cmp("-")) {
			f = stdout;
		}
		else {
			if (!(f = fopen(textFileName->c_str(), "wb"))) {
				error(errIO, -1, "Couldn't open text file '{0:t}'", textFileName);
				exitCode = 2;
				goto err3;
			}
		}
		fputs("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">", f);
		fputs("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n", f);
		fputs("<head>\n", f);
		info = doc->getDocInfo();
		if (info.isDict()) {
			Object obj = info.getDict()->lookup("Title");
			if (obj.isString()) {
				printInfoString(f, info.getDict(), "Title", "<title>", "</title>\n", uMap);
			}
			else {
				fputs("<title></title>\n", f);
			}
			printInfoString(f, info.getDict(), "Subject",
				"<meta name=\"Subject\" content=\"", "\"/>\n", uMap);
			printInfoString(f, info.getDict(), "Keywords",
				"<meta name=\"Keywords\" content=\"", "\"/>\n", uMap);
			printInfoString(f, info.getDict(), "Author",
				"<meta name=\"Author\" content=\"", "\"/>\n", uMap);
			printInfoString(f, info.getDict(), "Creator",
				"<meta name=\"Creator\" content=\"", "\"/>\n", uMap);
			printInfoString(f, info.getDict(), "Producer",
				"<meta name=\"Producer\" content=\"", "\"/>\n", uMap);
			printInfoDate(f, info.getDict(), "CreationDate",
				"<meta name=\"CreationDate\" content=\"\"/>\n");
			printInfoDate(f, info.getDict(), "LastModifiedDate",
				"<meta name=\"ModDate\" content=\"\"/>\n");
		}
		fputs("</head>\n", f);
		fputs("<body>\n", f);
		if (!bbox) {
			fputs("<pre>\n", f);
			if (f != stdout) {
				fclose(f);
			}
		}
	}

	// write text file
	if (htmlMeta && bbox) { // htmlMeta && is superfluous but makes gcc happier
		textOut = new TextOutputDev(nullptr, physLayout, fixedPitch, rawOrder, htmlMeta, discardDiag);

		if (textOut->isOk()) {
			if (bboxLayout) {
				printDocBBox(f, doc, textOut, firstPage, lastPage);
			}
			else {
				printWordBBox(f, doc, textOut, firstPage, lastPage);
			}
		}
		if (f != stdout) {
			fclose(f);
		}
	}
	else {
		textOut = new TextOutputDev(textFileName->c_str(),
			physLayout, fixedPitch, rawOrder, htmlMeta, discardDiag);
		if (textOut->isOk()) {
			if ((w == 0) && (h == 0) && (x == 0) && (y == 0)) {
				doc->displayPages(textOut, firstPage, lastPage, resolution, resolution, 0,
					true, false, false);
			}
			else {

				for (int page = firstPage; page <= lastPage; ++page) {
					doc->displayPageSlice(textOut, page, resolution, resolution, 0,
						true, false, false,
						x, y, w, h);
				}
			}

		}
		else {
			delete textOut;
			exitCode = 2;
			goto err3;
		}
	}
	delete textOut;

	// write end of HTML file
	if (htmlMeta) {
		if (!textFileName->cmp("-")) {
			f = stdout;
		}
		else {
			if (!(f = fopen(textFileName->c_str(), "ab"))) {
				error(errIO, -1, "Couldn't open text file '{0:t}'", textFileName);
				exitCode = 2;
				goto err3;
			}
		}
		if (!bbox) fputs("</pre>\n", f);
		fputs("</body>\n", f);
		fputs("</html>\n", f);
		if (f != stdout) {
			fclose(f);
		}
	}

	exitCode = 0;

	// clean up
err3:
	delete textFileName;
err2:
	delete doc;
	delete fileName;
	uMap->decRefCnt();
err1:
	delete globalParams;
	return exitCode;
}

static void printInfoString(FILE *f, Dict *infoDict, const char *key,
	const char *text1, const char *text2, UnicodeMap *uMap) {
	const GooString *s1;
	bool isUnicode;
	Unicode u;
	char buf[9];
	int i, n;

	Object obj = infoDict->lookup(key);
	if (obj.isString()) {
		fputs(text1, f);
		s1 = obj.getString();
		if ((s1->getChar(0) & 0xff) == 0xfe &&
			(s1->getChar(1) & 0xff) == 0xff) {
			isUnicode = true;
			i = 2;
		}
		else {
			isUnicode = false;
			i = 0;
		}
		while (i < obj.getString()->getLength()) {
			if (isUnicode) {
				u = ((s1->getChar(i) & 0xff) << 8) |
					(s1->getChar(i + 1) & 0xff);
				i += 2;
			}
			else {
				u = pdfDocEncoding[s1->getChar(i) & 0xff];
				++i;
			}
			n = uMap->mapUnicode(u, buf, sizeof(buf));
			buf[n] = '\0';
			const std::string myString = myXmlTokenReplace(buf);
			fputs(myString.c_str(), f);
		}
		fputs(text2, f);
	}
}

static void printInfoDate(FILE *f, Dict *infoDict, const char *key, const char *fmt) {
	Object obj = infoDict->lookup(key);
	if (obj.isString()) {
		const char *s = obj.getString()->c_str();
		if (s[0] == 'D' && s[1] == ':') {
			s += 2;
		}
		fprintf(f, fmt, s);
	}
}

static void printLine(FILE *f, TextLine *line) {
	double xMin, yMin, xMax, yMax;
	double lineXMin = 0, lineYMin = 0, lineXMax = 0, lineYMax = 0;
	TextWord *word;
	std::stringstream wordXML;
	wordXML << std::fixed << std::setprecision(6);

	for (word = line->getWords(); word; word = word->getNext()) {
		word->getBBox(&xMin, &yMin, &xMax, &yMax);

		if (lineXMin == 0 || lineXMin > xMin) lineXMin = xMin;
		if (lineYMin == 0 || lineYMin > yMin) lineYMin = yMin;
		if (lineXMax < xMax) lineXMax = xMax;
		if (lineYMax < yMax) lineYMax = yMax;

		GooString *wordText = word->getText();
		const std::string myString = myXmlTokenReplace(wordText->c_str());
		wordXML << "          <word xMin=\"" << xMin << "\" yMin=\"" << yMin << "\" xMax=\"" <<
			xMax << "\" yMax=\"" << yMax << "\">" << myString << "</word>\n";
		delete wordText;
	}
	fprintf(f, "        <line xMin=\"%f\" yMin=\"%f\" xMax=\"%f\" yMax=\"%f\">\n",
		lineXMin, lineYMin, lineXMax, lineYMax);
	fputs(wordXML.str().c_str(), f);
	fputs("        </line>\n", f);
}

void printDocBBox(FILE *f, PDFDoc *doc, TextOutputDev *textOut, int first, int last) {
	double xMin, yMin, xMax, yMax;
	TextFlow *flow;
	TextBlock *blk;
	TextLine *line;

	fprintf(f, "<doc>\n");
	for (int page = first; page <= last; ++page) {
		fprintf(f, "  <page width=\"%f\" height=\"%f\">\n", doc->getPageMediaWidth(page), doc->getPageMediaHeight(page));
		doc->displayPage(textOut, page, resolution, resolution, 0, true, false, false);
		for (flow = textOut->getFlows(); flow; flow = flow->getNext()) {
			fprintf(f, "    <flow>\n");
			for (blk = flow->getBlocks(); blk; blk = blk->getNext()) {
				blk->getBBox(&xMin, &yMin, &xMax, &yMax);
				fprintf(f, "      <block xMin=\"%f\" yMin=\"%f\" xMax=\"%f\" yMax=\"%f\">\n", xMin, yMin, xMax, yMax);
				for (line = blk->getLines(); line; line = line->getNext()) {
					printLine(f, line);
				}
				fprintf(f, "      </block>\n");
			}
			fprintf(f, "    </flow>\n");
		}
		fprintf(f, "  </page>\n");
	}
	fprintf(f, "</doc>\n");
}

void printWordBBox(FILE *f, PDFDoc *doc, TextOutputDev *textOut, int first, int last) {
	fprintf(f, "<doc>\n");
	for (int page = first; page <= last; ++page) {
		fprintf(f, "  <page width=\"%f\" height=\"%f\">\n", doc->getPageMediaWidth(page), doc->getPageMediaHeight(page));
		doc->displayPage(textOut, page, resolution, resolution, 0, true, false, false);
		TextWordList *wordlist = textOut->makeWordList();
		const int word_length = wordlist != nullptr ? wordlist->getLength() : 0;
		TextWord *word;
		double xMinA, yMinA, xMaxA, yMaxA;
		if (word_length == 0)
			fprintf(stderr, "no word list\n");

		for (int i = 0; i < word_length; ++i) {
			word = wordlist->get(i);
			word->getBBox(&xMinA, &yMinA, &xMaxA, &yMaxA);
			const std::string myString = myXmlTokenReplace(word->getText()->c_str());
			fprintf(f, "    <word xMin=\"%f\" yMin=\"%f\" xMax=\"%f\" yMax=\"%f\">%s</word>\n", xMinA, yMinA, xMaxA, yMaxA, myString.c_str());
		}
		fprintf(f, "  </page>\n");
		delete wordlist;
	}
	fprintf(f, "</doc>\n");
}