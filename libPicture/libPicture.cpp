// =============================================================================
// libPicture_facade.cpp  —  CLibPicture : implémentation de la façade
//
// Chaque méthode publique délègue à la couche métier correspondante.
// Aucune logique métier n'est dupliquée ici.
// =============================================================================

#include <header.h>
#include "picture_factory.h"
#include "libPicture.h"
#include <ParamInit.h>

using namespace Regards::Picture;

// =============================================================================
//  Cycle de vie
// =============================================================================

CLibPicture::CLibPicture()
    : loader_        (new ImageLoader())
    , saver_         (new ImageSaver())
    , thumbSvc_      (new ThumbnailService())
    , videoThumbSvc_ (new VideoThumbnailService())
    , metaSvc_       (new MetadataService())
    , configRegards  (CParamInit::getInstance())
{}

CLibPicture::~CLibPicture()
{
    delete loader_;
    delete saver_;
    delete thumbSvc_;
    delete videoThumbSvc_;
    delete metaSvc_;
}

// =============================================================================
//  Chargement
// =============================================================================

void CLibPicture::LoadPicture(const wxString& fileName,
                               const bool& isThumbnail,
                               const int& numPicture,
                               CImageLoadingFormat* bitmap)
{
    loader_->Load(fileName, isThumbnail, numPicture, bitmap);
}

CImageLoadingFormat* CLibPicture::LoadPicture(const wxString& szFileName,
                                               const bool& isThumbnail,
                                               const int& numPicture)
{
    return loader_->Load(szFileName, isThumbnail, numPicture);
}

CImageLoadingFormat* CLibPicture::LoadPictureToBGRA(const wxString& filename,
                                                     bool& pictureOK,
                                                     const int& resizeWidth,
                                                     const int& resizeHeight)
{
    return loader_->LoadToBGRA(filename, pictureOK, resizeWidth, resizeHeight);
}

// =============================================================================
//  Sauvegarde
// =============================================================================

int CLibPicture::SavePicture(const wxString& fileName,
                              CImageLoadingFormat* bitmap)
{
    return saver_->Save(fileName, bitmap);
}

int CLibPicture::SavePicture(const wxString& fileName,
                              CImageLoadingFormat* bitmap,
                              const int& option,
                              const int& quality)
{
    return saver_->Save(fileName, bitmap, option, quality);
}

int CLibPicture::SavePicture(const wxString& fileNameIn,
                              const wxString& fileNameOut)
{
    return saver_->Convert(fileNameIn, fileNameOut);
}

int CLibPicture::SavePictureOption(const int& format, int& option, int& quality)
{
    return saver_->AskSaveOptions(format, option, quality);
}

// =============================================================================
//  Thumbnails image statique
// =============================================================================

CImageLoadingFormat* CLibPicture::LoadThumbnail(const wxString& szFileName,
                                                 const bool& fromExifOnly)
{
    return thumbSvc_->LoadThumbnail(szFileName, fromExifOnly);
}

bool CLibPicture::HasThumbnail(const wxString& filename)
{
    return FormatDetector::HasEmbeddedThumbnail(filename);
}

// =============================================================================
//  Thumbnails vidéo / animation
// =============================================================================

CImageLoadingFormat* CLibPicture::LoadVideoThumbnail(const wxString& szFileName,
                                                      const int& percent,
                                                      int& timePosition)
{
    return videoThumbSvc_->LoadVideoFrame(szFileName, percent, timePosition);
}

std::vector<CImageVideoThumbnail*> CLibPicture::LoadAllVideoThumbnail(
    const wxString& szFileName,
    const bool& compressJpeg,
    const bool& isThumbnail)
{
    return videoThumbSvc_->LoadAllFrames(szFileName, compressJpeg, isThumbnail);
}

std::vector<CImageVideoThumbnail*> CLibPicture::LoadDefaultVideoThumbnail(
    const wxString& szFileName, const int& size)
{
    return videoThumbSvc_->LoadPlaceholders(szFileName, size);
}

// =============================================================================
//  Métadonnées / infos fichier
// =============================================================================

int CLibPicture::GetPictureDimensions(const wxString& fileName,
                                       int& width, int& height, int& rotation)
{
    return loader_->GetDimensions(fileName, width, height, rotation);
}

int CLibPicture::GetNbImage(const wxString& szFileName)
{
    return loader_->GetFrameCount(szFileName);
}

uint32_t CLibPicture::GetFrameDelay(const wxString& szFileName)
{
    return loader_->GetFrameDelay(szFileName);
}

int CLibPicture::GetVideoDuration(const wxString& szFileName)
{
    return MetadataService::GetVideoDuration(szFileName);
}

bool CLibPicture::TestIsVideoValid(const wxString& szFileName)
{
    return MetadataService::IsVideoValid(szFileName);
}

// =============================================================================
//  Détection de format
// =============================================================================

int CLibPicture::TestImageFormat(const wxString& szFileName, const bool& reading)
{
    return FormatDetector::DetectFormat(szFileName, reading);
}

int CLibPicture::TestExtension(const wxString& ext)
{
    return FormatDetector::GetExtensionId(ext);
}

bool CLibPicture::TestIsVideo(const wxString& szFileName)
{
    return FormatDetector::IsVideo(szFileName);
}

bool CLibPicture::TestIsPDF(const wxString& szFileName)
{
    return FormatDetector::IsPDF(szFileName);
}

bool CLibPicture::TestIsPicture(const wxString& szFileName)
{
    return FormatDetector::IsPicture(szFileName);
}

bool CLibPicture::TestIsAnimation(const wxString& szFileName)
{
    return FormatDetector::IsAnimation(szFileName);
}

bool CLibPicture::TestIsExifCompatible(const wxString& filename)
{
    return FormatDetector::IsExifCompatible(filename);
}

// =============================================================================
//  Conversion
// =============================================================================

cv::Mat CLibPicture::mat_from_wx(const wxImage& wx)
{
    return ImageConverter::WxToCvMat(wx);
}

wxImage CLibPicture::ConvertRegardsBitmapToWXImage(cv::Mat& img)
{
    return ImageConverter::CvMatToWx(img);
}

float CLibPicture::CalculPictureRatio(const int& pictureWidth,
                                       const int& pictureHeight,
                                       const int& width,
                                       const int& height)
{
    return ImageConverter::CalcFitRatio(pictureWidth, pictureHeight, width, height);
}

// =============================================================================
//  Image d'erreur / annulation
// =============================================================================

CImageLoadingFormat* CLibPicture::GetCancelPhoto(const wxString& szFileName,
                                                  const int& widthThumbnail,
                                                  const int& heightThumbnail)
{
    return ImageLoader::GetCancelPhoto(szFileName, widthThumbnail, heightThumbnail);
}

// =============================================================================
//  SVG
// =============================================================================

void CLibPicture::DefineSVGSize(const int& width, const int& height)
{
    loader_->SetSvgSize(width, height);
}

// =============================================================================
//  FreeImage lifecycle
// =============================================================================

void CLibPicture::InitFreeImage()
{
    ImageLoader::InitFreeImage();
}

void CLibPicture::UninitFreeImage()
{
    ImageLoader::UninitFreeImage();
}
