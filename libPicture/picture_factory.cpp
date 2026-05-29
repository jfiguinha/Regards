// =============================================================================
// libPicture_refactored.cpp
// Implémentation des couches métier issues du refactoring de CLibPicture
// =============================================================================

#include <header.h>
#include "picture_factory.h"

#ifdef __APPLE__
#include <ReadImage.h>
#endif

#include <wx/filename.h>
#include <wx/progdlg.h>
#include <wx/dir.h>
#include <wx/mstream.h>
#include <wx/image.h>

#define __FREEIMAGE__
#include <FreeImage.h>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>

#include "MetadataExiv2.h"
#include "regards_webp.h"
#include "picture_utility.h"
#include "avif.h"
#include <ximage.h>
#include <ParamInit.h>
#include "ascii.h"
#ifdef ROTDETECT
#include <rotdetect.h>
#endif

#include <ImageVideoThumbnail.h>
#include <ImageLoadingFormat.h>
#include <ConvertUtility.h>
#include <picture_id.h>
#include <LibResource.h>
#include "FFmpegVideoThumb.h"
#include "pfm.h"
#ifdef LIBHEIC
#include <Heic.h>
#endif
#if defined(WIN32)
#include "wic.h"
#endif
#ifdef TURBOJPEG
#include <turbojpeg.h>
#endif
#if defined(LIBRAW)
#include "raw.h"
#endif
#include "jxl.h"
#if defined(FFMPEG)
#include <videothumb.h>
#endif
#if defined(EXIV2)
using namespace Regards::exiv2;
#endif

#include <FileUtility.h>
#include <GifOption.h>
#include <TiffOption.h>
#include <PngOption.h>
#include <webpOption.h>
#include <BmpOption.h>
#include <ExrOption.h>
#include <J2kOption.h>
#include <JxrOption.h>
#include <SqlPhotos.h>
#include <CompressionOption.h>
#include "imageinfo.hpp"
#include "RegardsPDF.h"
#include "MediaInfo.h"

#define TYPE_IMAGE_CXIMAGE      0
#define TYPE_IMAGE_WXIMAGE      1
#define TYPE_IMAGE_REGARDSIMAGE 2
#define OR ||

using namespace Regards::Picture;
using namespace Regards::Video;
using namespace Regards::Sqlite;
using namespace OPENEXR_IMF_INTERNAL_NAMESPACE;
using namespace IMATH_INTERNAL_NAMESPACE;

extern float   clamp(float val, float minval, float maxval);
extern wxImage defaultPicture;

// ── Définitions des membres statiques ────────────────────────────────────────
std::map<wxString, int64_t> MetadataService::videoDurationCache_;

// ── Helper interne FreeImage error handler ───────────────────────────────────
static void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char* message)
{
    printf("\n*** ");
    if (fif != FIF_UNKNOWN && FreeImage_GetFormatFromFIF(fif))
        printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
    printf("%s ***\n", message);
}

// ── Helper interne : détection FreeImage ─────────────────────────────────────
static FREE_IMAGE_FORMAT DetectFreeImageFormat(const char* filename)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filename);
    return fif;
}

// ── Helper interne : dimensions écran / 4 ────────────────────────────────────
static void GetScreenThumbnailSize(int& w, int& h)
{
#ifdef WIN32
    HDC  screen = GetDC(nullptr);
    RECT rcClip;
    GetClipBox(screen, &rcClip);
    ReleaseDC(nullptr, screen);
    w = max(static_cast<int32_t>(rcClip.right  / 4), 200);
    h = max(static_cast<int32_t>(rcClip.bottom / 4), 200);
#else
    w = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, 200);
    h = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 4, 200);
#endif
}


// =============================================================================
//  FormatDetector
// =============================================================================

int FormatDetector::GetExtensionId(const wxString& ext)
{
    return CLibResource::GetExtensionId(ext);
}

int FormatDetector::DetectFormat(const wxString& fileName, bool forReading)
{
    int numExt = 0;
    wxDir testDir(fileName);
    if (!testDir.IsOpened())
    {
        wxFileName fichier(fileName);
        wxString   ext = fichier.GetExt();
        if (ext.size() < 3)
            return 0;

        numExt = GetExtensionId(ext.Lower());
        if (numExt == ASCII && forReading)
            numExt = 0;
    }
    return numExt;
}

bool FormatDetector::IsPicture(const wxString& fileName)
{
    wxFileName fichier(fileName);
    wxString   ext = fichier.GetExt();

    FREE_IMAGE_FORMAT fif = DetectFreeImageFormat(CConvertUtility::ConvertToUTF8(fileName));
    if (fif != FIF_UNKNOWN)
        return true;

    const int id = GetExtensionId(ext.Lower());
    return (id < 100 && id != ANI && id != 0);
}

bool FormatDetector::IsVideo(const wxString& fileName)
{
    wxFileName fichier(fileName);
    const int  id = GetExtensionId(fichier.GetExt().Lower());
    return (id > 100);
}

bool FormatDetector::IsPDF(const wxString& fileName)
{
    wxFileName fichier(fileName);
    return (GetExtensionId(fichier.GetExt().Lower()) == PDF);
}

bool FormatDetector::IsAnimation(const wxString& fileName)
{
    // On s'appuie sur ImageLoader pour GetFrameCount ; ici on fait une version
    // autonome légère : si la vidéo → animation, sinon on délègue au loader.
    if (IsVideo(fileName))
        return true;

    ImageLoader loader;
    return (loader.GetFrameCount(fileName) > 1);
}

bool FormatDetector::IsExifCompatible(const wxString& fileName)
{
    const int fmt = DetectFormat(fileName);
    return (fmt == JPEG || fmt == JXL  || fmt == PNG  || fmt == TIFF ||
            fmt == WEBP || fmt == RAWFILE || fmt == HEIC || fmt == AVIF);
}

bool FormatDetector::HasEmbeddedThumbnail(const wxString& fileName)
{
#if defined(EXIV2)
    const int fmt = DetectFormat(fileName);
    if (fmt == RAWFILE)
        return true;
    if (fmt == TIFF || fmt == JPEG)
    {
        CMetadataExiv2 meta(fileName);
        return meta.HasThumbnail();
    }
#endif
    return false;
}


// =============================================================================
//  ImageConverter
// =============================================================================

cv::Mat ImageConverter::WxToCvMat(const wxImage& wx)
{
    cv::Mat im2(cv::Size(wx.GetWidth(), wx.GetHeight()), CV_8UC3, wx.GetData());
    if (wx.HasAlpha())
    {
        std::vector<cv::Mat> ch;
        cv::split(im2, ch);
        cv::Mat alpha(cv::Size(wx.GetWidth(), wx.GetHeight()), CV_8UC1, wx.GetAlpha());
        ch.push_back(alpha);
        cv::merge(ch, im2);
        cvtColor(im2, im2, cv::COLOR_RGBA2BGRA);
    }
    else
    {
        cvtColor(im2, im2, cv::COLOR_RGB2BGRA);
    }
    return im2;
}

wxImage ImageConverter::CvMatToWx(cv::Mat& img)
{
    wxImage wx;
    cv::Mat im2, alpha;

    if (img.channels() == 1)
        cvtColor(img, im2, cv::COLOR_GRAY2RGB);
    else if (img.channels() == 4)
    {
        cvtColor(img, im2, cv::COLOR_BGRA2RGB);
        cv::Mat ch[4];
        cv::split(img, ch);
        alpha = ch[3];
    }
    else
        cvtColor(img, im2, cv::COLOR_BGR2RGB);

    const long imsize = im2.rows * im2.cols * im2.channels();

    if (img.channels() == 4)
    {
        wx = wxImage(im2.cols, im2.rows, static_cast<unsigned char*>(malloc(imsize)), false);
        memcpy(wx.GetData(), im2.data, imsize);
        unsigned char* wxalpha = new unsigned char[im2.rows * im2.cols];
        memcpy(wxalpha, alpha.data, im2.rows * im2.cols);
        wx.SetAlpha(wxalpha);
    }
    else
    {
        wx = wxImage(im2.cols, im2.rows, static_cast<unsigned char*>(malloc(imsize)), false);
        memcpy(wx.GetData(), im2.data, imsize);
    }
    return wx;
}

float ImageConverter::CalcFitRatio(int pictureWidth, int pictureHeight,
                                    int targetWidth,  int targetHeight)
{
    if (pictureWidth == 0 && pictureHeight == 0)
        return 1.0f;

    float ratio = (pictureWidth > pictureHeight)
        ? static_cast<float>(targetWidth)  / static_cast<float>(pictureWidth)
        : static_cast<float>(targetHeight) / static_cast<float>(pictureHeight);

    if ((pictureHeight * ratio) > targetHeight)
        ratio = static_cast<float>(targetHeight) / static_cast<float>(pictureHeight);
    else if ((pictureWidth * ratio) > targetWidth)
        ratio = static_cast<float>(targetWidth) / static_cast<float>(pictureWidth);

    return ratio;
}

float ImageConverter::CalcScreenThumbnailRatio(int width, int height)
{
    int tw, th;
    GetScreenThumbnailSize(tw, th);

    float ratio = (width > height)
        ? static_cast<float>(tw) / static_cast<float>(width)
        : static_cast<float>(th) / static_cast<float>(height);

    if ((height * ratio) > th)
        ratio = static_cast<float>(th) / static_cast<float>(height);
    if ((width  * ratio) > tw)
        ratio = static_cast<float>(tw) / static_cast<float>(width);

    return ratio;
}

void ImageConverter::AdjustSizeToJpegScalingFactors(int& width, int& height)
{
    int tw, th;
    GetScreenThumbnailSize(tw, th);

    tjscalingfactor* sf_list = nullptr;
    tjscalingfactor  sf      = {1, 1};
    int              nsf     = 0;

    if ((sf_list = tjGetScalingFactors(&nsf)) == nullptr || nsf == 0)
        throw("executing tjGetScalingFactors()");

    float ratio = CalcFitRatio(width, height, tw, th);
    int   match = 0;

    for (int j = 0; j < nsf; j++)
    {
        sf = sf_list[j];
        float local = static_cast<float>(sf.num) / static_cast<float>(sf.denom);
        if (local < ratio)
        {
            if (j > 0) sf = sf_list[j - 1];
            match = 1;
            break;
        }
    }
    if (match == 0)
    {
        sf    = sf_list[nsf - 1];
        match = 1;
    }
    if (match == 1)
    {
        width  = TJSCALED(width,  sf);
        height = TJSCALED(height, sf);
    }
}


// =============================================================================
//  MetadataService
// =============================================================================

void MetadataService::ApplyExifOrientation(const wxString& fileName,
                                            CImageLoadingFormat* bitmap)
{
    if (FormatDetector::IsExifCompatible(fileName))
    {
        CMetadataExiv2 meta(fileName);
        const int orientation = meta.GetOrientation();
        bitmap->SetOrientation(orientation);
    }
}

int MetadataService::GetOrientation(const wxString& fileName)
{
    if (FormatDetector::IsExifCompatible(fileName))
    {
        CMetadataExiv2 meta(fileName);
        return meta.GetOrientation();
    }
    return -1;
}

int MetadataService::GetVideoDuration(const wxString& fileName)
{
    auto it = videoDurationCache_.find(fileName);
    if (it != videoDurationCache_.end())
        return static_cast<int>(it->second);

    int64_t dur = CMediaInfo::GetVideoDuration(fileName);
    videoDurationCache_[fileName] = dur;
    return static_cast<int>(dur);
}

bool MetadataService::IsVideoValid(const wxString& fileName)
{
    int64_t dur = 0;
    auto it = videoDurationCache_.find(fileName);
    if (it != videoDurationCache_.end())
        dur = it->second;
    else
    {
        dur = CMediaInfo::GetVideoDuration(fileName);
        videoDurationCache_[fileName] = dur;
    }

    if (dur > 0)
    {
        CFFmpegVideoThumb player(fileName);
        return player.isOpened();
    }
    return false;
}


// =============================================================================
//  ImageLoader
// =============================================================================

ImageLoader::ImageLoader()
{
#ifdef WIN32
    wic_ = new CWic();
#endif
#ifdef __APPLE__
    readimage_ = new CReadMacOSImage();
#endif
}

ImageLoader::~ImageLoader()
{
#ifdef WIN32
    delete wic_;
#endif
#ifdef __APPLE__
    delete readimage_;
#endif
}

void ImageLoader::SetSvgSize(int width, int height)
{
    svgWidth_  = width;
    svgHeight_ = height;
}

void ImageLoader::InitFreeImage()
{
    FreeImage_Initialise(true);
    FreeImage_SetOutputMessage(&FreeImageErrorHandler);
}

void ImageLoader::UninitFreeImage()
{
    FreeImage_DeInitialise();
}

// -----------------------------------------------------------------------------
// GetCancelPhoto  (static)
// -----------------------------------------------------------------------------
CImageLoadingFormat* ImageLoader::GetCancelPhoto(const wxString& szFileName,
                                                  int thumbW, int thumbH)
{
    wxString             photoCancel = CLibResource::GetPhotoCancel();
    CImageLoadingFormat* bitmap      = new CImageLoadingFormat();
    try
    {
        cv::Mat mat = cv::imread(CConvertUtility::ConvertToStdString(photoCancel),
                                 cv::IMREAD_COLOR);
        if (!mat.empty())
        {
            bitmap->SetFilename(szFileName);
            bitmap->SetPicture(mat);
            if (thumbW > 0 && thumbH > 0)
                bitmap->Resize(thumbW, thumbH, 0);
        }
    }
    catch (cv::Exception& e)
    {
        std::cout << "GetCancelPhoto: " << e.what() << std::endl;
    }
    return bitmap;
}

// -----------------------------------------------------------------------------
// LoadViaFreeImage  (private)
// -----------------------------------------------------------------------------
cv::Mat ImageLoader::LoadViaFreeImage(const char* filename)
{
    cv::Mat result;
#ifdef __FREEIMAGE__
    FIBITMAP* dib     = nullptr;
    FIBITMAP* dibRgba = nullptr;

    FREE_IMAGE_FORMAT fif = DetectFreeImageFormat(filename);
    if (fif == FIF_UNKNOWN)
        return result;

    if (FreeImage_FIFSupportsReading(fif))
        dib = FreeImage_Load(fif, filename);
    if (!dib)
        return result;

    dibRgba = FreeImage_ConvertTo32Bits(dib);
    unsigned int w = FreeImage_GetWidth(dibRgba);
    unsigned int h = FreeImage_GetHeight(dibRgba);

    result = cv::Mat(h, w, CV_8UC4);
    memcpy(result.data, FreeImage_GetBits(dibRgba), w * h * 4);
    cv::flip(result, result, 0);

    FreeImage_Unload(dib);
    FreeImage_Unload(dibRgba);
#endif
    return result;
}

// -----------------------------------------------------------------------------
// GetDimensionsViaFreeImage  (private)
// -----------------------------------------------------------------------------
bool ImageLoader::GetDimensionsViaFreeImage(const char* filename, int& w, int& h)
{
#ifdef __FREEIMAGE__
    FREE_IMAGE_FORMAT fif = DetectFreeImageFormat(filename);
    if (fif == FIF_UNKNOWN)
        return false;

    FIBITMAP* dib = nullptr;
    if (FreeImage_FIFSupportsReading(fif))
        dib = FreeImage_Load(fif, filename);
    if (!dib)
        return false;

    w = FreeImage_GetWidth(dib);
    h = FreeImage_GetHeight(dib);
    FreeImage_Unload(dib);
    return true;
#else
    return false;
#endif
}

// -----------------------------------------------------------------------------
// GetFrameCount
// -----------------------------------------------------------------------------
int ImageLoader::GetFrameCount(const wxString& szFileName)
{
    const int fmt = FormatDetector::DetectFormat(szFileName);

    switch (fmt)
    {
#ifdef LIBHEIC
    case AVIF:
    case HEIC:
        return CHeic::GetNbFrame(CConvertUtility::ConvertToUTF8(szFileName));
#endif
    case PDF:
    {
        bool valid = false;
        return CRegardsPDF::GetNbFrame(szFileName, valid);
    }
    case TIFF:
    case ANI:
        return wxImage::GetImageCount(szFileName, wxBITMAP_TYPE_ANY);
    case WEBP:
        return CRegardsWebp::GetNbFrame(szFileName);
    case PNG:
    case GIF:
    {
        wxFileName fichier(szFileName);
        auto* cx = new CxImage();
        cx->SetRetreiveAllFrames(true);
        cx->Load(CConvertUtility::ConvertToUTF8(szFileName),
                 CxImage::GetTypeIdFromName(fichier.GetExt()));
        int n = cx->GetNumFrames();
        delete cx;
        return n;
    }
    case MPG2: case MPEG: case AVCHD: case WINREC:
    case AVI:  case MP4:  case WEBM:  case MKV:
    case Y4M:  case WMV:  case AV1:   case MOV:
        return CMediaInfo::GetVideoDuration(szFileName);
    default:
        return 1;
    }
}

// -----------------------------------------------------------------------------
// GetFrameDelay
// -----------------------------------------------------------------------------
uint32_t ImageLoader::GetFrameDelay(const wxString& szFileName)
{
    uint32_t  delay = 10;
    const int fmt   = FormatDetector::DetectFormat(szFileName);
    switch (fmt)
    {
#ifdef LIBHEIC
    case AVIF:
    case HEIC:
        return CHeic::GetDelay(CConvertUtility::ConvertToUTF8(szFileName));
#endif
    case GIF:
    {
        auto* cx = new CxImage();
        cx->SetRetreiveAllFrames(true);
        cx->Load(CConvertUtility::ConvertToUTF8(szFileName),
                 CxImage::GetTypeIdFromName("gif"));
        delay = cx->GetFrameDelay();
        delete cx;
        break;
    }
    default:
        break;
    }
    return delay;
}

// -----------------------------------------------------------------------------
// GetDimensions
// -----------------------------------------------------------------------------
int ImageLoader::GetDimensions(const wxString& fileName,
                                int& width, int& height, int& rotation)
{
    const int fmt = FormatDetector::DetectFormat(fileName);
    width = height = rotation = 0;

    // Tentative rapide via imageinfo (header-only)
    auto info = imageinfo::parse<imageinfo::FilePathReader>(
        CConvertUtility::ConvertToStdString(fileName));
    if (info)
    {
        width    = info.size().width;
        height   = info.size().height;
        rotation = 0;
        return 0;
    }

    CxImage* image    = nullptr;
    wxImage  imageWx;
    int      typeImg  = TYPE_IMAGE_CXIMAGE;

    switch (fmt)
    {
    case JXL:
        CJxl::GetDimensions(fileName, width, height);
        break;

    case HDR:
    {
        cv::Mat hdr = cv::imread(CConvertUtility::ConvertToStdString(fileName), -1);
        width  = hdr.cols;
        height = hdr.rows;
        break;
    }
    case EXR:
        try
        {
            RgbaInputFile f(CConvertUtility::ConvertToUTF8(fileName));
            Box2i dw = f.dataWindow();
            width    = dw.max.x - dw.min.x + 1;
            height   = dw.max.y - dw.min.y + 1;
        }
        catch (...) {}
        break;

    case PFM:
        CPfm::GetDimensions(fileName, width, height);
        break;

    case WEBP:
        typeImg = TYPE_IMAGE_REGARDSIMAGE;
        CRegardsWebp::GetPictureDimension(fileName, width, height);
        break;

    case JPEG:
#ifdef TURBOJPEG
    {
        typeImg = TYPE_IMAGE_REGARDSIMAGE;
        size_t   jpegSize = 0;
        uint8_t* compressed = CPictureUtility::readfile(fileName, jpegSize);
        if (compressed && jpegSize > 0)
        {
            int        sub = 0;
            tjhandle   dec = tjInitDecompress();
            tjDecompressHeader2(dec, compressed, jpegSize, &width, &height, &sub);
            tjDestroy(dec);
            delete[] compressed;
        }
        break;
    }
#else
        image = new CxImage(fileName.ToStdWstring(),
                            CxImage::GetTypeIdFromName("jpg"), true);
        break;
#endif

    case JBIG:
        image = new CxImage(fileName.ToStdWstring(),
                            CxImage::GetTypeIdFromName("jbg"), true);
        break;

    case PDF:
    {
        CRegardsPDF pdf(fileName);
        imageWx  = pdf.GetPicture(0);
        typeImg  = TYPE_IMAGE_WXIMAGE;
        break;
    }
    case IFF:
        imageWx.LoadFile(fileName, wxBITMAP_TYPE_IFF);
        typeImg = TYPE_IMAGE_WXIMAGE;
        break;

    case XPM:
        imageWx.LoadFile(fileName, wxBITMAP_TYPE_XPM);
        typeImg = TYPE_IMAGE_WXIMAGE;
        break;

    case RAWFILE:
        typeImg = TYPE_IMAGE_REGARDSIMAGE;
        CRaw::GetDimensions(fileName, width, height);
        break;

    case SVG:
        typeImg = TYPE_IMAGE_REGARDSIMAGE;
        width   = svgWidth_;
        height  = svgHeight_;
        break;

    case MPG2: case MPEG: case AVCHD: case WINREC:
    case AVI:  case MP4:  case WMV:   case WEBM:
    case MKV:  case AV1:  case Y4M:   case MOV:
        CMediaInfo::GetVideoDimensions(fileName, width, height);
        break;

#ifdef LIBHEIC
    case AVIF:
    case HEIC:
        typeImg = TYPE_IMAGE_REGARDSIMAGE;
        CHeic::GetPictureDimension(CConvertUtility::ConvertToUTF8(fileName),
                                   width, height);
        break;
#endif
    default:
        GetDimensionsViaFreeImage(CConvertUtility::ConvertToUTF8(fileName),
                                  width, height);
        break;
    }

    if (typeImg == TYPE_IMAGE_CXIMAGE && image)
    {
        width    = image->GetWidth();
        height   = image->GetHeight();
        EXIFINFO* exif = image->GetExifInfo();
        rotation = exif ? exif->Orientation : 0;
        delete image;
    }
    else if (typeImg == TYPE_IMAGE_WXIMAGE && imageWx.IsOk())
    {
        width  = imageWx.GetWidth();
        height = imageWx.GetHeight();
    }

    return 0;
}

// -----------------------------------------------------------------------------
// Load  (void version – cœur du chargement)
// -----------------------------------------------------------------------------
void ImageLoader::Load(const wxString& fileName, bool isThumbnail,
                       int numPicture, CImageLoadingFormat* bitmap)
{
    const int fmt      = FormatDetector::DetectFormat(fileName);
    bool      applyExif = true;

    bitmap->SetFilename(fileName);

    try
    {
        switch (fmt)
        {
        // ── HEIC / AVIF ──────────────────────────────────────────────────────
#ifdef LIBHEIC
        case AVIF:
        case HEIC:
        {
            cv::Mat picture;
            int     orientation = 0;
            applyExif = false;
            bool resize    = false;
            int  thumbW = 0, thumbH = 0;

            if (numPicture == 0)
            {
                if (isThumbnail)
                {
                    picture   = CHeic::GetThumbnailPicture(
                        CConvertUtility::ConvertToUTF8(fileName), orientation);
                    applyExif = true;
                }

                if (picture.empty())
                {
                    if (fmt == HEIC)
                    {
                        picture = CHeic::GetPicture(
                            CConvertUtility::ConvertToUTF8(fileName),
                            orientation, isThumbnail);
                    }
                    else // AVIF
                    {
                        applyExif = true;
                        if (isThumbnail)
                        {
                            CMetadataExiv2 meta(fileName);
                            orientation = meta.GetOrientation();

                            int w = 0, h = 0;
                            CHeic::GetPictureDimension(
                                CConvertUtility::ConvertToUTF8(fileName), w, h);

                            float ratio = 1.0f;
                            if (orientation > 4)
                            {
                                ratio  = ImageConverter::CalcScreenThumbnailRatio(h, w);
                                thumbH = static_cast<int>(w * ratio);
                                thumbW = static_cast<int>(h * ratio);
                            }
                            else
                            {
                                ratio  = ImageConverter::CalcScreenThumbnailRatio(w, h);
                                thumbW = static_cast<int>(w * ratio);
                                thumbH = static_cast<int>(h * ratio);
                            }

                            picture = CAvif::GetPictureThumb(
                                CConvertUtility::ConvertToUTF8(fileName),
                                thumbW, thumbH);
                            if (picture.empty())
                            {
                                int delay = 4;
                                picture   = CHeic::GetPicture(
                                    CConvertUtility::ConvertToUTF8(fileName),
                                    delay, numPicture);
                                resize    = true;
                                applyExif = false;
                            }
                        }
                        else
                        {
                            CMetadataExiv2 meta(fileName);
                            orientation = meta.GetOrientation();

                            int angle = 0;
                            picture   = CAvif::GetPicture(
                                CConvertUtility::ConvertToUTF8(fileName), angle);
                            if (picture.empty())
                            {
                                int delay = 4;
                                picture   = CHeic::GetPicture(
                                    CConvertUtility::ConvertToUTF8(fileName),
                                    delay, numPicture);
                            }
                            if (orientation == -1 && angle > 0)
                            {
                                static std::vector<int> tabConvert = {0, 2, 1, 0};
                                cv::rotate(picture, picture, tabConvert[angle]);
                            }
                        }
                    }
                }
            }
            else
            {
                int delay = 4;
                picture   = CHeic::GetPicture(
                    CConvertUtility::ConvertToUTF8(fileName), delay, numPicture);
            }

            if (!picture.empty())
            {
                bitmap->SetPicture(picture);
                bitmap->SetFilename(fileName);
                if (resize)
                    bitmap->Resize(thumbW, thumbH, 0);
            }
            break;
        }
#endif // LIBHEIC

        // ── JXL ─────────────────────────────────────────────────────────────
        case JXL:
        {
            cv::Mat raw = CJxl::GetPicture(fileName);
            cv::Mat out;
            raw.convertTo(out, CV_8UC4, 255);
            bitmap->SetPicture(out);
            bitmap->ConvertToBGR();
            break;
        }

        // ── PFM ─────────────────────────────────────────────────────────────
        case PFM:
        {
            cv::Mat raw = CPfm::ReadFilePFM(fileName, isThumbnail);
            cv::Mat out;
            raw.convertTo(out, CV_8UC4, 255);
            bitmap->SetPicture(out);
            bitmap->Flip();
            break;
        }

        // ── EXR ─────────────────────────────────────────────────────────────
        case EXR:
            try
            {
                Array2D<Rgba> pixels;
                RgbaInputFile f(CConvertUtility::ConvertToUTF8(fileName));
                Box2i dw   = f.dataWindow();
                int   w    = dw.max.x - dw.min.x + 1;
                int   h    = dw.max.y - dw.min.y + 1;
                pixels.resizeErase(h, w);
                f.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * w, 1, w);
                f.readPixels(dw.min.y, dw.max.y);

                if (w > 0 && h > 0)
                {
                    cv::Mat picture(h, w, CV_8UC4);
                    uint8_t* d = picture.data;
                    int k = 0;
                    for (int i = 0; i < h; ++i)
                        for (int j = 0; j < w; ++j, k += 4)
                        {
                            d[k]   = static_cast<uint8_t>(
                                clamp(float(pixels[i][j].b), 0.f, 1.f) * 255.f);
                            d[k+1] = static_cast<uint8_t>(
                                clamp(float(pixels[i][j].g), 0.f, 1.f) * 255.f);
                            d[k+2] = static_cast<uint8_t>(
                                clamp(float(pixels[i][j].r), 0.f, 1.f) * 255.f);
                            d[k+3] = static_cast<uint8_t>(
                                clamp(float(pixels[i][j].a), 0.f, 1.f) * 255.f);
                        }
                    bitmap->SetPicture(picture);
                    bitmap->SetFilename(fileName);
                }
            }
            catch (...) {}
            break;

        // ── JPEG / BMP / WEBP / PNM / PPM ───────────────────────────────────
        case JPEG:
        case PNM:
        case WEBP:
        case BMP:
        case PPM:
            try
            {
                if (isThumbnail && fmt == JPEG)
                {
                    size_t   jpegSize = 0;
                    uint8_t* compressed = CPictureUtility::readfile(fileName, jpegSize);
                    if (compressed && jpegSize > 0)
                    {
                        int jpegSubsamp, w = 0, h = 0;
                        tjhandle dec = tjInitDecompress();
                        tjDecompressHeader2(dec, compressed, jpegSize,
                                            &w, &h, &jpegSubsamp);
                        ImageConverter::AdjustSizeToJpegScalingFactors(w, h);

                        cv::Mat picture(h, w, CV_8UC4);
                        try
                        {
                            tjDecompress2(dec, compressed, jpegSize,
                                          picture.data, w, 0, h,
                                          TJPF_BGRX, TJFLAG_FASTDCT);
                        }
                        catch (...) {}
                        tjDestroy(dec);
                        delete[] compressed;

                        bitmap->SetPicture(picture);
                        bitmap->SetFilename(fileName);
                    }
                }
                else
                {
                    cv::Mat mat = cv::imread(
                        CConvertUtility::ConvertToStdString(fileName),
                        cv::IMREAD_COLOR | cv::IMREAD_IGNORE_ORIENTATION);
                    if (!mat.empty())
                    {
                        bitmap->SetFilename(fileName);
                        bitmap->SetPicture(mat);
                    }
                }
            }
            catch (cv::Exception& e)
            {
                std::cout << "ImageLoader::Load JPEG: " << e.what() << std::endl;
            }
            break;

        // ── PNG ──────────────────────────────────────────────────────────────
        case PNG:
            try
            {
                cv::Mat mat = cv::imread(
                    CConvertUtility::ConvertToStdString(fileName),
                    cv::IMREAD_UNCHANGED);
                if (!mat.empty())
                {
                    bitmap->SetFilename(fileName);
                    bitmap->SetPicture(mat);
                }
            }
            catch (cv::Exception& e)
            {
                std::cout << "ImageLoader::Load PNG: " << e.what() << std::endl;
            }
            break;

        // ── TIFF ─────────────────────────────────────────────────────────────
        case TIFF:
            try
            {
                cv::Mat mat = cv::imread(
                    CConvertUtility::ConvertToStdString(fileName),
                    cv::IMREAD_COLOR | cv::IMREAD_IGNORE_ORIENTATION);
                if (!mat.empty())
                {
                    bitmap->SetFilename(fileName);
                    bitmap->SetPicture(mat);
                    applyExif = false;
                }
            }
            catch (cv::Exception& e)
            {
                std::cout << "ImageLoader::Load TIFF: " << e.what() << std::endl;
            }
            break;

        // ── HDR ──────────────────────────────────────────────────────────────
        case HDR:
            try
            {
                cv::Mat hdr = cv::imread(
                    CConvertUtility::ConvertToStdString(fileName), -1);
                if (!hdr.empty())
                {
                    cv::Ptr<cv::TonemapReinhard> tm =
                        cv::createTonemapReinhard(1.0f);
                    tm->process(hdr, hdr);
                    hdr.convertTo(hdr, CV_8UC3, 255);
                    bitmap->SetFilename(fileName);
                    bitmap->SetPicture(hdr);
                }
            }
            catch (cv::Exception& e)
            {
                std::cout << "ImageLoader::Load HDR: " << e.what() << std::endl;
            }
            break;

        // ── ANI / XPM / ICO / CUR / IFF ──────────────────────────────────────
        case ANI:
        case XPM:
        case ICO:
        case CUR:
        case IFF:
        {
            wxImage img;
            img.LoadFile(fileName, wxBITMAP_TYPE_ANY);
            bitmap->SetPicture(img);
            break;
        }

        // ── SVG ──────────────────────────────────────────────────────────────
        case SVG:
        {
            wxImage img = CLibResource::CreatePictureFromSVGFilename(
                fileName, svgWidth_, svgHeight_);

            wxBitmap   memBmp(svgWidth_, svgHeight_);
            wxMemoryDC dc(memBmp);
            wxBrush    brush(*wxWHITE, wxBRUSHSTYLE_SOLID);
            dc.SetBrush(brush);
            dc.SetPen(wxPen(*wxWHITE, 1));
            dc.DrawRectangle(0, 0, svgWidth_, svgHeight_);
            dc.SetPen(wxNullPen);
            dc.SetBrush(wxNullBrush);
            dc.DrawBitmap(img, 0, 0);
            dc.SelectObject(wxNullBitmap);
            img.Destroy();

            wxImage local = memBmp.ConvertToImage();
            bitmap->SetPicture(local);
            break;
        }

        // ── RAW ──────────────────────────────────────────────────────────────
#if defined(LIBRAW)
        case RAWFILE:
            CRaw::LoadPicture(fileName, bitmap);
            if (bitmap)
                bitmap->SetFilename(fileName);
            applyExif = false;
            break;
#endif

        // ── JBIG ─────────────────────────────────────────────────────────────
        case JBIG:
        {
            auto* cx = new CxImage(fileName.ToStdWstring(),
                                   CxImage::GetTypeIdFromName("jbg"));
            bitmap->SetPicture(cx);
            delete cx;
            break;
        }

        // ── PDF ───────────────────────────────────────────────────────────────
        case PDF:
        {
            CRegardsPDF pdf(fileName);
            wxImage     img = pdf.GetPicture(numPicture);
            bitmap->SetPicture(img);
            break;
        }

        // ── GIF ───────────────────────────────────────────────────────────────
        case GIF:
        {
            wxFileName fichier(fileName);
            auto* cx = new CxImage();
            cx->SetRetreiveAllFrames(true);
            cx->Load(CConvertUtility::ConvertToUTF8(fileName),
                     CxImage::GetTypeIdFromName(fichier.GetExt()));
            if (cx->GetNumFrames() > 1)
            {
                CxImage* frame = cx->GetFrame(numPicture);
                bitmap->SetPicture(frame);
            }
            else
                bitmap->SetPicture(cx);
            delete cx;
            break;
        }

        // ── Vidéo ─────────────────────────────────────────────────────────────
        case MPG2: case MPEG: case AVCHD: case WINREC:
        case AVI:  case MP4:  case WMV:   case WEBM:
        case MKV:  case AV1:  case Y4M:   case MOV:
        {
            CVideoThumb* thumb = new CVideoThumb(fileName);
            int          orientation = 0;
            const int    percent     =
                static_cast<int>((static_cast<float>(numPicture) / 20.f) * 100.f);
            cv::Mat mat = thumb->GetVideoFramePercent(percent, 0, 0);
            if (mat.empty())
                mat = ImageConverter::WxToCvMat(defaultPicture);
            orientation = thumb->GetOrientation();
            bitmap->SetPicture(mat);
            bitmap->SetOrientation(orientation);
            bitmap->SetFilename(fileName);
            delete thumb;
            break;
        }

        // ── Fallback FreeImage ────────────────────────────────────────────────
        default:
        {
            cv::Mat mat = LoadViaFreeImage(
                CConvertUtility::ConvertToUTF8(fileName));
            if (!mat.empty())
            {
                bitmap->SetPicture(mat);
                bitmap->SetFilename(fileName);
            }
            break;
        }
        } // switch

        // ── Orientation EXIF ─────────────────────────────────────────────────
        if (applyExif)
        {
            MetadataService meta;
            meta.ApplyExifOrientation(fileName, bitmap);
        }

#ifdef ROTDETECT
        {
            int orientation = bitmap->GetOrientation();
            if ((orientation == -1 || orientation == 1) && !isThumbnail)
            {
                ThumbnailService thumbSvc;
                CImageLoadingFormat* thumb = thumbSvc.LoadThumbnail(fileName);
                float            result[4];
                CRotDetect       rotDetect;
                rotDetect.rotdetect(thumb, result, true);
                CRotDetect::rotation rot = rotDetect.analyzeResult(result);
                int exifOri = 1;
                switch (rot)
                {
                case CRotDetect::ROTATED90CW:  exifOri = 6; break;
                case CRotDetect::ROTATED90CCW: exifOri = 5; break;
                default:                       exifOri = 1; break;
                }
                bitmap->SetOrientation(exifOri);
                delete thumb;
            }
        }
#endif
    }
    catch (...) {}

    // Fallback image d'erreur si le chargement a échoué
    if (!bitmap->IsOk())
    {
        cv::Mat pic = cv::imread(
            CConvertUtility::ConvertToStdString(CLibResource::GetPhotoCancel()));
        bitmap->SetPicture(pic);
        bitmap->SetFilename(fileName);
    }
}

// -----------------------------------------------------------------------------
// Load  (ptr version)
// -----------------------------------------------------------------------------
CImageLoadingFormat* ImageLoader::Load(const wxString& fileName,
                                        bool isThumbnail, int numPicture)
{
    CImageLoadingFormat* bitmap = new CImageLoadingFormat();
    Load(fileName, isThumbnail, numPicture, bitmap);
    return bitmap;
}

// -----------------------------------------------------------------------------
// LoadToBGRA
// -----------------------------------------------------------------------------
CImageLoadingFormat* ImageLoader::LoadToBGRA(const wxString& fileName,
                                              bool& ok,
                                              int resizeWidth, int resizeHeight)
{
    ImageLoader          inner;
    CImageLoadingFormat* img = inner.Load(fileName);

    if (img)
    {
        if (resizeWidth != 0 && resizeHeight != 0)
        {
            float ratio = ImageConverter::CalcFitRatio(
                img->GetWidth(), img->GetHeight(), resizeWidth, resizeHeight);
            img->Resize(static_cast<int>(img->GetWidth()  * ratio),
                        static_cast<int>(img->GetHeight() * ratio), 1);
        }
        ok = true;
    }
    else
        ok = false;

    return img;
}


// =============================================================================
//  ImageSaver
// =============================================================================

int ImageSaver::AskSaveOptions(int format, int& option, int& quality)
{
    int ret = 0;
    switch (format)
    {
    case BMP:
    {
        BmpOption dlg(nullptr);
        dlg.ShowModal();
        if (dlg.IsOk()) { quality = dlg.CompressionOption(); ret = 1; }
        break;
    }
    case WEBP:
    {
        WebpOption dlg(nullptr);
        dlg.ShowModal();
        if (dlg.IsOk()) { quality = dlg.CompressionLevel(); ret = 1; }
        break;
    }
    case PDF:
        CRegardsPDF::SavePictureOption(option, quality);
        break;
    case TIFF:
    {
        TiffOption dlg(nullptr);
        dlg.ShowModal();
        if (dlg.IsOk()) { option = dlg.CompressionOption(); ret = 1; }
        break;
    }
    case PNG:
    {
        PngOption dlg(nullptr);
        dlg.ShowModal();
        if (dlg.IsOk()) { option = dlg.CompressionOption(); ret = 1; }
        break;
    }
    case GIF:
    {
        GifOption dlg(nullptr);
        dlg.ShowModal();
        if (dlg.IsOk()) { option = dlg.CompressionOption(); ret = 1; }
        break;
    }
    case JPEG:
    case HEIC:
    case AVIF:
    case JPEG2000:
    {
        CompressionOption dlg(nullptr);
        dlg.ShowModal();
        if (dlg.IsOk()) { option = 0; quality = dlg.CompressionLevel(); ret = 1; }
        break;
    }
    case JP2:
    case J2K:
    {
        J2kOption dlg(nullptr);
        dlg.ShowModal();
        if (dlg.IsOk())
        {
            option  = dlg.CompressionOption();
            quality = dlg.CompressionLevel();
            ret = 1;
        }
        break;
    }
    case EXR:
    {
        ExrOption dlg(nullptr);
        dlg.ShowModal();
        if (dlg.IsOk()) { option = dlg.CompressionOption(); ret = 1; }
        break;
    }
    case JXR:
    {
        JxrOption dlg(nullptr);
        dlg.ShowModal();
        if (dlg.IsOk()) { option = dlg.CompressionOption(); quality = dlg.CompressionLevel(); ret = 1; }
        break;
    }
    case ASCII: case TGA: case PCX: case MNG:
    case PPM:   case XPM: case IFF: case PFM:
    case JXL:   case PNM: case JPC:
        ret = 1;
        break;
    default:
        break;
    }
    return ret;
}

int ImageSaver::Save(const wxString& fileName, CImageLoadingFormat* bitmap,
                      int option, int quality)
{
    // Applique les transformations en attente (rotation/flip depuis la BDD)
    CSqlPhotos photos;
    int exifId = photos.GetPhotoExif(fileName);
    if (exifId != -1)
    {
        int angle = 0, flipH = 0, flipV = 0;
        photos.GetAngleAndFlip(exifId, angle, flipH, flipV);
        bitmap->SetRotation(angle);
        if (flipV) bitmap->Flip();
        if (flipH) bitmap->Mirror();
    }

    const int iFormat = FormatDetector::DetectFormat(fileName, false);
    const wxString errTitle = CLibResource::LoadStringFromResource(L"informationserror", 1);

    switch (iFormat)
    {
    case ASCII:
        CBitmapToAscii::SaveToAscii(bitmap, CConvertUtility::ConvertToStdString(fileName));
        break;

    case PFM:
        CPfm::WriteFilePFM(bitmap->GetFloatImage(), fileName, 1.0f);
        break;

    case JXL:
        CJxl::WriteFile(bitmap->GetFloatImage(), fileName);
        break;

    case BMP:
    {
        int _opt = (option == 1) ? BMP_SAVE_RLE : BMP_DEFAULT;
        FIBITMAP* fi = bitmap->GetFreeImage();
        if (!FreeImage_Save(FIF_BMP, fi, fileName, _opt))
            wxMessageBox(CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1),
                         errTitle, wxICON_ERROR);
        FreeImage_Unload(fi);
        break;
    }
    case J2K:
    {
        int _opt = (option == 1) ? J2K_DEFAULT : (100 - quality);
        FIBITMAP* fi = bitmap->GetFreeImage();
        if (!FreeImage_Save(FIF_J2K, fi, fileName, _opt))
            wxMessageBox(CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1),
                         errTitle, wxICON_ERROR);
        FreeImage_Unload(fi);
        break;
    }
    case JPEG2000:
    case JP2:
    {
        int _opt = (option == 1) ? JP2_DEFAULT : (100 - quality);
        FIBITMAP* fi = bitmap->GetFreeImage();
        if (!FreeImage_Save(FIF_JP2, fi, fileName, _opt))
            wxMessageBox(CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1),
                         errTitle, wxICON_ERROR);
        FreeImage_Unload(fi);
        break;
    }
    case JXR:
    {
        int _opt = quality;
        switch (option)
        {
        case 1: _opt = JXR_DEFAULT;      break;
        case 2: _opt = JXR_LOSSLESS;     break;
        case 3: _opt = JXR_PROGRESSIVE;  break;
        default: _opt = 100 - quality;   break;
        }
        FIBITMAP* fi = bitmap->GetFreeImage();
        if (!FreeImage_Save(FIF_JXR, fi, fileName, _opt))
            wxMessageBox(CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1),
                         errTitle, wxICON_ERROR);
        FreeImage_Unload(fi);
        break;
    }
    case EXR:
    {
        int _opt = EXR_DEFAULT;
        switch (option)
        {
        case 1: _opt = EXR_FLOAT;  break;
        case 2: _opt = EXR_NONE;   break;
        case 3: _opt = EXR_ZIP;    break;
        case 4: _opt = EXR_PIZ;    break;
        case 5: _opt = EXR_PXR24; break;
        case 6: _opt = EXR_B44;    break;
        case 7: _opt = EXR_LC;     break;
        }
        FIBITMAP* fi     = bitmap->GetFreeImage();
        FIBITMAP* fiRgba = FreeImage_ConvertToRGBAF(fi);
        if (!FreeImage_Save(FIF_EXR, fiRgba, fileName, _opt))
            wxMessageBox(CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1),
                         errTitle, wxICON_ERROR);
        FreeImage_Unload(fiRgba);
        FreeImage_Unload(fi);
        break;
    }
    case WEBP:
    {
        int _opt = quality;
        switch (option)
        {
        case 1: _opt = WEBP_DEFAULT;   break;
        case 2: _opt = WEBP_LOSSLESS;  break;
        default: _opt = 100 - quality; break;
        }
        CRegardsWebp::SavePicture(fileName, bitmap->GetMatrix().getMat(), _opt);
        break;
    }
    case TGA:
    {
        FIBITMAP* fi = bitmap->GetFreeImage();
        if (!FreeImage_Save(FIF_TARGA, fi, fileName, 0))
            wxMessageBox(CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1),
                         errTitle, wxICON_ERROR);
        FreeImage_Unload(fi);
        break;
    }
    case MNG:
    {
        FIBITMAP* fi = bitmap->GetFreeImage();
        if (!FreeImage_Save(FIF_MNG, fi, fileName, 0))
            wxMessageBox(CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1),
                         errTitle, wxICON_ERROR);
        FreeImage_Unload(fi);
        break;
    }
    case TIFF:
    {
        int _opt = TIFF_DEFAULT;
        switch (option)
        {
        case 1: _opt = TIFF_DEFLATE; break;
        case 2: _opt = TIFF_LZW;    break;
        case 3: _opt = TIFF_JPEG;   break;
        case 4: _opt = TIFF_NONE;   break;
        }
        FIBITMAP* fi = bitmap->GetFreeImage();
        if (!FreeImage_Save(FIF_TIFF, fi, fileName, _opt))
            wxMessageBox(CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1),
                         errTitle, wxICON_ERROR);
        FreeImage_Unload(fi);
        break;
    }
    case PNG:
    {
        CxImage img = bitmap->GetCxImage();
        img.SetCodecOption(option, CXIMAGE_FORMAT_PNG);
        img.Save(CConvertUtility::ConvertToUTF8(fileName),
                 CxImage::GetTypeIdFromName("png"));
        break;
    }
    case GIF:
    {
        CxImage img = bitmap->GetCxImage();
        img.SetCodecOption(option, CXIMAGE_FORMAT_GIF);
        img.Save(CConvertUtility::ConvertToUTF8(fileName),
                 CxImage::GetTypeIdFromName("gif"));
        wxString err = img.GetLastError();
        if (!err.empty())
            wxMessageBox(err, errTitle, wxOK | wxICON_ERROR);
        break;
    }
    case AVIF:
    case HEIC:
    {
        wxLogNull logNo;
        uint8_t*     data    = nullptr;
        unsigned int size    = 0;
        bool         hasExif = false;
        cv::Mat      img     = bitmap->GetMatrix().getMat();

        CMetadataExiv2 srcMeta(bitmap->GetFilename());
        if (srcMeta.HasExif())
        {
            if (FormatDetector::DetectFormat(bitmap->GetFilename()) == RAWFILE)
                srcMeta.SetOrientation(0);

            wxString tmp = CFileUtility::GetTempFile("temp_exif.jpg");
            cv::imwrite(CConvertUtility::ConvertToStdString(tmp), img);
            srcMeta.CopyMetadata(tmp);

            CMetadataExiv2 tmpMeta(tmp);
            tmpMeta.GetMetadataBuffer(data, size);
            if (size > 0)
            {
                data = new uint8_t[size + 1];
                tmpMeta.GetMetadataBuffer(data, size);
                hasExif = true;
            }
        }
        CHeic::SavePicture(CConvertUtility::ConvertToUTF8(fileName),
                           iFormat, img, data, size, quality, hasExif);
        if (data) delete[] data;
        break;
    }
    case JPEG:
    {
        wxLogNull logNo;
        wxImage img = bitmap->GetwxImage();
        img.SetOption("wxIMAGE_OPTION_QUALITY", quality);
        img.SaveFile(fileName, wxBITMAP_TYPE_JPEG);
        break;
    }
    case PCX:
    {
        wxImage img = bitmap->GetwxImage();
        img.SaveFile(fileName, wxBITMAP_TYPE_PCX);
        break;
    }
    case PPM:
    {
        cv::Mat dest = bitmap->GetMatrix().getMat();
        cv::cvtColor(dest, dest, cv::COLOR_BGRA2BGR);
        cv::imwrite(CConvertUtility::ConvertToStdString(fileName), dest);
        break;
    }
    case PDF:
        CRegardsPDF::SavePictureToPdf(fileName, bitmap, option, quality);
        break;

    case XPM:
    {
        wxImage img = bitmap->GetwxImage();
        img.SaveFile(fileName, wxBITMAP_TYPE_XPM);
        break;
    }
    default:
        break;
    }

    // Copie des métadonnées EXIF vers le fichier de destination
    if (FormatDetector::IsExifCompatible(fileName) &&
        wxFileName::FileExists(fileName) &&
        iFormat != AVIF && iFormat != HEIC)
    {
        CMetadataExiv2 srcMeta(bitmap->GetFilename());
        srcMeta.CopyMetadata(fileName);
    }

    // Remet l'orientation à 0 pour éviter la double rotation
    if (wxFileName::FileExists(fileName) &&
        FormatDetector::IsExifCompatible(fileName))
    {
        try
        {
            CMetadataExiv2 dstMeta(fileName);
            dstMeta.SetOrientation(0);
        }
        catch (...) {}
    }

    if (!wxFileName::FileExists(fileName))
        wxMessageBox(CLibResource::LoadStringFromResource(L"ImpossibleOpenFile", 1),
                     errTitle, wxOK | wxICON_ERROR);

    return 0;
}

int ImageSaver::Save(const wxString& fileName, CImageLoadingFormat* bitmap)
{
    const int iFormat = FormatDetector::DetectFormat(fileName, false);
    int option = 0, quality = 0;
    if (AskSaveOptions(iFormat, option, quality) == 1)
        Save(fileName, bitmap, option, quality);
    return 0;
}

int ImageSaver::Convert(const wxString& fileNameIn, const wxString& fileNameOut)
{
    int option = 0, quality = 0;
    const int iFormat = FormatDetector::DetectFormat(fileNameOut, false);
    if (AskSaveOptions(iFormat, option, quality) != 1)
        return 0;

    ImageLoader loader;
    const int   nbFrames = loader.GetFrameCount(fileNameIn);

    wxProgressDialog dlg("Export File", "Checking...", nbFrames, nullptr,
                         wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
    dlg.Update(0, "in progress");

    for (int i = 0; i < nbFrames; ++i)
    {
        wxString msg = "In progress : " + std::to_string(i + 1) +
                       "/" + std::to_string(nbFrames);
        if (!dlg.Update(i, msg))
            break;

        CImageLoadingFormat* pic = loader.Load(fileNameIn, false, i);

        wxString outPath;
        if (nbFrames > 1)
        {
            wxFileName fn(fileNameOut);
            outPath = fn.GetPathWithSep() + fn.GetName() +
                      "_page_" + std::to_string(i + 1) +
                      "." + fn.GetExt();
        }
        else
            outPath = fileNameOut;

        Save(outPath, pic, option, quality);
        delete pic;
    }
    return 0;
}


// =============================================================================
//  ThumbnailService
// =============================================================================

CImageLoadingFormat* ThumbnailService::LoadThumbnail(const wxString& fileName,
                                                      bool fromExifOnly)
{
    CImageLoadingFormat* imageLoading = nullptr;
    bool                 notThumbnail = false;

    int tw, th;
    GetScreenThumbnailSize(tw, th);

    const int fmt = FormatDetector::DetectFormat(fileName);

#if defined(EXIV2)

#ifdef LIBRAW
    if (fmt == RAWFILE)
    {
        bool isFromExif = false;
        imageLoading = CRaw::GetThumbnail(fileName, true, isFromExif);
        if (imageLoading && imageLoading->IsOk())
            imageLoading->Resize(tw, th, 0);
        else
            notThumbnail = true;
    }
    else if (FormatDetector::IsVideo(fileName))
    {
        ImageLoader loader;
        imageLoading = loader.Load(fileName, true);
        if (imageLoading && imageLoading->IsOk())
            imageLoading->Resize(tw, th, 0);
    }
    else if (fmt == HEIC || fmt == AVIF)
    {
        int     angle  = 0;
        cv::Mat bm = CHeic::GetThumbnailPicture(
            CConvertUtility::ConvertToUTF8(fileName), angle);
        if (!bm.empty())
        {
            imageLoading = new CImageLoadingFormat();
            imageLoading->SetFilename(fileName);
            imageLoading->SetPicture(bm);
            imageLoading->SetRotation(angle);
        }
        else
        {
            ImageLoader loader;
            imageLoading = loader.Load(fileName, true);
        }
    }
    else
    {
#endif // LIBRAW

        int        orientation = -1;
        wxString   extension;
        wxImage    jpegImage;

        CMetadataExiv2 meta(fileName);
        if (meta.HasThumbnail())
            jpegImage = meta.DecodeThumbnail(extension, orientation);

        if (!jpegImage.IsOk() && !fromExifOnly && fmt == JPEG)
        {
            ImageLoader loader;
            imageLoading = loader.Load(fileName, true);
        }
        else if (jpegImage.IsOk() && jpegImage.GetWidth() > 0)
        {
            imageLoading = new CImageLoadingFormat();
            imageLoading->SetFilename(fileName);
            imageLoading->SetPicture(jpegImage);
            imageLoading->SetOrientation(orientation);
        }
        else
            notThumbnail = true;

#ifdef LIBRAW
    }
#endif

#endif // EXIV2

    if (notThumbnail)
    {
        ImageLoader loader;
        imageLoading = loader.Load(fileName, true);
        if (imageLoading && imageLoading->IsOk())
            imageLoading->Resize(tw, th, 1);
    }

    if (!imageLoading || !imageLoading->IsOk())
    {
        if (imageLoading) delete imageLoading;
        imageLoading = ImageLoader::GetCancelPhoto(fileName, tw, th);
    }

    return imageLoading;
}


// =============================================================================
//  VideoThumbnailService
// =============================================================================

CImageLoadingFormat* VideoThumbnailService::LoadVideoFrame(
    const wxString& fileName, int percent, int& timePosition)
{
    const int fmt = FormatDetector::DetectFormat(fileName);

    int tw, th;
    GetScreenThumbnailSize(tw, th);

    CImageLoadingFormat* bitmap = nullptr;
    try
    {
        switch (fmt)
        {
        case MPG2: case MPEG: case AVCHD: case WINREC:
        case AVI:  case MP4:  case WMV:   case WEBM:
        case MKV:  case Y4M:  case MOV:   case AV1:
        {
            bitmap = new CImageLoadingFormat();
            bitmap->SetFilename(fileName);
            CVideoThumb* thumb = new CVideoThumb(fileName);
            cv::Mat mat = thumb->GetVideoFramePercent(percent, tw, th);
            if (mat.empty())
                mat = ImageConverter::WxToCvMat(defaultPicture);
            bitmap->SetPicture(mat);
            bitmap->SetOrientation(thumb->GetOrientation());
            bitmap->SetFilename(fileName);
            delete thumb;
            break;
        }
        default:
            break;
        }
    }
    catch (...) {}

    if (!bitmap || bitmap->GetWidth() == 0 || bitmap->GetHeight() == 0)
    {
        if (bitmap) delete bitmap;
        bitmap = ImageLoader::GetCancelPhoto(fileName, tw, th);
    }
    return bitmap;
}

std::vector<CImageVideoThumbnail*> VideoThumbnailService::LoadPlaceholders(
    const wxString& fileName, int count)
{
    std::vector<CImageVideoThumbnail*> list;
    int tw, th;
    GetScreenThumbnailSize(tw, th);

    const bool isAnim = FormatDetector::IsAnimation(fileName);
    int        rotation = 0;

#ifdef WIN32
    wxString loadingPng = CFileUtility::GetResourcesFolderPath() + "\\loading.png";
#else
    wxString loadingPng = CFileUtility::GetResourcesFolderPath() + "/loading.png";
#endif

    wxImage img, imgResized;
    img.LoadFile(loadingPng);
    imgResized = img.ResampleBilinear(tw, th);

    for (int i = 0; i < count; ++i)
    {
        const float pct =
            (static_cast<float>(i) / static_cast<float>(count)) * 100.f;
        auto* entry = new CImageVideoThumbnail();
        entry->rotation     = rotation;
        entry->percent      = pct;
        entry->image        = ImageConverter::WxToCvMat(imgResized);
        entry->filename     = fileName;
        entry->timePosition = isAnim ? i : 0;
        list.push_back(entry);
    }
    return list;
}

std::vector<CImageVideoThumbnail*> VideoThumbnailService::LoadWxMultiPage(
    const wxString& fileName, int bitmapType,
    int thumbW, int thumbH, bool /*compressJpeg*/, bool isThumbnail)
{
    std::vector<CImageVideoThumbnail*> list;
    wxImage        image;
    wxBitmapType   wxType   = {};
    CRegardsPDF*   pdf      = nullptr;
    int            nbFrames = 0;

    if (bitmapType == PDF)
    {
        bool err = false;
        nbFrames = CRegardsPDF::GetNbFrame(fileName, err);
    }
    else
    {
        wxType   = (bitmapType == TIFF) ? wxBITMAP_TYPE_TIFF : wxBITMAP_TYPE_ANI;
        nbFrames = wxImage::GetImageCount(fileName, wxType);
    }

    if (nbFrames == 0)
    {
        auto* e = new CImageVideoThumbnail();
        e->image        = ImageConverter::WxToCvMat(defaultPicture);
        e->filename     = fileName;
        e->rotation     = 0;
        e->delay        = 4;
        e->percent      = 100.f;
        e->timePosition = 0;
        list.push_back(e);
        return list;
    }

    if (bitmapType == PDF)
        pdf = new CRegardsPDF(fileName);

    for (int i = 0; i < nbFrames; ++i)
    {
        bool ok = true;
        image.Destroy();

        if (bitmapType == PDF)
        {
            image = pdf->GetPicture(i);
            ok    = pdf->IsOk();
        }
        else
            ok = image.LoadFile(fileName, wxType, i);

        if (!ok)
        {
            wxLogError(wxT("Can't load image number %d"), i);
            continue;
        }

        auto* e = new CImageVideoThumbnail();

        if (isThumbnail)
        {
            float scaleX = static_cast<float>(thumbW) / image.GetWidth();
            float scaleY = static_cast<float>(thumbH) / image.GetHeight();
            double zoom  = std::min(scaleX, scaleY);
            int    bw    = static_cast<int>(image.GetWidth()  * zoom);
            int    bh    = static_cast<int>(image.GetHeight() * zoom);

            cv::Mat in  = ImageConverter::WxToCvMat(image);
            cv::Mat out;
            cv::resize(in, out, cv::Size(bw, bh), 0, 0, cv::INTER_CUBIC);
            e->image = out;
        }
        else
            e->image = ImageConverter::WxToCvMat(image);

        e->filename     = fileName;
        e->rotation     = 0;
        e->delay        = 4;
        e->percent      = static_cast<int>(
            (static_cast<float>(i) / nbFrames) * 100.f);
        e->timePosition = i;
        list.push_back(e);
    }

    if (pdf) delete pdf;
    return list;
}

std::vector<CImageVideoThumbnail*> VideoThumbnailService::LoadAllFrames(
    const wxString& fileName, bool compressJpeg, bool isThumbnail)
{
    std::vector<CImageVideoThumbnail*> list;
    const int fmt = FormatDetector::DetectFormat(fileName);

    int tw, th;
    GetScreenThumbnailSize(tw, th);

    try
    {
        switch (fmt)
        {
        // ── HEIC / AVIF / WEBP animés ─────────────────────────────────────
#ifdef LIBHEIC
        case WEBP:
        case AVIF:
        case HEIC:
        {
            int              delay = 4;
            std::vector<cv::Mat> frames;

            if (fmt == HEIC || fmt == AVIF)
                frames = CHeic::GetAllPicture(
                    CConvertUtility::ConvertToUTF8(fileName), delay);
            else
                frames = CRegardsWebp::GetAllPicture(fileName, delay);

            for (size_t i = 0; i < frames.size(); ++i)
            {
                auto* e = new CImageVideoThumbnail();
                e->filename     = fileName;
                e->image        = frames[i];
                e->rotation     = 0;
                e->delay        = delay;
                e->percent      = static_cast<int>(
                    (static_cast<float>(i) / frames.size()) * 100.f);
                e->timePosition = static_cast<int>(i);
                list.push_back(e);
            }
            frames.clear();
            break;
        }
#endif

        // ── TIFF / PDF / ANI (multi-page via wxImage) ─────────────────────
        case TIFF:
        case PDF:
        case ANI:
            list = LoadWxMultiPage(fileName, fmt, tw, th, compressJpeg, isThumbnail);
            break;

        // ── GIF / PNG animés (via CxImage) ────────────────────────────────
        case PNG:
        case GIF:
        {
            wxFileName fichier(fileName);
            auto* cx = new CxImage();
            cx->SetRetreiveAllFrames(true);
            cx->Load(CConvertUtility::ConvertToUTF8(fileName),
                     CxImage::GetTypeIdFromName(fichier.GetExt()));

            if (cx->GetNumFrames() > 1)
            {
                for (int i = 0; i < cx->GetNumFrames(); ++i)
                {
                    auto* e = new CImageVideoThumbnail();
                    CxImage*            frame = cx->GetFrame(i);
                    CImageLoadingFormat img;
                    img.SetPicture(frame);

                    e->filename     = fileName;
                    e->image        = img.GetMatrix().getMat();
                    e->rotation     = 0;
                    e->delay        = cx->GetFrameDelay();
                    e->percent      = (static_cast<float>(i) /
                                       cx->GetNumFrames()) * 100.f;
                    e->timePosition = i;
                    list.push_back(e);
                }
            }
            else
            {
                auto* e = new CImageVideoThumbnail();
                CImageLoadingFormat* fallback =
                    ImageLoader::GetCancelPhoto(fileName, tw, th);
                e->filename     = fileName;
                e->image        = fallback->GetMatrix().getMat();
                e->rotation     = 0;
                e->delay        = cx->GetFrameDelay();
                e->percent      = 0.f;
                e->timePosition = 0;
                list.push_back(e);
                delete fallback;
            }
            delete cx;
            break;
        }

        // ── Vidéo (FFmpeg) ────────────────────────────────────────────────
        case MPG2: case MPEG: case AVCHD: case WINREC:
        case AVI:  case MP4:  case WEBM:  case MKV:
        case Y4M:  case WMV:  case AV1:   case MOV:
        {
            CVideoThumb* thumb = new CVideoThumb(fileName, true, false);
            thumb->GetVideoListFrame(list, tw, th);
            delete thumb;
            break;
        }

        // ── Fallback : image statique → un seul thumbnail ─────────────────
        default:
        {
            ThumbnailService svc;
            auto* e = new CImageVideoThumbnail();
            CImageLoadingFormat* img = svc.LoadThumbnail(fileName);
            e->image        = img->GetMatrix().getMat();
            e->filename     = fileName;
            e->rotation     = 0;
            e->delay        = 0;
            e->percent      = 0.f;
            e->timePosition = 0;
            list.push_back(e);
            delete img;
            break;
        }
        }
    }
    catch (...)
    {
        // Dernier recours : cv::imread direct et resize
        auto* e = new CImageVideoThumbnail();
        cv::Mat mat = cv::imread(CConvertUtility::ConvertToStdString(fileName),
                                 cv::IMREAD_COLOR | cv::IMREAD_IGNORE_ORIENTATION);
        if (!mat.empty())
            cv::resize(mat, e->image, cv::Size(tw, th), 0, 0, cv::INTER_LINEAR);
        e->filename     = fileName;
        e->rotation     = 0;
        e->delay        = 0;
        e->percent      = 0.f;
        e->timePosition = 0;
        list.push_back(e);
    }

    return list;
}
