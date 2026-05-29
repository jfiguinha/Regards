#pragma once
// =============================================================================
// libPicture.h  —  CLibPicture : façade rétro-compatible
//
// Cette classe conserve exactement la même interface publique qu'avant le
// refactoring. Elle délègue chaque appel à la couche métier correspondante
// (FormatDetector, ImageConverter, ImageLoader, ImageSaver,
//  ThumbnailService, VideoThumbnailService, MetadataService).
// =============================================================================

#include <map>
#include <vector>
#include <cstdint>
#include <wx/string.h>
#include <opencv2/core.hpp>

// Forward declarations — inchangées par rapport à l'original
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
    namespace Picture
    {
        // Forward declarations des couches métier
        class FormatDetector;
        class ImageConverter;
        class ImageLoader;
        class ImageSaver;
        class ThumbnailService;
        class VideoThumbnailService;
        class MetadataService;

#ifdef WIN32
        class CWic;
#endif

        // =====================================================================
        // CLibPicture — façade
        // =====================================================================
        class CLibPicture
        {
        public:
            CLibPicture();
            virtual ~CLibPicture();

            // -----------------------------------------------------------------
            // Chargement
            // -----------------------------------------------------------------
            void LoadPicture(const wxString& fileName, const bool& isThumbnail,
                             const int& numPicture, CImageLoadingFormat* bitmap);

            CImageLoadingFormat* LoadPicture(const wxString& szFileName,
                                             const bool& isThumbnail = false,
                                             const int& numPicture   = 0);

            CImageLoadingFormat* LoadPictureToBGRA(const wxString& filename,
                                                   bool& pictureOK,
                                                   const int& resizeWidth  = 0,
                                                   const int& resizeHeight = 0);

            // -----------------------------------------------------------------
            // Sauvegarde
            // -----------------------------------------------------------------
            int SavePicture(const wxString& fileName, CImageLoadingFormat* bitmap);

            int SavePicture(const wxString& fileName, CImageLoadingFormat* bitmap,
                            const int& option, const int& quality);

            int SavePicture(const wxString& fileNameIn, const wxString& fileNameOut);

            int SavePictureOption(const int& format, int& option, int& quality);

            // -----------------------------------------------------------------
            // Thumbnails image statique
            // -----------------------------------------------------------------
            CImageLoadingFormat* LoadThumbnail(const wxString& szFileName,
                                               const bool& fromExifOnly = false);

            bool HasThumbnail(const wxString& filename);

            // -----------------------------------------------------------------
            // Thumbnails vidéo / animation
            // -----------------------------------------------------------------
            CImageLoadingFormat* LoadVideoThumbnail(const wxString& szFileName,
                                                    const int& percent,
                                                    int& timePosition);

            std::vector<CImageVideoThumbnail*> LoadAllVideoThumbnail(
                const wxString& szFileName,
                const bool& compressJpeg,
                const bool& isThumbnail);

            std::vector<CImageVideoThumbnail*> LoadDefaultVideoThumbnail(
                const wxString& szFileName, const int& size);

            // -----------------------------------------------------------------
            // Métadonnées / infos fichier
            // -----------------------------------------------------------------
            int      GetPictureDimensions(const wxString& fileName,
                                          int& width, int& height, int& rotation);
            int      GetNbImage(const wxString& szFileName);
            uint32_t GetFrameDelay(const wxString& szFileName);

            static int  GetVideoDuration(const wxString& szFileName);
            bool        TestIsVideoValid(const wxString& szFileName);

            // -----------------------------------------------------------------
            // Détection de format
            // -----------------------------------------------------------------
            static int  TestImageFormat(const wxString& szFileName,
                                        const bool& reading = true);
            static int  TestExtension(const wxString& ext);

            bool TestIsVideo(const wxString& szFileName);
            bool TestIsPDF(const wxString& szFileName);
            bool TestIsPicture(const wxString& szFileName);
            bool TestIsAnimation(const wxString& szFileName);
            bool TestIsExifCompatible(const wxString& filename);

            // -----------------------------------------------------------------
            // Conversion
            // -----------------------------------------------------------------
            static cv::Mat  mat_from_wx(const wxImage& wx);
            static wxImage  ConvertRegardsBitmapToWXImage(cv::Mat& img);
            static float    CalculPictureRatio(const int& pictureWidth,
                                               const int& pictureHeight,
                                               const int& width,
                                               const int& height);

            // -----------------------------------------------------------------
            // Image d'erreur / annulation
            // -----------------------------------------------------------------
            static CImageLoadingFormat* GetCancelPhoto(
                const wxString& szFileName,
                const int& widthThumbnail  = 0,
                const int& heightThumbnail = 0);

            // -----------------------------------------------------------------
            // SVG
            // -----------------------------------------------------------------
            void DefineSVGSize(const int& width, const int& height);

            // -----------------------------------------------------------------
            // FreeImage lifecycle
            // -----------------------------------------------------------------
            static void InitFreeImage();
            static void UninitFreeImage();

        private:
            // Couches métier instanciées en composition
            ImageLoader*            loader_;
            ImageSaver*             saver_;
            ThumbnailService*       thumbSvc_;
            VideoThumbnailService*  videoThumbSvc_;
            MetadataService*        metaSvc_;

            // Conservé pour rétro-compatibilité (accès via configRegards dans
            // le code appelant existant)
            CRegardsConfigParam* configRegards;
        };

    } // namespace Picture
} // namespace Regards
