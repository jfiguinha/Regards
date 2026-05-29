#pragma once
// =============================================================================
// Refactoring de CLibPicture en couches métier distinctes
// =============================================================================

#include <vector>
#include <map>
#include <cstdint>
#include <wx/string.h>
#include <opencv2/core.hpp>

class CImageLoadingFormat;
class CImageVideoThumbnail;

namespace Regards {
namespace Picture {

// =============================================================================
// FormatDetector
// Responsabilité : détecter et identifier le format d'un fichier image/vidéo
// Méthodes issues de : TestImageFormat, TestIsPicture, TestIsVideo, TestIsPDF,
//                      TestIsAnimation, TestIsExifCompatible, TestExtension,
//                      HasThumbnail
// =============================================================================
class FormatDetector
{
public:
    // Retourne l'identifiant de format (JPEG, PNG, MP4, etc.) depuis l'extension
    static int  DetectFormat(const wxString& fileName, bool forReading = true);

    // Retourne l'identifiant numérique associé à une extension (table de lookup)
    static int  GetExtensionId(const wxString& ext);

    static bool IsPicture(const wxString& fileName);
    static bool IsVideo(const wxString& fileName);
    static bool IsPDF(const wxString& fileName);
    static bool IsAnimation(const wxString& fileName);
    static bool IsExifCompatible(const wxString& fileName);

    // Vérifie si le fichier possède un thumbnail EXIF embarqué
    static bool HasEmbeddedThumbnail(const wxString& fileName);
};


// =============================================================================
// ImageConverter
// Responsabilité : convertir des représentations d'image (wxImage ↔ cv::Mat,
//                  calcul de ratios, orientation)
// Méthodes issues de : mat_from_wx, ConvertRegardsBitmapToWXImage,
//                      CalculPictureRatio, CalculRatio,
//                      CalculThumbSizeFromScreenDef
// =============================================================================
class ImageConverter
{
public:
    // wxImage → cv::Mat (BGR/BGRA selon canal alpha)
    static cv::Mat      WxToCvMat(const wxImage& wx);

    // cv::Mat → wxImage (gère niveaux de gris, BGRA, BGR)
    static wxImage      CvMatToWx(cv::Mat& img);

    // Calcule le ratio pour faire tenir (pictureW×pictureH) dans (targetW×targetH)
    static float        CalcFitRatio(int pictureWidth, int pictureHeight,
                                     int targetWidth,  int targetHeight);

    // Calcule le ratio adapté à la taille de l'écran / 4 (usage miniature)
    static float        CalcScreenThumbnailRatio(int width, int height);

    // Ajuste width/height aux facteurs de scaling TurboJPEG les plus proches
    static void         AdjustSizeToJpegScalingFactors(int& width, int& height);
};


// =============================================================================
// MetadataService
// Responsabilité : lire/écrire les métadonnées EXIF/IPTC/XMP, orientation,
//                  durée vidéo
// Méthodes issues de : TestIsExifCompatible, HasThumbnail, ApplyOrientation,
//                      GetVideoDuration, TestIsVideoValid
// =============================================================================
class MetadataService
{
public:
    // Lit l'orientation EXIF et l'applique au bitmap (si le format le supporte)
    void ApplyExifOrientation(const wxString& fileName, CImageLoadingFormat* bitmap);

    // Retourne l'orientation EXIF brute (-1 si absente)
    int  GetOrientation(const wxString& fileName);

    // Durée en secondes d'une vidéo (avec cache interne)
    static int  GetVideoDuration(const wxString& fileName);

    // Vérifie qu'une vidéo est lisible (durée > 0 ET flux décodable)
    static bool IsVideoValid(const wxString& fileName);

private:
    // Cache : fileName → durée en ms
    static std::map<wxString, int64_t> videoDurationCache_;
};


// =============================================================================
// ImageLoader
// Responsabilité : charger une image (full ou thumbnail) depuis le disque,
//                  gérer tous les formats supportés
// Méthodes issues de : LoadPicture (void & ptr), LoadPictureToBGRA,
//                      LoadFromFreeImage, GetCancelPhoto, GetPictureDimensions,
//                      GetNbImage, GetFrameDelay, DefineSVGSize
// =============================================================================
class ImageLoader
{
public:
    ImageLoader();
    ~ImageLoader();

    // Charge l'image fileName dans bitmap (frame numPicture, mode thumbnail ou full)
    void              Load(const wxString& fileName, bool isThumbnail,
                           int numPicture, CImageLoadingFormat* bitmap);

    // Variante retournant un nouveau CImageLoadingFormat (propriété transférée à l'appelant)
    CImageLoadingFormat* Load(const wxString& fileName, bool isThumbnail = false,
                              int numPicture = 0);

    // Charge + redimensionne en BGRA (resizeWidth/Height = 0 → pas de resize)
    CImageLoadingFormat* LoadToBGRA(const wxString& fileName, bool& ok,
                                    int resizeWidth = 0, int resizeHeight = 0);

    // Retourne une image "annulation" aux dimensions données
    static CImageLoadingFormat* GetCancelPhoto(const wxString& fileName,
                                               int thumbW = 0, int thumbH = 0);

    // Lit width/height/rotation sans décoder toute l'image
    int  GetDimensions(const wxString& fileName, int& width, int& height, int& rotation);

    // Nombre de frames/pages (GIF multi-frame, TIFF, PDF, vidéo…)
    int  GetFrameCount(const wxString& fileName);

    // Délai entre frames en ms (GIF, HEIC animé…)
    uint32_t GetFrameDelay(const wxString& fileName);

    // Taille de rendu SVG (défaut 1024×1024)
    void SetSvgSize(int width, int height);

    // Initialisation / libération globale de FreeImage
    static void InitFreeImage();
    static void UninitFreeImage();

private:
    cv::Mat LoadViaCvMat(const wxString& fileName);      // OpenCV générique
    cv::Mat LoadViaFreeImage(const char* filename);      // FallBack FreeImage
    bool    GetDimensionsViaFreeImage(const char* filename, int& w, int& h);

    int svgWidth_  = 1024;
    int svgHeight_ = 1024;

#ifdef WIN32
    class CWic* wic_ = nullptr;
#endif
#ifdef __APPLE__
    class CReadMacOSImage* readimage_ = nullptr;
#endif
};


// =============================================================================
// ImageSaver
// Responsabilité : sauvegarder une image dans le format demandé, gérer les
//                  options de compression, copier les métadonnées EXIF
// Méthodes issues de : SavePicture (x3 surcharges), SavePictureOption
// =============================================================================
class ImageSaver
{
public:
    // Ouvre la boîte de dialogue d'options puis sauvegarde
    int Save(const wxString& fileName, CImageLoadingFormat* bitmap);

    // Sauvegarde avec options et qualité déjà connues
    int Save(const wxString& fileName, CImageLoadingFormat* bitmap,
             int option, int quality);

    // Conversion multi-pages : fileNameIn → fileNameOut
    // (génère fileNameOut_page_N.ext si plusieurs frames)
    int Convert(const wxString& fileNameIn, const wxString& fileNameOut);

    // Affiche la boîte de dialogue d'options pour le format donné
    // Retourne 1 si l'utilisateur a validé, 0 sinon
    int AskSaveOptions(int format, int& option, int& quality);
};


// =============================================================================
// ThumbnailService
// Responsabilité : obtenir un thumbnail d'image statique (EXIF, RAW, HEIC…)
// Méthodes issues de : LoadThumbnail
// =============================================================================
class ThumbnailService
{
public:
    // Charge le thumbnail le plus approprié :
    //   1. EXIF embarqué (si disponible)
    //   2. Thumbnail natif du format (HEIC, RAW…)
    //   3. Image complète redimensionnée en dernier recours
    // fromExifOnly = true : n'essaie pas de charger l'image complète
    CImageLoadingFormat* LoadThumbnail(const wxString& fileName,
                                       bool fromExifOnly = false);
};


// =============================================================================
// VideoThumbnailService
// Responsabilité : extraire des frames depuis des vidéos ou animations
// Méthodes issues de : LoadVideoThumbnail, LoadAllVideoThumbnail,
//                      LoadDefaultVideoThumbnail, LoadwxImageThumbnail
// =============================================================================
class VideoThumbnailService
{
public:
    // Frame unique à la position percent% de la vidéo
    CImageLoadingFormat* LoadVideoFrame(const wxString& fileName,
                                        int percent, int& timePosition);

    // Toutes les frames d'une vidéo / animation (HEIC, WebP, GIF, PDF…)
    std::vector<CImageVideoThumbnail*> LoadAllFrames(const wxString& fileName,
                                                     bool compressJpeg,
                                                     bool isThumbnail);

    // Génère size placeholders (image "loading") en attendant le vrai chargement
    std::vector<CImageVideoThumbnail*> LoadPlaceholders(const wxString& fileName,
                                                        int count);

private:
    // Charge les frames d'un format multi-page géré par wxImage ou CRegardsPDF
    std::vector<CImageVideoThumbnail*> LoadWxMultiPage(const wxString& fileName,
                                                       int bitmapType,
                                                       int thumbW, int thumbH,
                                                       bool compressJpeg,
                                                       bool isThumbnail);
};


} // namespace Picture
} // namespace Regards
