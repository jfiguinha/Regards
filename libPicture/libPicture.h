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
class CReadMacOSImage;

namespace Regards
{
	namespace Video
	{
		class CThumbnailVideo;
	}
	namespace Picture
	{
#ifdef WIN32
		class CWic;
#endif
		

		class CLibPicture
		{
		public:
			CLibPicture();
			virtual ~CLibPicture();

			void LoadPicture(const wxString& fileName, const bool& isThumbnail, const int& numPicture,
			                 CImageLoadingFormat* bitmap);
			bool TestIsVideoValid(const wxString& szFileName);
			void DefineSVGSize(const int& width, const int& height);
			bool HasThumbnail(const wxString& filename);
			int GetNbImage(const wxString& szFileName);
			uint32_t GetFrameDelay(const wxString& szFileName);
			static int TestImageFormat(const wxString& szFileName, const bool& reading = true);
			bool TestIsVideo(const wxString& szFileName);
			bool TestIsPDF(const wxString& szFileName);
			bool TestIsPicture(const wxString& szFileName);
			bool TestIsAnimation(const wxString& szFileName);
			int GetPictureDimensions(const wxString& fileName, int& width, int& height, int& rotation);
			CImageLoadingFormat* LoadPicture(const wxString& szFileName, const bool& isThumbnail = false,
			                                 const int& numPicture = 0);
			int SavePicture(const wxString& fileName, CImageLoadingFormat* bitmap);
			int SavePicture(const wxString& fileNameIn, const wxString& fileNameOut);
			//CRegardsBitmap * LoadThumbnailOnlyFromPicture(const  wxString & fileName, int &returnValue);
			vector<CImageVideoThumbnail*> LoadDefaultVideoThumbnail(const wxString& szFileName, const int& size);
			CImageLoadingFormat* LoadThumbnail(const wxString& szFileName, const bool& fromExifOnly = false);
			//vector<CImageVideoThumbnail *> LoadAllVideoThumbnail(const  wxString & szFileName, const bool &compressJpeg);
			vector<CImageVideoThumbnail*> LoadAllVideoThumbnail(const wxString& szFileName,const bool& compressJpeg, const bool& isThumbnail);

			CImageLoadingFormat* LoadVideoThumbnail(const wxString& szFileName, const int& percent, int& timePosition);

			static int GetVideoDuration(const wxString& szFileName);

			//CRegardsBitmap * LoadThumbnailFromBuffer(uint8_t * data, size_t size, const char * ext, const int &orientation);
			bool TestIsExifCompatible(const wxString& filename);
			int SavePictureOption(const int& format, int& option, int& quality);
			int SavePicture(const wxString& fileName, CImageLoadingFormat* bitmap, const int& option,
			                const int& quality);
			//CPictureData * LoadPictureData(const wxString &filename, bool &pictureOK);
			//-----------------------------------------------------------------------
			//Fonction de conversion
			//-----------------------------------------------------------------------
			static cv::Mat mat_from_wx(const wxImage& wx);
			static float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight, const int& width,
			                                const int& height);
                                            
                                            
			static void Initx265Decoder();
			static void Uninitx265Decoder();

			static void InitFreeImage();
			static void UninitFreeImage();

			static wxImage ConvertRegardsBitmapToWXImage(cv::Mat& img);
			//static wxImage* ConvertRegardsBitmapToWXImagePt(cv::Mat& img);

			static CImageLoadingFormat* GetCancelPhoto(const wxString& szFileName, const int& widthThumbnail = 0,
			                                           const int& heightThumbnail = 0);
			CImageLoadingFormat* LoadPictureToBGRA(const wxString& filename, bool& pictureOK,
			                                       const int& resizeWidth = 0, const int& resizeHeight = 0);
			static int TestExtension(const wxString& ext);

	

		private:
			cv::Mat LoadFromFreeImage(const char* filename);
			bool PictureDimensionFreeImage(const char* filename, int& width, int& height);
			vector<CImageVideoThumbnail*> LoadwxImageThumbnail(const wxString& szFileName,
			                          const int& bitmapType, const int& width, const int& height,
			                          const bool& compressJpeg, const bool& isThumbnail);

			//-----------------------------------------------------------------------------
			//Liste des fonctions de la DLL
			//-----------------------------------------------------------------------------

			void ApplyOrientation(const wxString& fileName, const bool& applyExif, CImageLoadingFormat* bitmap);

            bool useOpenCVThumb = true;
			int svgWidth;
			int svgHeight;
			CRegardsConfigParam* configRegards;
			static std::map<wxString, int64_t> listMovieDuration;

#ifdef LIBBPG
#if defined(WIN32) && not defined(__MINGW32__)
#else
			static void* lib_handle;
#endif
#endif


#ifdef WIN32
			CWic* wic = nullptr;
#endif
#ifdef __APPLE__
			CReadMacOSImage * readimage = nullptr;
#endif


		};
	}
}
