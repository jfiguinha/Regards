// Le bloc ifdef suivant est la façon standard de créer des macros qui facilitent l'exportation 
// à partir d'une DLL. Tous les fichiers contenus dans cette DLL sont compilés avec le symbole DLLPICTURE_EXPORTS
// défini sur la ligne de commande. Ce symbole ne doit pas être défini pour un projet
// qui utilisent cette DLL. De cette manière, les autres projets dont les fichiers sources comprennent ce fichier considèrent les fonctions 
// DLLPICTURE_API comme étant importées à partir d'une DLL, tandis que cette DLL considère les symboles
// définis avec cette macro comme étant exportés.
#pragma once
#include <map>
using namespace std;

class CxImage;
class CRegardsBitmap;
class CVideoThumbnail;
class CImageVideoThumbnail;
class CImageLoadingFormat;
class CPictureData;
class CRegardsConfigParam;

namespace Regards
{
	namespace Picture
	{
		class CLibPicture
		{

		public:

			CLibPicture();
			virtual ~CLibPicture();

			void LoadPicture(const wxString& fileName, const bool& isThumbnail, const int& numPicture, CImageLoadingFormat * bitmap);
			bool TestIsVideoValid(const wxString & szFileName);
			void DefineSVGSize(const int &width, const int &height);
			bool HasThumbnail(const wxString &filename);
			int GetNbImage(const  wxString & szFileName);
			uint32_t GetFrameDelay(const  wxString & szFileName);
			int TestImageFormat(const wxString & szFileName);
			bool TestIsVideo(const wxString & szFileName);
			bool TestIsPDF(const wxString & szFileName);
			bool TestIsPicture(const wxString & szFileName);
			bool TestIsAnimation(const wxString & szFileName);
			int GetPictureDimensions(const  wxString & fileName, int & width, int & height, int & rotation);
			CImageLoadingFormat * LoadPicture(const  wxString & szFileName, const bool &isThumbnail = false, const int &numPicture = 0);
			int SavePicture(const  wxString & fileName, CImageLoadingFormat * bitmap);
			int SavePicture(const wxString & fileNameIn, const wxString & fileNameOut);
			//CRegardsBitmap * LoadThumbnailOnlyFromPicture(const  wxString & fileName, int &returnValue);
			vector<CImageVideoThumbnail *> LoadDefaultVideoThumbnail(const  wxString & szFileName, const int &size);
			CImageLoadingFormat * LoadThumbnail(const  wxString & szFileName, const bool &fromExifOnly = false);
			//vector<CImageVideoThumbnail *> LoadAllVideoThumbnail(const  wxString & szFileName, const bool &compressJpeg);
			void LoadAllVideoThumbnail(const  wxString & szFileName, vector<CImageVideoThumbnail *> * listThumbnail, const bool &compressJpeg, const bool &isThumbnail);

			CImageLoadingFormat * LoadVideoThumbnail(const  wxString & szFileName, const int &percent, int &timePosition);

			//CRegardsBitmap * LoadThumbnailFromBuffer(uint8_t * data, size_t size, const char * ext, const int &orientation);
			bool TestIsExifCompatible(const wxString &filename);
			int SavePictureOption(const int &format, int &option, int &quality);
			int SavePicture(const  wxString & fileName, CImageLoadingFormat * bitmap, const int &option, const int &quality);
			//CPictureData * LoadPictureData(const wxString &filename, bool &pictureOK);
			//-----------------------------------------------------------------------
			//Fonction de conversion
			//-----------------------------------------------------------------------
			CxImage * ConvertwxImageToCxImage(const wxImage & image);
			CRegardsBitmap * ConvertCXImageToRegardsBitmap(CxImage * image);
			//CRegardsBitmap * ConvertCXImageToScaleRegardsBitmap(CxImage * image, const int &width, const int &height);
			CxImage * ConvertRegardsBitmapToCXImage(CRegardsBitmap * bitmap);
			CRegardsBitmap * ConvertCXImageToRegardsBitmap(CxImage * image, const int &orientation);
			CRegardsBitmap * ConvertwxImageToRegardsBitmap(const wxImage & image);
			CRegardsBitmap * ConvertCXImageToScaleRegardsBitmapBGR(CxImage * image, const int &width, const int &height);
			CRegardsBitmap * ConvertCXImageToScaleRegardsBitmapCRgbaquad(CxImage * image, const int &width, const int &height);
			static wxImage * ConvertRegardsBitmapToWXImage(CRegardsBitmap * image, const bool &loadAlpha = false);
			static wxImage ConvertRegardsBitmapToWXImage(CRegardsBitmap * bitmap, const bool &flip, const bool &loadAlpha = false);
			static float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &width, const int &height);

			static void Initx265Decoder();
			static void Uninitx265Decoder();

			static void InitFreeImage();
			static void UninitFreeImage();

			bool SaveToPDF(wxImage * poImage, const wxString &fileName, int option = -1, int quality = -1);

#ifdef LIBBPG
#if defined(WIN32) && not defined(__MINGW32__)
#else
			static void LoadBpgDll();
			static void UnloadBpgDll();
#endif
#endif
			static CImageLoadingFormat * GetCancelPhoto(const wxString &szFileName, const int &widthThumbnail = 0, const int &heightThumbnail = 0);
			CRegardsBitmap * LoadPictureToBGRA(const wxString &filename, bool &pictureOK, const int &resizeWidth = 0, const int &resizeHeight = 0);

		private:

			//CPictureData * LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth = 0, const int &resizeHeight = 0);
			bool SaveToPDF(wxImage* poImage, const wxString &fileName, const wxString &pictureName, int option, int quality);
			CRegardsBitmap * LoadFromFreeImage(const char* filename);
			bool PictureDimensionFreeImage(const char* filename, int &width, int &height);
			void LoadwxImageThumbnail(const wxString & szFileName, vector<CImageVideoThumbnail *> * listThumbnail, const int & bitmapType, const int &width, const int &height, const bool &compressJpeg, const bool & isThumbnail);

			//-----------------------------------------------------------------------------
			//Liste des fonctions de la DLL
			//-----------------------------------------------------------------------------
			int TestExtension(const wxString & ext);


			int svgWidth;
			int svgHeight;
			CRegardsConfigParam* configRegards;

#ifdef LIBBPG
#if defined(WIN32) && not defined(__MINGW32__)
#else
			static void * lib_handle;
#endif
#endif
			map<wxString, bool> fileValid;

		};

	}
}

