// Le bloc ifdef suivant est la façon standard de créer des macros qui facilitent l'exportation 
// à partir d'une DLL. Tous les fichiers contenus dans cette DLL sont compilés avec le symbole DLLPICTURE_EXPORTS
// défini sur la ligne de commande. Ce symbole ne doit pas être défini pour un projet
// qui utilisent cette DLL. De cette manière, les autres projets dont les fichiers sources comprennent ce fichier considèrent les fonctions 
// DLLPICTURE_API comme étant importées à partir d'une DLL, tandis que cette DLL considère les symboles
// définis avec cette macro comme étant exportés.
#pragma once

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <stdint.h>
#include <string>
using namespace std;

#ifdef LIBBPG
#ifdef WIN32
#include <DllBpg.h>
#else
#include <bpg.h>
#endif
#endif

class CxImage;
class CRegardsBitmap;
class CVideoThumbnail;

#define FRENCH 1
#define ENGLISH 2
#define SPANISH 3

#define JPEG 1
#define BMP 2
#define TIFF 3
#define RAW 4
#define GIF 5
#define PNM 6
#define JPC 7
#define PCX 8
#define PNG 9
#define TGA 10
#define JPEG2000 11
#define PCD 12
#define PSD 13
#define PDF 14
#define PGX 15
#define MNG 16
#define PPM 17
#define BPG 18
#define IFF 19
#define XPM 20
#define ICO 21
#define CUR 22
#define ANI 23
#define SVG 24

#define AVI 101
#define MOV 102
#define MP4 103
#define WMV 104
#define MPG2 105
#define MPEG 108
#define AVCHD 106
#define WINREC 107
#define MPG 108


class CLibPicture
{

public:

    CLibPicture();
    virtual ~CLibPicture();

    void DefineSVGSize(const int &width, const int &height);
    bool HasThumbnail(const wxString &filename);
    CRegardsBitmap * ConvertwxImageToRegardsBitmap(const wxImage & image);
	int TestImageFormat(const wxString & szFileName);
	bool TestIsVideo(const wxString & szFileName);
	bool TestIsPicture(const wxString & szFileName);
    bool TestIsAnimation(const wxString & szFileName);
	int GetDimensions(const  wxString & fileName, int & width, int & height, int & rotation);
	CRegardsBitmap * LoadPicture(const  wxString & szFileName);
	int SavePicture(const  wxString & fileName, CRegardsBitmap * bitmap);
	
	//CRegardsBitmap * LoadThumbnailOnlyFromPicture(const  wxString & fileName, int &returnValue);

	CRegardsBitmap * LoadThumbnail(const  wxString & szFileName, const bool &fromExifOnly = false);
	CVideoThumbnail * LoadAllVideoThumbnail(const  wxString & szFileName, int &width, int &height);
	CRegardsBitmap * LoadVideoThumbnail(const  wxString & szFileName, const int &percent, int &timePosition);

	//CRegardsBitmap * LoadThumbnailFromBuffer(uint8_t * data, size_t size, const char * ext, const int &orientation);

	int SavePictureOption(const int &format, int &option, int &quality);
	int SavePicture(const  wxString & fileName, CRegardsBitmap * bitmap,const int &option, const int &quality);

private:

	//-----------------------------------------------------------------------------
	//Liste des fonctions de la DLL
	//-----------------------------------------------------------------------------
	int TestExtension(const wxString & ext);
#ifdef LIBRAW
	CRegardsBitmap * LoadThumbnailFromRawPicture(const wxString & szFilename, int &returnValue);
#endif
	CRegardsBitmap * ConvertCXImageToRegardsBitmap(CxImage * image);
	CRegardsBitmap * ConvertCXImageToScaleRegardsBitmap(CxImage * image, const int &width, const int &height);
	CxImage * ConvertRegardsBitmapToCXImage(CRegardsBitmap * bitmap);
	CRegardsBitmap * ConvertCXImageToRegardsBitmap(CxImage * image, const int &orientation);
	CRegardsBitmap * LoadThumbnailPicture(const wxString & fileName, const int &width, const int &height);
	
	CRegardsBitmap * ConvertCXImageToScaleRegardsBitmapBGR(CxImage * image, const int &width, const int &height);
	CRegardsBitmap * ConvertCXImageToScaleRegardsBitmapCRgbaquad(CxImage * image, const int &width, const int &height);
	CRegardsBitmap * ConvertWXImageToRegardsBitmap(wxImage * image);
	wxImage * ConvertRegardsBitmapToWXImage(CRegardsBitmap * image, const bool &loadAlpha = false);
    
    int svgWidth = 1024;
    int svgHeight = 1024;
    
    
#ifdef LIBBPG
#ifndef WIN32
    static void * lib_handle;
	static CBpg * libbpg;
#endif
#endif
    
    
};
