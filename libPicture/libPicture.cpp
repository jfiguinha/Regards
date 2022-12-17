// ReSharper disable All
#include <header.h>
// DllPicture.cpp : définit les fonctions exportées pour l'application DLL.
//
#ifdef __APPLE__
#include <ReadImage.h>
#endif
#include "libPicture.h"
#include <wx/filename.h>
#include <wx/progdlg.h>
#include <wx/pdfdocument.h>
#define __FREEIMAGE__
#include <FreeImage.h>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
#include "MetadataExiv2.h"
#include "regards_webp.h"
#include "picture_utility.h"
#include <ximage.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include "ascii.h"
#ifdef ROTDETECT
#include <rotdetect.h>
#endif
#include <wx/wxpoppler.h>
#include <ImageVideoThumbnail.h>
#include <ImageLoadingFormat.h>
#include <ConvertUtility.h>
#include <picture_utility.h>
#include <picture_id.h>
#include <LibResource.h>
#include "pfm.h"
#ifdef LIBHEIC
#include <Heic.h>
#include <avif.h>
#endif
#if defined(WIN32)
#include "wic.h"
#endif



#ifdef LIBBPG
#if defined(WIN32)
#include <DllBpg.h>
#else
#include <dlfcn.h>
#endif
#endif
#include "PictureData.h"
//#include <SqlPhotos.h>

#ifdef TURBOJPEG
#include <turbojpeg.h>
#endif
#include <wx/dir.h>
#include <wx/mstream.h>
#if defined(LIBRAW)
#include "raw.h"
#endif
#include "jxl.h"
#include "ScaleThumbnail.h"

#if defined(FFMPEG)
#include <videothumb.h>
//using namespace Regards::Video;
#endif

#if defined(EXIV2)
using namespace Regards::exiv2;
#endif

#include <wx/image.h>

#include <FileUtility.h>
#include <VideoPlayer.h>
//Dialog Save
#include <GifOption.h>
#include <JpegOption.h>
#include <TiffOption.h>
#include <PngOption.h>
#include <webpOption.h>
#include <bpgoption.h>
#include <BmpOption.h>
#include <ExrOption.h>
#include <J2kOption.h>
#include <JxrOption.h>
#include <PDFOption.h>
#include <CompressionOption.h>
#include <opencv2/core/core.hpp>
#include "RegardsPDF.h"
#define TYPE_IMAGE_CXIMAGE 0
#define TYPE_IMAGE_WXIMAGE 1
#define TYPE_IMAGE_REGARDSIMAGE 2
#define OR ||

//using namespace Regards::Sqlite;
using namespace Regards::Picture;
using namespace Regards::Video;

using namespace OPENEXR_IMF_INTERNAL_NAMESPACE;
using namespace IMATH_INTERNAL_NAMESPACE;
extern float clamp(float val, float minval, float maxval);



#if defined(LIBBPG) && not defined(WIN32)

void * CLibPicture::lib_handle = nullptr;

void CLibPicture::LoadBpgDll()
{
     printf("LoadBpgDll\n");
     wxString path = CFileUtility::GetProgramFolderPath();

#ifdef __APPLE__
    path.Append("/../Frameworks/libRegardsBpg.dylib");
#else
    path.Append("/libRegardsBpg.so");
#endif

    //const char * fichier = CConvertUtility::ConvertToUTF8(path);
    lib_handle = dlopen(CConvertUtility::ConvertToUTF8(path), RTLD_LOCAL|RTLD_LAZY);
    if (!lib_handle) {
        printf("[%s] Unable to load library: %s\n", __FILE__, dlerror());
        //exit(EXIT_FAILURE);
    }
    else
    {
        printf("LoadBpgDll enable to load library\n");
    }
}

void CLibPicture::UnloadBpgDll()
{
    // Close the library.
    if (dlclose(lib_handle) != 0) {
        cerr << "[" << __FILE__ << "] main: Unable to close library: "
        << dlerror() << "\n";
    }
}

#endif

static FREE_IMAGE_FORMAT ImageFormat(const char* filename)
{
	//check the file signature and deduce its format
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	return fif;
}


CLibPicture::CLibPicture()
{
	svgWidth = 1024;
	svgHeight = 1024;
	configRegards = CParamInit::getInstance();
#ifdef WIN32
	wic = new CWic();
#endif
#ifdef __APPLE__
    readimage = new CReadMacOSImage();
#endif
}

CLibPicture::~CLibPicture()
{
#ifdef WIN32
	delete wic;
#endif
#ifdef __APPLE__
    delete readimage;
#endif
}


CImageLoadingFormat * CLibPicture::LoadPictureToBGRA(const wxString& filename, bool& pictureOK, const int& resizeWidth,
                                               const int& resizeHeight)
{
	CLibPicture libPicture;

	CImageLoadingFormat * imageLoading = libPicture.LoadPicture(filename);

	if (imageLoading != nullptr)
	{
		if (resizeWidth != 0 && resizeHeight != 0)
		{
			float ratio = CalculPictureRatio(imageLoading->GetWidth(), imageLoading->GetHeight(), resizeWidth,
			                                 resizeHeight);
			int width = imageLoading->GetWidth() * ratio;
			int height = imageLoading->GetHeight() * ratio;
			imageLoading->Resize(width, height, 1);
		}

		pictureOK = true;
	}
	else
		pictureOK = false;


	return imageLoading;
}

int CLibPicture::TestExtension(const wxString& ext)
{
	return CLibResource::GetExtensionId(ext);
}

void CLibPicture::Initx265Decoder()
{
	CHeic::Initx265Decoder();
}

void CLibPicture::Uninitx265Decoder()
{
	CHeic::Uninitx265Decoder();
}

bool CLibPicture::TestIsPicture(const wxString& szFileName)
{
	wxFileName fichier(szFileName);
	wxString extension = fichier.GetExt();

	if (ImageFormat(CConvertUtility::ConvertToUTF8(szFileName)) != FIF_UNKNOWN)
		return true;

	const int numExt = TestExtension(extension.Lower());
	if (numExt < 100 && numExt != ANI && numExt != 0)
		return true;
	return false;
}

wxImage * CLibPicture::ConvertRegardsBitmapToWXImagePt(cv::Mat& img)
{
	cv::Mat im2;
	if (img.channels() == 1) { cvtColor(img, im2, cv::COLOR_GRAY2RGB); }
	else if (img.channels() == 4) { cvtColor(img, im2, cv::COLOR_BGRA2RGB); }
	else { cvtColor(img, im2, cv::COLOR_BGR2RGB); }

	//cv::flip(im2, im2, 0);

	long imsize = im2.rows * im2.cols * im2.channels();
	wxImage * wx = new wxImage(im2.cols, im2.rows, (unsigned char*)malloc(imsize), false);
	unsigned char* s = im2.data;
	unsigned char* d = wx->GetData();
	memcpy(d, s, imsize);
	/*
	for (long i = 0; i < imsize; i++)
	{
		d[i] = s[i];
	}
	*/
	return wx;
}


wxImage CLibPicture::ConvertRegardsBitmapToWXImage(cv::Mat & img)
{
	cv::Mat im2;
	if (img.channels() == 1) { cvtColor(img, im2, cv::COLOR_GRAY2RGB); }
	else if (img.channels() == 4) { cvtColor(img, im2, cv::COLOR_BGRA2RGB); }
	else { cvtColor(img, im2, cv::COLOR_BGR2RGB); }

	//cv::flip(im2, im2, 0);

	long imsize = im2.rows * im2.cols * im2.channels();
	wxImage wx(im2.cols, im2.rows, (unsigned char*)malloc(imsize), false);
	unsigned char* s = im2.data;
	unsigned char* d = wx.GetData();
	memcpy(d, s, imsize);
	/*
	for (long i = 0; i < imsize; i++)
	{
		d[i] = s[i];
	}
	*/
	return wx;
}

bool CLibPicture::TestIsPDF(const wxString& szFileName)
{
	wxFileName fichier(szFileName);
	wxString extension = fichier.GetExt();

	const int numExt = TestExtension(extension.Lower());
	if (numExt == PDF)
		return true;
	return false;
}

bool CLibPicture::TestIsVideo(const wxString& szFileName)
{
	wxFileName fichier(szFileName);
	wxString extension = fichier.GetExt();

	const int numExt = TestExtension(extension.Lower());
	if (numExt > 100)
		return true;
	return false;
}

bool CLibPicture::TestIsAnimation(const wxString& szFileName)
{
	bool returnValue = false;
	int nbFrame = this->GetNbImage(szFileName);
	if (nbFrame > 1)
		returnValue = true;

	if(TestIsVideo(szFileName))
		returnValue = true;

	return returnValue;
}

//----------------------------------------------------------------------------
//Test si le format de l'image est exploitable
//----------------------------------------------------------------------------
int CLibPicture::TestImageFormat(const wxString& szFileName, const bool& reading )
{
	int numExt = 0;
	wxDir testDir(szFileName);

	if (!testDir.IsOpened())
	{
		//int numExt = 0;
		wxFileName fichier(szFileName);
		wxString extension = fichier.GetExt();
		if (extension.size() < 3)
			return 0;

		numExt = TestExtension(extension.Lower());
		if (numExt == ASCII && reading)
			numExt = 0;
	}
	return numExt;
}

int CLibPicture::SavePictureOption(const int& format, int& option, int& quality)
{
	int returnValue = 0;
	switch (format)
	{
	case BMP:
		{
			BmpOption bmpOption(nullptr);
			bmpOption.ShowModal();
			if (bmpOption.IsOk())
				quality = bmpOption.CompressionOption();

			returnValue = 1;
		}
		break;

	case WEBP:
		{
			WebpOption webpOption(nullptr);
			webpOption.ShowModal();
			if (webpOption.IsOk())
			{
				quality = webpOption.CompressionLevel();
				returnValue = 1;
			}
		}
		break;
#ifdef LIBBPG
	case BPG:
		{
			CBpgOption bpgOption(nullptr);
			bpgOption.ShowModal();
			if (bpgOption.IsOk())
			{
				quality = bpgOption.CompressionLevel();
				option = bpgOption.CompressionOption();
				returnValue = 1;
			}
		}
		break;
#endif

	case PDF:
		{
			CRegardsPDF::SavePictureOption( option, quality);
		}
		break;


	case TIFF:
		{
			TiffOption tiffOption(nullptr);
			tiffOption.ShowModal();
			if (tiffOption.IsOk())
			{
				option = tiffOption.CompressionOption();
				returnValue = 1;
			}
		}
		break;

	case PNG:
		{
			PngOption pngOption(nullptr);
			pngOption.ShowModal();
			if (pngOption.IsOk())
			{
				option = pngOption.CompressionOption();
				returnValue = 1;
			}
		}
		break;

	case GIF:
		{
			GifOption gifOption(nullptr);
			gifOption.ShowModal();
			if (gifOption.IsOk())
			{
				option = gifOption.CompressionOption();
				returnValue = 1;
			}
		}
		break;
		/*
	case JPEG:

		{
			JpegOption jpegOption(nullptr);
			jpegOption.ShowModal();
			if (jpegOption.IsOk())
			{
				option = jpegOption.CompressionOption();
				quality = jpegOption.CompressionLevel();
				returnValue = 1;
			}
		}
		break;
		*/
	case JPEG:
	case HEIC:
	case AVIF:
	case JPEG2000:
		{
			CompressionOption jpegOption(nullptr);
			jpegOption.ShowModal();
			if (jpegOption.IsOk())
			{
				option = 0;
				quality = jpegOption.CompressionLevel();
				returnValue = 1;
			}
		}
		break;

	case JP2:
	case J2K:
		{
			J2kOption j2kOption(nullptr);
			j2kOption.ShowModal();
			if (j2kOption.IsOk())
			{
				option = j2kOption.CompressionOption();
				quality = j2kOption.CompressionLevel();
				returnValue = 1;
			}
		}
		break;

	case EXR:
		{
			ExrOption exrOption(nullptr);
			exrOption.ShowModal();
			if (exrOption.IsOk())
			{
				option = exrOption.CompressionOption();
				returnValue = 1;
			}
		}
		break;

	case JXR:
		{
			JxrOption jxrOption(nullptr);
			jxrOption.ShowModal();
			if (jxrOption.IsOk())
			{
				option = jxrOption.CompressionOption();
				quality = jxrOption.CompressionLevel();
				returnValue = 1;
			}
		}
		break;

	case ASCII:
	case TGA:
	case PCX:
	case MNG:
	case PPM:
	case XPM:
	case IFF:
	case PFM:
	case JXL:
	case PNM:
	case JPC:
		{
			returnValue = 1;
		}
		break;
	default: ;
	}

	return returnValue;
}

void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char* message) {
	printf("\n*** ");
	if (fif != FIF_UNKNOWN) {
		if (FreeImage_GetFormatFromFIF(fif))
			printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf(message);
	printf(" ***\n");
}


int CLibPicture::SavePicture(const wxString& fileName, CImageLoadingFormat* bitmap, const int& option,
                             const int& quality)
{
	int iFormat = 0;

	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	iFormat = TestImageFormat(fileName, false);
	wxString informations_error = CLibResource::LoadStringFromResource(L"informationserror", 1);
	switch (iFormat)
	{
    case ASCII:
    {
        CBitmapToAscii::SaveToAscii(bitmap, CConvertUtility::ConvertToStdString(fileName));
        break;
    }

	case PFM:
		{
			cv::Mat matFloat = bitmap->GetFloatImage();
			CPfm::WriteFilePFM(matFloat, fileName, 1.0f);
			break;
		}

	case JXL:
	{
		cv::Mat matFloat = bitmap->GetFloatImage();
		CJxl::WriteFile(matFloat, fileName);
		break;
	}

	case BMP:
		{
			int _option = BMP_DEFAULT;
			if (option == 1)
				_option = BMP_SAVE_RLE;
			FIBITMAP* Image = bitmap->GetFreeImage();
			if (!FreeImage_Save(FIF_BMP, Image, fileName, _option))
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);
				//L"&Help";
				wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
				wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
			}
			FreeImage_Unload(Image);
		}
		break;


	case J2K:
		{
			int _option = quality;
			switch (option)
			{
			case 1:
				_option = J2K_DEFAULT;
				break;
			default:
				_option = 100 - quality;
				break;
			}
			FIBITMAP* Image = bitmap->GetFreeImage();
			if (!FreeImage_Save(FIF_J2K, Image, fileName, _option))
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);
				//L"&Help";
				wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
				wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
			}
			FreeImage_Unload(Image);
		}
		break;

	case JPEG2000:
	case JP2:
		{
			int _option = quality;
			switch (option)
			{
			case 1:
				_option = JP2_DEFAULT;
				break;
			default:
				_option = 100 - quality;
				break;
			}
			FIBITMAP* Image = bitmap->GetFreeImage();
			if (!FreeImage_Save(FIF_JP2, Image, fileName, _option))
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);
				//L"&Help";
				wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
				wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
			}
			FreeImage_Unload(Image);
		}
		break;

	case JXR:
		{
			int _option = quality;
			switch (option)
			{
			case 1:
				_option = JXR_DEFAULT;
				break;
			case 2:
				_option = JXR_LOSSLESS;
				break;
			case 3:
				_option = JXR_PROGRESSIVE;
				break;
			default:
				_option = 100 - quality;
				break;
			}
			FIBITMAP* Image = bitmap->GetFreeImage();
			if (!FreeImage_Save(FIF_JXR, Image, fileName, _option))
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);
				//L"&Help";
				wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
				wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
			}
			FreeImage_Unload(Image);
		}
		break;

	case EXR:
		{
			//regards->ConvertToBgr();
			int _option = EXR_DEFAULT;
			switch (option)
			{
			case 1:
				_option = EXR_FLOAT;
				break;
			case 2:
				_option = EXR_NONE;
				break;
			case 3:
				_option = EXR_ZIP;
				break;
			case 4:
				_option = EXR_PIZ;
				break;
			case 5:
				_option = EXR_PXR24;
				break;
			case 6:
				_option = EXR_B44;
				break;
			case 7:
				_option = EXR_LC;
				break;
			}

			FIBITMAP* Image = bitmap->GetFreeImage();
			FIBITMAP* floatImage = FreeImage_ConvertToRGBAF(Image);
			if (!FreeImage_Save(FIF_EXR, floatImage, fileName, _option))
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);
				//L"&Help";
				wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
				wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
			}
			FreeImage_Unload(floatImage);
			FreeImage_Unload(Image);
		}
		break;

	case WEBP:
		{
			cv::Mat mat = bitmap->GetOpenCVPicture();

			int _option = quality;
			switch (option)
			{
			case 1:
				_option = WEBP_DEFAULT;
				break;
			case 2:
				_option = WEBP_LOSSLESS;
				break;
			default:
				_option = 100 - quality;
				break;
			}

			//cv::flip(mat, mat, 0);
			CRegardsWebp::SavePicture(fileName, mat, _option);
			break;
		}

	case TGA:
		{
			FIBITMAP* Image = bitmap->GetFreeImage();
			if (!FreeImage_Save(FIF_TARGA, Image, fileName, 0))
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);
				//L"&Help";
				wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
				wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
			}
			FreeImage_Unload(Image);
		}
		break;


	case MNG:
		{
			FIBITMAP* Image = bitmap->GetFreeImage();
			if (!FreeImage_Save(FIF_MNG, Image, fileName, 0))
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);
				//L"&Help";
				wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
				wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
			}
			FreeImage_Unload(Image);
		}
		break;

	case TIFF:
		{
			int _option = TIFF_DEFAULT;
			switch (option)
			{
			case 1:
				_option = TIFF_DEFLATE;
				break;
			case 2:
				_option = TIFF_LZW;
				break;
			case 3:
				_option = TIFF_JPEG;
				break;
			case 4:
				_option = TIFF_NONE;
				break;
			}


			FIBITMAP* Image = bitmap->GetFreeImage();
			if (!FreeImage_Save(FIF_TIFF, Image, fileName, _option))
			{
				wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);
				//L"&Help";
				wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
				wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
			}
			FreeImage_Unload(Image);
		}
		break;

	case PNG:
		{
			CxImage image = bitmap->GetCxImage();
			image.SetCodecOption(option, CXIMAGE_FORMAT_PNG);
			image.Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("png"));
		}
		break;

	case GIF:
		{
			CxImage image = bitmap->GetCxImage();
			image.SetCodecOption(option, CXIMAGE_FORMAT_GIF);
			image.Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("gif"));

			wxString error = image.GetLastError();
			if (error != "")
				wxMessageBox(error,
				             informations_error, wxOK | wxICON_ERROR);
		}
		break;

#ifdef LIBBPG
	case BPG:
		{
			//int width = bitmap->GetWidth();
			//int height = bitmap->GetHeight();
			int lossless_mode = option;
			int compress_level = quality;

			CxImage image = bitmap->GetCxImage();
			//image->SetCodecOption(option, CXIMAGE_FORMAT_PNG);


			uint8_t* buffer = nullptr;
			int32_t sizeLen = 0;
			image.Encode(buffer, sizeLen, CxImage::GetTypeIdFromName("png"));
#if defined(WIN32)
			BPG_SavePNGPicture(buffer, sizeLen, compress_level, lossless_mode, 8,
			                   CConvertUtility::ConvertToUTF8(fileName));
#else
			int(*BPG_SavePNGPicture)(uint8_t * , uint64_t , int , int , int , const char * ) = (int(*)(uint8_t * , uint64_t , int , int , int , const char * ))dlsym(lib_handle, "BPG_SavePNGPicture");
			if (BPG_SavePNGPicture) {
				printf("[%s] dlsym(lib_handle, \"BPG_SavePNGPicture\"): Successful\n", __FILE__);
				int result = BPG_SavePNGPicture(buffer, sizeLen, compress_level, lossless_mode, 8, CConvertUtility::ConvertToUTF8(fileName));
			}
			else {
				printf("[%s] Unable to get symbol: %s\n",
					   __FILE__, dlerror());
				//exit(EXIT_FAILURE);
			}
#endif
			image.FreeMemory(buffer);
		}
		break;
#endif

	case HEIC:
		{
			wxLogNull logNo;
			uint8_t* data = nullptr;
			unsigned int size = 0;
			bool hasExif = false;
			cv::Mat image = bitmap->GetOpenCVPicture();
			CMetadataExiv2 pictureMetadata(bitmap->GetFilename());
			if (pictureMetadata.HasExif())
			{
				int typeImage = TestImageFormat(bitmap->GetFilename());
				if (typeImage == RAWFILE)
				{
					pictureMetadata.SetOrientation(0);
				}

				wxString fileTemp = CFileUtility::GetTempFile("temp_exif.jpg");
				
				cv::imwrite(CConvertUtility::ConvertToStdString(fileTemp), image);


				pictureMetadata.CopyMetadata(fileTemp);

				CMetadataExiv2 metadata(fileTemp);
				metadata.GetMetadataBuffer(data, size);
				if (size > 0)
				{
					data = new uint8_t[size + 1];
					metadata.GetMetadataBuffer(data, size);
					hasExif = true;
				}
			}

			//CRegardsBitmap* image = bitmap->GetRegardsBitmap(false);
			CHeic::SavePicture(CConvertUtility::ConvertToStdString(fileName), image, data, size, quality, hasExif);

			if (data != nullptr)
				delete[] data;

			break;
		}
	case AVIF:
		{
			wxLogNull logNo;
			uint8_t* data = nullptr;
			unsigned int size = 0;
			bool hasExif = false;
			cv::Mat image = bitmap->GetOpenCVPicture();
			CMetadataExiv2 pictureMetadata(bitmap->GetFilename());
			if (pictureMetadata.HasExif())
			{
				wxString fileTemp = CFileUtility::GetTempFile("temp_exif.jpg");
				cv::imwrite(CConvertUtility::ConvertToStdString(fileTemp), image);

				pictureMetadata.CopyMetadata(fileTemp);

				CMetadataExiv2 metadata(fileTemp);
				metadata.GetMetadataBuffer(data, size);
				if (size > 0)
				{
					data = new uint8_t[size + 1];
					metadata.GetMetadataBuffer(data, size);
					hasExif = true;
				}
			}


			CAvif::SavePicture(CConvertUtility::ConvertToStdString(fileName), image, data, size, quality, hasExif);
			if (data != nullptr)
				delete[] data;
			break;
		}

	case JPEG:
		{
			wxLogNull logNo;
            wxImage image = bitmap->GetwxImage();
            image.SetOption("wxIMAGE_OPTION_QUALITY",quality);
            image.SaveFile(fileName,wxBITMAP_TYPE_JPEG);
		}
		break;

	case PCX:
	{
		wxImage image = bitmap->GetwxImage();
		image.SaveFile(fileName, wxBITMAP_TYPE_PCX);
		break;
	}



	case PPM:
		{
			cv::Mat dest = bitmap->GetOpenCVPicture();
			cv::cvtColor(dest, dest, cv::COLOR_BGRA2BGR);
			cv::imwrite(CConvertUtility::ConvertToStdString(fileName), dest);

		}
		break;

	case PDF:
		{
			CRegardsPDF::SavePictureToPdf(fileName, bitmap, option, quality);
		}
		break;

	case XPM:
		{
			wxImage image = bitmap->GetwxImage();
			image.SaveFile(fileName, wxBITMAP_TYPE_XPM);

		}
		break;
	default: ;
	}


	wxString openfile_error = CLibResource::LoadStringFromResource(L"ImpossibleOpenFile", 1);
	if (TestIsExifCompatible(fileName))
	{
		if (wxFileName::FileExists(fileName) && (iFormat != AVIF) && (iFormat != HEIC))
		{
			CMetadataExiv2 pictureMetadata(bitmap->GetFilename());
			int typeImage = TestImageFormat(bitmap->GetFilename());
			//pictureMetadata.SetOrientation(0);
			pictureMetadata.CopyMetadata(fileName);
		}

		if (wxFileName::FileExists(fileName))
		{
			try
			{
				CMetadataExiv2 pictureMetadata(fileName);
				pictureMetadata.SetOrientation(0);
			}
			catch (...)
			{

			}

		}
	}

	if (!wxFileName::FileExists(fileName))
		wxMessageBox(openfile_error,
		             informations_error, wxOK | wxICON_ERROR);

	return 0;
}


bool CLibPicture::TestIsExifCompatible(const wxString& filename)
{
	const int iFormat = TestImageFormat(filename);
	if ((iFormat == JPEG || iFormat == JXL || iFormat == PNG || iFormat == TIFF || iFormat == WEBP || iFormat == RAWFILE || iFormat ==
		HEIC || iFormat == AVIF))
		return true;
	return false;
}

//-----------------------------------------------------------------------------
//Sauvegarde
//-----------------------------------------------------------------------------
int CLibPicture::SavePicture(const wxString& fileName, CImageLoadingFormat* bitmap)
{
	const int iFormat = TestImageFormat(fileName, false);
	const wxString wxfileName = fileName;

	int option = 0;
	int quality = 0;

	if (SavePictureOption(iFormat, option, quality) == 1)
	{
		SavePicture(wxfileName, bitmap, option, quality);
	}

	return 0;
}


//-----------------------------------------------------------------------------
//Sauvegarde
//-----------------------------------------------------------------------------
int CLibPicture::SavePicture(const wxString& fileNameIn, const wxString& fileNameOut)
{
	int option = 0;
	int quality = 0;
	int iFormat = TestImageFormat(fileNameOut, false);
	if (SavePictureOption(iFormat, option, quality) == 1)
	{
		int nbPicture = GetNbImage(fileNameIn);
		wxString msg = "in progress";
		wxProgressDialog dialog("Export File", "Checking...", nbPicture, nullptr,
		                        wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
		int updatesize = 0;
		dialog.Update(updatesize, msg);

		for (int i = 0; i < nbPicture; i++)
		{
			int j = i + 1;
			wxString filenameOutput = "";
			CImageLoadingFormat* picture = LoadPicture(fileNameIn, false, i);
			wxString message = "In progress : " + to_string(j) + "/" + to_string(nbPicture);
			if (false == dialog.Update(i, message))
			{
				break;
			}
			if (nbPicture > 1)
			{
				wxFileName filename(fileNameOut);
				filenameOutput = filename.GetPathWithSep() + filename.GetName() + "_page_" + to_string(j) + "." +
					filename.GetExt();
			}
			else
			{
				filenameOutput = fileNameOut;
			}
			SavePicture(filenameOutput, picture, option, quality);
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
//Obtention d'un thumbnail à partir des informations exif
//--------------------------------------------------------------------------------------------
CImageLoadingFormat* CLibPicture::LoadVideoThumbnail(const wxString& szFileName, const int& percent, int& timePosition)
{
	int iFormat = TestImageFormat(szFileName);
	CImageLoadingFormat* bitmap = nullptr;


#ifdef WIN32

	HDC screen = GetDC(nullptr);
	RECT rcClip;
	GetClipBox(screen, &rcClip);
	ReleaseDC(nullptr, screen);

	int widthThumbnail = max(static_cast<int32_t>(rcClip.right / 4), 200);
	int heightThumbnail = max(static_cast<int32_t>(rcClip.bottom / 4), 200);

#else
	int widthThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, 200);
	int heightThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 4, 200);
#endif

	try
	{
		switch (iFormat)
		{
		case MPG2:
		case MPEG:
		case AVCHD:
		case WINREC:
		case AVI:
		case MP4:
		case WMV:
		case WEBM:
		case MKV:
		case Y4M:
		case MOV:
		case AV1:
			{
				//RGBQUAD color = {0,0,0,0};
				bitmap = new CImageLoadingFormat();
				bitmap->SetFilename(szFileName);
				CThumbnailVideo video(szFileName, false);
				cv::Mat mat = video.GetVideoFramePercent(percent, widthThumbnail, heightThumbnail);
				bitmap->SetPicture(mat);
				bitmap->SetOrientation(video.GetOrientation());
				bitmap->SetFilename(szFileName);
				break;
			}
		default: ;
		}
	}
	catch (...)
	{
	}


	if (bitmap == nullptr || bitmap->GetWidth() == 0 || bitmap->GetHeight() == 0)
	{
		bitmap = GetCancelPhoto(szFileName, widthThumbnail, heightThumbnail);
	}

	return bitmap;
}

vector<CImageVideoThumbnail*> CLibPicture::LoadDefaultVideoThumbnail(const wxString& szFileName, const int& size)
{
#ifdef WIN32

	HDC screen = GetDC(nullptr);
	RECT rcClip;
	GetClipBox(screen, &rcClip);
	ReleaseDC(nullptr, screen);

	int widthThumbnail = max(static_cast<int32_t>(rcClip.right / 4), 200);
	int heightThumbnail = max(static_cast<int32_t>(rcClip.bottom / 4), 200);

#else
	int widthThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, 200);
	int heightThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 4, 200);
#endif

	vector<CImageVideoThumbnail*> listThumbnail;
	int rotation = 0;


	const bool isAnimation = TestIsAnimation(szFileName);



	for (auto i = 0; i < size; i++)
	{
		const int pourcentage = 0;
		const float percent = (static_cast<float>(i) / static_cast<float>(size)) * 100.0f;
		auto cxVideo = new CImageVideoThumbnail();

		cxVideo->rotation = rotation;
		cxVideo->percent = percent;

#ifdef WIN32
		wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "\\loading.png";
#else
		wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "/loading.png";
#endif

		//picture->Resize(widthThumbnail, heightThumbnail, 0);
		//picture->SetFilename(CConvertUtility::ConvertToStdString(szFileName));
		cxVideo->image.LoadFile(photoCancel);
		cxVideo->image = cxVideo->image.ResampleBilinear(widthThumbnail, heightThumbnail);
		cxVideo->filename = szFileName;
		//cxVideo->image->SetPicturToJpeg(picture->GetRegardsBitmap(), false);
		if (isAnimation)
			cxVideo->timePosition = i;
		else
			cxVideo->timePosition = pourcentage * percent;
		cxVideo->filename = szFileName;
		cxVideo->rotation = rotation;
		//delete picture;
		listThumbnail.push_back(cxVideo);
	}

	return listThumbnail;
}


void CLibPicture::LoadwxImageThumbnail(const wxString& szFileName, vector<CImageVideoThumbnail*>* listThumbnail,
                                       const int& bitmapType, const int& width, const int& height,
                                       const bool& compressJpeg, const bool& isThumbnail)
{
	
	wxImage image;
	wxBitmapType bitmapTypeWxWidget = {};
	CRegardsPDF* regardsPdf = nullptr;
	//int iFormat = TestImageFormat(szFileName);
	//bool needresize = true;
	int m_ani_images = 0;
	if (bitmapType == PDF)
	{
		bool error = false;
		m_ani_images = CRegardsPDF::GetNbFrame(szFileName, error);
	}
	else
	{
		if (bitmapType == TIFF)
			bitmapTypeWxWidget = wxBITMAP_TYPE_TIFF;
		else
			bitmapTypeWxWidget = wxBITMAP_TYPE_ANI;

		m_ani_images = wxImage::GetImageCount(szFileName, bitmapTypeWxWidget);
	}
	if (m_ani_images == 0)
	{
		auto imageVideoThumbnail = new CImageVideoThumbnail();
		imageVideoThumbnail->image.LoadFile(CPictureUtility::GetPhotoCancel());
		imageVideoThumbnail->filename = szFileName;
		imageVideoThumbnail->rotation = 0;
		imageVideoThumbnail->delay = 4;
		imageVideoThumbnail->percent = 100.0f;
		imageVideoThumbnail->timePosition = 0;
		listThumbnail->push_back(imageVideoThumbnail);
	}
	else
	{
		if (bitmapType == PDF)
			regardsPdf = new CRegardsPDF(szFileName);

		for (auto i = 0; i < m_ani_images; i++)
		{
			bool return_value = true;
			image.Destroy();
			if (bitmapType == PDF)
			{
				image = regardsPdf->GetPicture(i);
				return_value = regardsPdf->IsOk();
			}
			else
			{
				return_value = image.LoadFile(szFileName, bitmapTypeWxWidget, i);
			}


			if (!return_value)
			{
				wxString tmp = wxT("Can't load image number ");
				tmp << i;
				wxLogError(tmp);
			}
			else
			{
				auto imageVideoThumbnail = new CImageVideoThumbnail();
				cv::Mat out;
				wxImage bitmapResize;
				if (isThumbnail)
				{
					int bw, bh;

					float scale_x = static_cast<float>(width) / static_cast<float>(image.GetWidth());
					float scale_y = static_cast<float>(height) / static_cast<float>(image.GetHeight());

					double m_zoomFactor = min(scale_x, scale_y);
					bw = static_cast<int>(image.GetWidth() * m_zoomFactor);
					bh = static_cast<int>(image.GetHeight() * m_zoomFactor);


					cv::Mat in = CLibPicture::mat_from_wx(image);
					cv::resize(in, out, cv::Size(bw, bh), cv::INTER_CUBIC);
					imageVideoThumbnail->image = CLibPicture::ConvertRegardsBitmapToWXImage(out);
				}
				else
					imageVideoThumbnail->image = image;


				imageVideoThumbnail->filename = szFileName;
				imageVideoThumbnail->rotation = 0;
				imageVideoThumbnail->delay = 4;
				imageVideoThumbnail->percent = static_cast<int>(static_cast<float>(i) / static_cast<float>(
					m_ani_images)) * 100.0f;
				imageVideoThumbnail->timePosition = i;
				listThumbnail->push_back(imageVideoThumbnail);
			}
		}
	}
	if (regardsPdf != nullptr)
		delete regardsPdf;
}

int CLibPicture::GetNbImage(const wxString& szFileName)
{
	int iFormat = TestImageFormat(szFileName);
#ifdef WIN32
	/*
	bool error = false;
	int nbFrame = wic->GetNbFrame(szFileName.ToStdString(), error);
	if (!error)
		return nbFrame;
	*/
#endif

	switch (iFormat)
	{
#ifdef LIBHEIC
	case AVIF:
		{
			return CAvif::GetNbFrame(CConvertUtility::ConvertToStdString(szFileName));
		}
		break;
	case HEIC:
		{
			return CHeic::GetNbFrame(CConvertUtility::ConvertToStdString(szFileName));
		}
		break;
#endif

	case PDF:
		{
			bool isValid = false;
			return CRegardsPDF::GetNbFrame(szFileName, isValid);
		}


	case TIFF:
	case ANI:
		{
			wxBitmapType bitmapType = wxBITMAP_TYPE_ANY;
			return wxImage::GetImageCount(szFileName, bitmapType);
			break;
		}

	case WEBP:
		{
			return CRegardsWebp::GetNbFrame(szFileName);
		}

	case PNG:
	case GIF:
		{

			wxFileName fichier(szFileName);
			wxString extension = fichier.GetExt();

			auto _cxImage = new CxImage();
			_cxImage->SetRetreiveAllFrames(true);
			_cxImage->Load(CConvertUtility::ConvertToUTF8(szFileName), CxImage::GetTypeIdFromName(extension));
			int nbFrame = _cxImage->GetNumFrames();
			delete _cxImage;
			return nbFrame;
			break;
		}

	case MPG2:
	case MPEG:
	case AVCHD:
	case WINREC:
	case AVI:
	case MP4:
	case WEBM:
	case MKV:
	case Y4M:
	case WMV:
	case AV1:
	case MOV:
		{
			CThumbnailVideo thumbnail(szFileName, false);
			int64_t duration = thumbnail.GetMovieDuration();
			if(duration > 20)
				return 20;
			return duration;

			break;
		}
	default: ;
	}

	return 1;
}

uint32_t CLibPicture::GetFrameDelay(const wxString& szFileName)
{
	uint32_t delay = 10;
	int iFormat = TestImageFormat(szFileName);
	switch (iFormat)
	{
#ifdef LIBHEIC
	case AVIF:
		return CAvif::GetDelay(CConvertUtility::ConvertToStdString(szFileName));

	case HEIC:
		return CHeic::GetDelay(CConvertUtility::ConvertToStdString(szFileName));

#endif

	case GIF:
		{
			auto _cxImage = new CxImage();
			_cxImage->SetRetreiveAllFrames(true);
			_cxImage->Load(CConvertUtility::ConvertToUTF8(szFileName), CxImage::GetTypeIdFromName("gif"));
			delay = _cxImage->GetFrameDelay();
			delete _cxImage;
		}
		break;
	default: ;
	}
	return delay;
}

CImageLoadingFormat* CLibPicture::GetCancelPhoto(const wxString& szFileName, const int& widthThumbnail,
                                                 const int& heightThumbnail)
{
#ifdef WIN32
	wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
	wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif

	CLibPicture libPicture;
	auto bitmap = libPicture.LoadPicture(photoCancel);
	if (widthThumbnail > 0 && heightThumbnail > 0)
		bitmap->Resize(widthThumbnail, heightThumbnail, 0);
	bitmap->SetFilename(szFileName);

	return bitmap;
}

void CLibPicture::LoadAllVideoThumbnail(const wxString& szFileName, vector<CImageVideoThumbnail*>* listThumbnail,
                                        const bool& compressJpeg, const bool& isThumbnail)
{
	int iFormat = TestImageFormat(szFileName);
#ifdef WIN32

	HDC screen = GetDC(nullptr);
	RECT rcClip;
	GetClipBox(screen, &rcClip);
	ReleaseDC(nullptr, screen);

	int widthThumbnail = max(static_cast<int32_t>(rcClip.right / 4), 200);
	int heightThumbnail = max(static_cast<int32_t>(rcClip.bottom / 4), 200);

#else
	int widthThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, 200);
	int heightThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 4, 200);
#endif


	//  int rotation = 0;
	try
	{
		switch (iFormat)
		{
#ifdef LIBHEIC
		case WEBP:
		case AVIF:
		case HEIC:
			{
				int delay = 4;
				bool isMaster;
				vector<cv::Mat> listPicture;
				if (iFormat == HEIC)
					listPicture = CHeic::GetAllPicture(CConvertUtility::ConvertToStdString(szFileName), isMaster, delay);
				else if (iFormat == AVIF)
					listPicture = CAvif::GetAllPicture(CConvertUtility::ConvertToStdString(szFileName), delay);
				else if(iFormat == WEBP)
					listPicture = CRegardsWebp::GetAllPicture(szFileName, delay);
				for (auto i = 0; i < listPicture.size(); i++)
				{
					auto imageVideoThumbnail = new CImageVideoThumbnail();
					imageVideoThumbnail->filename = szFileName;
					imageVideoThumbnail->image = ConvertRegardsBitmapToWXImage(listPicture.at(i));
					imageVideoThumbnail->rotation = 0;
					imageVideoThumbnail->delay = delay;
					imageVideoThumbnail->percent = static_cast<int>((static_cast<float>(i) / static_cast<float>(
						listPicture.size())) * 100.0f);
					imageVideoThumbnail->timePosition = i;
					listThumbnail->push_back(imageVideoThumbnail);
				}
				listPicture.clear();
			}
			break;
#endif

		case TIFF:
		case PDF:
		case ANI:
			LoadwxImageThumbnail(szFileName, listThumbnail, iFormat, widthThumbnail, heightThumbnail, compressJpeg,
			                     isThumbnail);
			break;


		case PNG:
		case GIF:
			{
				wxFileName fichier(szFileName);
				wxString extension = fichier.GetExt();
				auto _cxImage = new CxImage();
				_cxImage->SetRetreiveAllFrames(true);
				_cxImage->Load(CConvertUtility::ConvertToUTF8(szFileName),
				               CxImage::GetTypeIdFromName(extension));
				if (_cxImage->GetNumFrames() > 1)
				{
					for (auto i = 0; i < _cxImage->GetNumFrames(); i++)
					{
						auto imageVideoThumbnail = new CImageVideoThumbnail();
						CxImage* frame = _cxImage->GetFrame(i);
						CImageLoadingFormat image;
						image.SetPicture(frame);
						imageVideoThumbnail->filename = szFileName;
						imageVideoThumbnail->image = image.GetwxImage();
						imageVideoThumbnail->rotation = 0;
						imageVideoThumbnail->delay = _cxImage->GetFrameDelay();
						imageVideoThumbnail->percent = (static_cast<float>(i) / static_cast<float>(_cxImage->
							GetNumFrames())) * 100.0f;
						imageVideoThumbnail->timePosition = i;
						listThumbnail->push_back(imageVideoThumbnail);

					}
				}
				else
				{
					auto imageVideoThumbnail = new CImageVideoThumbnail();
					CImageLoadingFormat * image = GetCancelPhoto(szFileName, widthThumbnail, heightThumbnail);
					imageVideoThumbnail->filename = szFileName;
					imageVideoThumbnail->image = image->GetwxImage();
					imageVideoThumbnail->rotation = 0;
					imageVideoThumbnail->delay = _cxImage->GetFrameDelay();
					imageVideoThumbnail->percent = (static_cast<float>(0) / static_cast<float>(_cxImage->
						GetNumFrames())) * 100.0f;
					imageVideoThumbnail->timePosition = 0;
					listThumbnail->push_back(imageVideoThumbnail);
					delete image;
				}
				delete _cxImage;
			}
			break;

		case MPG2:
		case MPEG:
		case AVCHD:
		case WINREC:
		case AVI:
		case MP4:
		case WEBM:
		case MKV:
		case Y4M:
		case WMV:
		case AV1:
		case MOV:
			{

				CThumbnailVideo video(szFileName, true);
				vector<CImageVideoThumbnail*> listVideo = video.GetVideoListFrame(widthThumbnail, heightThumbnail);
				for (CImageVideoThumbnail* cxVideo : listVideo)
				{
					listThumbnail->push_back(cxVideo);
				}
				break;
			}

		default:
			{
				auto imageVideoThumbnail = new CImageVideoThumbnail();
				CImageLoadingFormat* imageLoad = LoadThumbnail(szFileName);
				imageVideoThumbnail->image = imageLoad->GetwxImage();
				imageVideoThumbnail->filename = szFileName;
				imageVideoThumbnail->rotation = 0;
				imageVideoThumbnail->delay = 0;
				imageVideoThumbnail->percent = 0;
				imageVideoThumbnail->timePosition = 0;
				listThumbnail->push_back(imageVideoThumbnail);
				delete imageLoad;
			}
		}
	}
	catch (...)
	{
		auto imageVideoThumbnail = new CImageVideoThumbnail();
		imageVideoThumbnail->image.LoadFile(szFileName);
		imageVideoThumbnail->image = imageVideoThumbnail->image.ResampleBicubic(widthThumbnail, heightThumbnail);
		imageVideoThumbnail->filename = szFileName;
		imageVideoThumbnail->rotation = 0;
		imageVideoThumbnail->delay = 0;
		imageVideoThumbnail->percent = 0;
		imageVideoThumbnail->timePosition = 0;
		listThumbnail->push_back(imageVideoThumbnail);
	}
}

bool CLibPicture::TestIsVideoValid(const wxString& szFileName)
{
	bool is_valid;

	if (fileValid.find(szFileName) != fileValid.end())
	{
		is_valid = fileValid[szFileName];
	}
	else
	{
		//CThumbnailVideo video;
		//video.SetFilename(szFileName);
		//is_valid = video.IsOk();
		CVideoPlayer capture(szFileName, false);
		if (capture.isOpened())
		{
			fileValid.insert(std::make_pair(szFileName, is_valid));
			is_valid = true;
		}
	}
	return is_valid;
}

//--------------------------------------------------------------------------------------------
//Obtention d'un thumbnail à partir des informations exif
//--------------------------------------------------------------------------------------------
CImageLoadingFormat* CLibPicture::LoadThumbnail(const wxString& fileName, const bool& fromExifOnly)
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	CImageLoadingFormat* imageLoading = nullptr;
	bool notThumbnail = false;
#ifdef WIN32

	
	cv::Mat bmp = wic->GetThumbnailMetadata(fileName.ToStdString());
	if (!bmp.empty())
	{
		CMetadataExiv2 pictureMetadata(fileName);
		imageLoading = new CImageLoadingFormat();
		imageLoading->SetPicture(bmp);
		imageLoading->SetFilename(fileName);
		imageLoading->SetOrientation(pictureMetadata.GetOrientation());
		return imageLoading;
	}
	HDC screen = GetDC(nullptr);
	RECT rcClip;
	GetClipBox(screen, &rcClip);
	ReleaseDC(nullptr, screen);

	int widthThumbnail = max(static_cast<int32_t>(rcClip.right / 4), 200);
	int heightThumbnail = max(static_cast<int32_t>(rcClip.bottom / 4), 200);

#else
	int widthThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, 200);
	int heightThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 4, 200);
#endif


#if defined(EXIV2)

	//bool exif = false;
	//bool iptc = false;
	//bool xmp = false;
	//int returnValue = -1;
	int iFormat = TestImageFormat(fileName);

#ifdef LIBRAW
	if (iFormat == RAWFILE)
	{
		bool isFromExif = false;
		CMetadataExiv2 pictureMetadata(fileName);
		imageLoading = CRaw::GetThumbnail(fileName, true, isFromExif);
		if (imageLoading != nullptr && imageLoading->IsOk())
		{
			imageLoading->Resize(widthThumbnail, heightThumbnail, 0);
		}
		else
			notThumbnail = true;

	}
	else if (TestIsVideo(fileName))
	{
		imageLoading = LoadPicture(fileName, true);
		if (imageLoading != nullptr && imageLoading->IsOk())
			imageLoading->Resize(widthThumbnail, heightThumbnail, 0);
	}
	else if (iFormat == HEIC)
	{
		int angle = 0;
		cv::Mat bitmap = CHeic::GetThumbnailPicture(CConvertUtility::ConvertToStdString(fileName), angle);
		if (!bitmap.empty())
		{
			imageLoading = new CImageLoadingFormat();
			imageLoading->SetFilename(fileName);
			imageLoading->SetPicture(bitmap);
			imageLoading->SetRotation(angle);
			if (imageLoading != nullptr && imageLoading->IsOk())
			{
				imageLoading->Resize(widthThumbnail, heightThumbnail, 1);
			}
		}
		else
		{
			notThumbnail = true;
		}
	}
	else
	{
#endif

		printf("CLibPicture::LoadThumbnail \n");
		int orientation = -1;
		wxString extension;
		wxMemoryInputStream * memFile = nullptr;
		CMetadataExiv2 pictureMetadata(fileName);
		if (pictureMetadata.HasThumbnail())
		{
			memFile = pictureMetadata.DecodeThumbnail(extension, orientation);
		}
		if (memFile == nullptr && !fromExifOnly)
		{
			imageLoading = new CImageLoadingFormat();
			imageLoading->SetFilename(fileName);
			imageLoading = LoadPicture(fileName, true);
			if (imageLoading != nullptr && imageLoading->IsOk())
			{
				imageLoading->Resize(widthThumbnail, heightThumbnail, 1);
				//imageLoading->ApplyExifOrientation();
			}

		}
		else if (memFile != nullptr)
		{
            printf("File to process : %s \n", CConvertUtility::ConvertToUTF8(fileName));
			//auto image = new CxImage(memFile, CxImage::GetTypeIdFromName(CConvertUtility::ConvertToUTF8(extension)));
			wxImage jpegImage;
			jpegImage.LoadFile(*memFile, wxBITMAP_TYPE_JPEG);
			if (jpegImage.GetWidth() > 0 && jpegImage.GetHeight() > 0)
			{
				imageLoading = new CImageLoadingFormat();
				imageLoading->SetFilename(fileName);
				imageLoading->SetPicture(jpegImage);
				imageLoading->SetOrientation(orientation);
				if (imageLoading->IsOk())
				{
					imageLoading->Resize(widthThumbnail, heightThumbnail, 1);
				}
			}
			else
			{
				notThumbnail = true;
			}
			delete memFile;
		}


#ifdef LIBRAW
	}
#endif

#endif

	if (notThumbnail)
	{
		imageLoading = LoadPicture(fileName, true);
		if (imageLoading != nullptr && imageLoading->IsOk())
		{
			imageLoading->Resize(widthThumbnail, heightThumbnail, 1);
		}
	}


	if (imageLoading == nullptr || !imageLoading->IsOk())
	{
		if (imageLoading != nullptr)
			delete imageLoading;

		imageLoading = GetCancelPhoto(fileName, widthThumbnail, heightThumbnail);
	}

	return imageLoading;
}

void CLibPicture::DefineSVGSize(const int& width, const int& height)
{
	svgWidth = width;
	svgHeight = height;
}


float CLibPicture::CalculPictureRatio(const int& pictureWidth, const int& pictureHeight, const int& widthThumbnail,
                                      const int& heightThumbnail)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float new_ratio;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		new_ratio = static_cast<float>(widthThumbnail) / static_cast<float>(pictureWidth);
	else
		new_ratio = static_cast<float>(heightThumbnail) / static_cast<float>(pictureHeight);

	if ((pictureHeight * new_ratio) > heightThumbnail)
	{
		new_ratio = static_cast<float>(heightThumbnail) / static_cast<float>(pictureHeight);
	}
	else
	{
		if ((pictureWidth * new_ratio) > widthThumbnail)
		{
			new_ratio = static_cast<float>(widthThumbnail) / static_cast<float>(pictureWidth);
		}
	}

	return new_ratio;
}

bool CLibPicture::PictureDimensionFreeImage(const char* filename, int& width, int& height)
{
#ifdef __FREEIMAGE__
	//pointer to the image, once loaded
	FIBITMAP* dib(nullptr);

	//check the file signature and deduce its format
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return false;

	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);


	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);
#endif
	return true;
}

cv::Mat CLibPicture::LoadFromFreeImage(const char* filename)
{
	cv::Mat bitmapMatrix;

#ifdef __FREEIMAGE__
	//pointer to the image, once loaded
	FIBITMAP* dib(nullptr);
	FIBITMAP* dibRgba(nullptr);
	//pointer to the image data
	BYTE* bits(nullptr);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return bitmapMatrix;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return bitmapMatrix;

	dibRgba = FreeImage_ConvertTo32Bits(dib);

	//get the image width and height
	width = FreeImage_GetWidth(dibRgba);
	height = FreeImage_GetHeight(dibRgba);
	bitmapMatrix = cv::Mat(height, width, CV_8UC4);
	memcpy(bitmapMatrix.data, FreeImage_GetBits(dibRgba), width * height * 4);
	cv::flip(bitmapMatrix, bitmapMatrix, 0);
	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);
	FreeImage_Unload(dibRgba);
	//return success
#endif
	return bitmapMatrix;
}

//------------------------------------------------------------------------------
//Chargement d'une image par son nom
//------------------------------------------------------------------------------
CImageLoadingFormat* CLibPicture::LoadPicture(const wxString& fileName, const bool& isThumbnail, const int& numPicture)
{
	CImageLoadingFormat * bitmap = new CImageLoadingFormat();
#ifdef WIN32
	cv::Mat _bitmap;
	
	if(isThumbnail)
		_bitmap = wic->GetThumbnailMetadata(CConvertUtility::ConvertToStdString(fileName));
	else if (numPicture == 0)
	{
		bool error = true;
		int nbFrame = wic->GetNbFrame(CConvertUtility::ConvertToStdString(fileName), error);
		if(nbFrame == GetNbImage(fileName))
			_bitmap = wic->GetPicture(CConvertUtility::ConvertToStdString(fileName), numPicture);
	}
	
	if (_bitmap.empty())
		LoadPicture(fileName, isThumbnail, numPicture, bitmap);
	else
    {
		bitmap->SetPicture(_bitmap);
        ApplyOrientation(fileName, true, bitmap);
    }
/*
#elif defined(__APPLE__)
    int width = 0;
    int height = 0;
    unsigned char * data = readimage->ReadImage(fileName,width,height);
    if(width != 0 && data != nullptr && height != 0 && !isThumbnail)
    {
        cv::Mat mat(height, width, CV_8UC4, data);
        CRegardsBitmap * _bitmap = new CRegardsBitmap();
        _bitmap->SetMatrix(mat);
        _bitmap->VertFlipBuf();
        _bitmap->ConvertToBgr();
        bitmap->SetPicture(_bitmap);
        ApplyOrientation(fileName, true, bitmap);
    }
    else
        LoadPicture(fileName, isThumbnail, numPicture, bitmap);
*/
#else
	LoadPicture(fileName, isThumbnail, numPicture, bitmap);
#endif
	return bitmap;
}

cv::Mat CLibPicture::mat_from_wx(const wxImage& wx)
{
	cv::Mat im2(cv::Size(wx.GetWidth(), wx.GetHeight()), CV_8UC3, wx.GetData());
	if (wx.HasAlpha())
	{
		std::vector<cv::Mat> matChannels;
		cv::split(im2, matChannels);

		// create alpha channel
		cv::Mat alpha(cv::Size(wx.GetWidth(), wx.GetHeight()), CV_8UC1, wx.GetAlpha());
		matChannels.push_back(alpha);

		cv::merge(matChannels, im2);

		cvtColor(im2, im2, cv::COLOR_RGBA2BGRA);
	}
	else
		cvtColor(im2, im2, cv::COLOR_RGB2BGRA);

	return im2;
}

//------------------------------------------------------------------------------
//Chargement d'une image par son nom
//------------------------------------------------------------------------------
void CLibPicture::LoadPicture(const wxString& fileName, const bool& isThumbnail, const int& numPicture, CImageLoadingFormat * bitmap)
{
#if defined(WIN32) && defined(_DEBUG)
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"LoadPicture\n");
#endif

	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	//printf("CLibPicture LoadPicture \n");

	int iFormat = TestImageFormat(fileName);
	bitmap->SetFilename(fileName);
	bool applyExif = true;
	//bool value = true;
	//int rotation = 0;
	try
	{
		//int typeImage = TYPE_IMAGE_CXIMAGE;
		//wxImage imageWx;
		//CxImage * image = nullptr;
		switch (iFormat)
		{
#ifdef LIBHEIC

		case HEIC:
		{
			cv::Mat picture;
			int orientation = 0;
			if (numPicture == 0)
			{
				if (isThumbnail)
					picture = CHeic::GetThumbnailPicture(CConvertUtility::ConvertToStdString(fileName), orientation);

				if (picture.empty())
					picture = CHeic::GetPicture(CConvertUtility::ConvertToStdString(fileName), orientation);
			}
			else
			{
				int delay = 4;
				bool isMaster;
				picture = CHeic::GetPicture(CConvertUtility::ConvertToStdString(fileName), isMaster, delay,
					numPicture);
			}

			if (!picture.empty())
			{
				bitmap->SetPicture(picture);
				bitmap->SetFilename(fileName);
				bitmap->RotateExif(orientation);
			}
			break;
		}
		case AVIF:
		{
			cv::Mat picture;

			if (numPicture == 0)
			{
				picture = CAvif::GetPicture(CConvertUtility::ConvertToStdString(fileName));
			}
			else
			{
				int delay = 4;
				picture = CAvif::GetPicture(CConvertUtility::ConvertToStdString(fileName), delay, numPicture);
			}

			if (!picture.empty())
			{
				bitmap->SetPicture(picture);
				bitmap->SetFilename(fileName);
			}
		}
		break;
#endif

		case JXL:
		{
			cv::Mat out;
			cv::Mat mat = CJxl::GetPicture(fileName);
			mat.convertTo(out, CV_8UC4, 255); // or CV_32F works (too)
			bitmap->SetPicture(out);
			bitmap->ConvertToBGR();
			//bitmap->Flip();
			break;
		}

		case PFM:
		{
			cv::Mat out;
			cv::Mat mat = CPfm::ReadFilePFM(fileName, isThumbnail);
			mat.convertTo(out, CV_8UC4, 255); // or CV_32F works (too)
			bitmap->SetPicture(out);
			bitmap->Flip();
			break;
		}

		case EXR:
		{
				Array2D<Rgba> pixels;
				RgbaInputFile file(CConvertUtility::ConvertToUTF8(fileName));
				Box2i dw = file.dataWindow();
				int width = dw.max.x - dw.min.x + 1;
				int height = dw.max.y - dw.min.y + 1;
				pixels.resizeErase(height, width);
				file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
				file.readPixels(dw.min.y, dw.max.y);

				cv::Mat picture;

				if (width > 0 && height > 0)
				{
					picture = cv::Mat(height, width, CV_8UC4);
					int k = 0;
					uint8_t * data = picture.data;
					for (int i = 0; i < height; i++)
					{
						for (int j = 0; j < width; j++, k += 4)
						{
							float rvalue = clamp(float(pixels[i][j].r), 0.0f, 1.0f);
							float gvalue = clamp(float(pixels[i][j].g), 0.0f, 1.0f);
							float bvalue = clamp(float(pixels[i][j].b), 0.0f, 1.0f);
							float avalue = clamp(float(pixels[i][j].a), 0.0f, 1.0f);

							data[k] = (int)(bvalue * 255.0);
							data[k + 1] = (int)(gvalue * 255.0);
							data[k + 2] = (int)(rvalue * 255.0);
							data[k + 3] = (int)(avalue * 255.0);
						}
					}

					bitmap->SetPicture(picture);
					//bitmap->Flip();
					bitmap->SetFilename(fileName);
				}
                break;
		}

		case JPEG:
		{
			cv::Mat picture;
			printf("CLibPicture LoadPicture JPEG \n");
			if (isThumbnail)
			{
#ifdef TURBOJPEG
				tjscalingfactor* scalingfactors = nullptr, sf = { 1, 1 };
				int nsf = 0;
				size_t _jpegSize;
				uint8_t* _compressedImage = CPictureUtility::readfile(fileName, _jpegSize);

				//unsigned char buffer[width*height*COLOR_COMPONENTS]; //!< will contain the decompressed image
				//Getting the size
				if (_compressedImage != nullptr && _jpegSize > 0)
				{
					int jpegSubsamp, width = 0, height = 0;


					tjhandle _jpegDecompressor = tjInitDecompress();


					tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height,
						&jpegSubsamp);


					if ((scalingfactors = tjGetScalingFactors(&nsf)) == nullptr || nsf == 0)
						throw("executing tjGetScalingFactors()");

					//int num = 1;
					//int denom = 4;
					//int defaultscaling = 0;
					int match = 0;

#ifdef WIN32

					HDC screen = GetDC(nullptr);
					RECT rcClip;
					GetClipBox(screen, &rcClip);
					ReleaseDC(nullptr, screen);

					int widthThumbnail = max(static_cast<int32_t>(rcClip.right / 4), 200);
					int heightThumbnail = max(static_cast<int32_t>(rcClip.bottom / 4), 200);

#else
					int widthThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, 200);
					int heightThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 4, 200);
#endif

					float ratio = CalculPictureRatio(width, height, widthThumbnail, heightThumbnail);

					for (int j = 0; j < nsf; j++)
					{
						sf = scalingfactors[j];
						float localRatio = static_cast<float>(sf.num) / static_cast<float>(sf.denom);
						if (localRatio < ratio)
						{
							if (j > 0)
								sf = scalingfactors[j - 1];

							match = 1;
							break;
						}
					}

					if (match == 0)
					{
						sf = scalingfactors[nsf - 1];
						match = 1;
					}

					if (match == 1)
					{
						width = TJSCALED(width, sf);
						height = TJSCALED(height, sf);
					}

					picture = cv::Mat(height, width, CV_8UC4);

					tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, picture.data,
						picture.size().width, 0, picture.size().height, TJPF_BGRX,
						TJFLAG_FASTDCT);

					tjDestroy(_jpegDecompressor);

					bitmap->SetPicture(picture);
					bitmap->SetFilename(fileName);

					delete[] _compressedImage;
				}
#else

				_cxImage = new CxImage();
				_cxImage->SetJpegScale(4);
				_cxImage->Load(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jpg"));
				bitmap->SetPicture(_cxImage);
                delete _cxImage;
#endif
			}
			else
			{
#ifdef TURBOJPEG

				printf("CLibPicture LoadPicture TURBOJPEG \n");
				size_t _jpegSize;
				uint8_t* _compressedImage = CPictureUtility::readfile(fileName, _jpegSize);


				if (_compressedImage == nullptr)
					cout << "File Failed To Load\n";
				else
				{
					int jpegSubsamp, width, height;

					tjhandle _jpegDecompressor = tjInitDecompress();


					tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height,
						&jpegSubsamp);

					picture = cv::Mat(height, width, CV_8UC4);

					tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, picture.data, width, 0,
						height, TJPF_BGRX, TJFLAG_FASTDCT);

					tjDestroy(_jpegDecompressor);

					cout << "JPEGTURBO END File Loaded Successfully\n";

					bitmap->SetPicture(picture);
					bitmap->SetFilename(fileName);
					//wxString filename = CFileUtility::GetFileName(fileName);
					//picture->WriteFile("e:\\" + filename + ".dat");

					delete[] _compressedImage;

					cout << "JPEGTURBO DELETE File Loaded Successfully\n";
				}

#else
				_cxImage = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jpg"));
				bitmap->SetPicture(_cxImage);
                delete _cxImage;
#endif
			}
		}
		break;


		case PNM:
		case PNG:
		case TIFF:
		case WEBP:
		case BMP:
		case PPM:
		{
			try
			{

				cv::Mat matPicture = cv::imread(CConvertUtility::ConvertToStdString(fileName), cv::IMREAD_COLOR | cv::IMREAD_IGNORE_ORIENTATION);
                if(!matPicture.empty())
                {
					bitmap->SetFilename(fileName);
                    bitmap->SetPicture(matPicture);
					//bitmap->Flip();
                }
			}
			catch (cv::Exception& e)
			{
				const char* err_msg = e.what();
				std::cout << "exception caught: " << err_msg << std::endl;
				std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
			}
		}
		break;

		case HDR:
		{
			try
			{

				cv::Mat hdr = cv::imread(CConvertUtility::ConvertToStdString(fileName),-1); // correct element size should be CV_32FC3
                if(!hdr.empty())
                {
                    cv::Ptr<cv::TonemapReinhard> tonemap = cv::createTonemapReinhard(1.0f);
                    tonemap->process(hdr, hdr);
                    hdr.convertTo(hdr, CV_8UC3, 255);
					bitmap->SetFilename(fileName);
                    bitmap->SetPicture(hdr);
					//bitmap->Flip();
                }

			}
			catch (cv::Exception& e)
			{
				const char* err_msg = e.what();
				std::cout << "exception caught: " << err_msg << std::endl;
				std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
			}

		}
		break;



		case IFF:
		{
			wxImage image;
			image.LoadFile(fileName, wxBITMAP_TYPE_IFF);
			bitmap->SetPicture(image);
		}
		break;

		case ICO:
		{
			wxImage image;
			image.LoadFile(fileName, wxBITMAP_TYPE_ICON);
			bitmap->SetPicture(image);
		}
		break;

		case CUR:
		{
			wxImage image;
			image.LoadFile(fileName, wxBITMAP_TYPE_CUR);
			bitmap->SetPicture(image);
		}
		break;

		case XPM:
		{
			wxImage image;
			image.LoadFile(fileName, wxBITMAP_TYPE_XPM);
			bitmap->SetPicture(image);
		}
		break;

		case SVG:
		{
            wxImage img = CLibResource::CreatePictureFromSVGFilename(fileName, svgWidth, svgHeight);
			wxRect rc;
			rc.x = 0;
			rc.y = 0;
			rc.width = svgWidth;
			rc.height = svgHeight;
			wxBitmap localmemBitmap_backup = wxBitmap(svgWidth, svgHeight);
			wxMemoryDC memDC(localmemBitmap_backup);
			wxBrush brush(*wxWHITE, wxBRUSHSTYLE_SOLID);
			memDC.SetBrush(brush);
			memDC.SetPen(wxPen(*wxWHITE, 1)); // 10-pixels-thick pink outline
			memDC.DrawRectangle(rc);
			memDC.SetPen(wxNullPen);
			memDC.SetBrush(wxNullBrush);

			memDC.DrawBitmap(img, 0, 0);

			memDC.SelectObject(wxNullBitmap);

			img.Destroy();


			wxImage local = localmemBitmap_backup.ConvertToImage();
            bitmap->SetPicture(local);

		}
		break;
#ifdef LIBBPG
		case BPG:
		{
			size_t data_size;
			cv::Mat picture;
			uint8_t* _compressedImage = CPictureUtility::readfile(fileName, data_size);
			if (_compressedImage != nullptr && data_size > 0)
			{
#if defined(WIN32)

				int width = 0, height = 0;
				//size_t size = 0;
				uint8_t* data = nullptr;
				if (BPG_GetDimensions(_compressedImage, data_size, width, height) == 0)
				{
					picture.create(height, width, CV_8UC4);
					int returnValue = 0;
					size_t data_len = width * height * 4;
					returnValue = BPG_GetPictureBGRA(_compressedImage, data_size, picture.data, data_len, width, height,
						true);
				}
#else
				int returnValue = 0;
				int width = 0, height = 0;
				size_t size = 0;

				void(*BPG_GetDimensions)(uint8_t*, uint64_t, int&, int&) = (void(*)(uint8_t*, uint64_t, int&, int&))dlsym(lib_handle, "BPG_GetDimensions");
				if (BPG_GetDimensions) {
					printf("[%s] dlsym(lib_handle, \"BPG_GetDimensions\"): Successful\n", __FILE__);
					BPG_GetDimensions(_compressedImage, data_size, width, height);
				}
				else {
					printf("[%s] Unable to get symbol: %s\n",
						__FILE__, dlerror());
				}

				uint8_t* data = new uint8_t[width * height * 4];
				size_t data_len = width * height * 4;

				int(*BPG_GetPictureBGRA)(uint8_t*, uint64_t, uint8_t*, uint64_t, int&, int&, bool) = (int(*)(uint8_t*, uint64_t, uint8_t*, uint64_t, int&, int&, bool))dlsym(lib_handle, "BPG_GetPictureBGRA");
				if (BPG_GetPictureBGRA) {
					printf("[%s] dlsym(lib_handle, \"BPG_GetPictureBGRA\"): Successful\n", __FILE__);
					returnValue = BPG_GetPictureBGRA(_compressedImage, data_size, data, data_len, width, height, true);
				}
				else {
					printf("[%s] Unable to get symbol: %s\n",
						__FILE__, dlerror());
				}

#endif
				if (!picture.empty())
				{
					bitmap->SetPicture(picture);
					bitmap->SetFilename(fileName);
				}
				delete[] _compressedImage;
			}
		}
		break;
#endif

#if defined(LIBRAW)
		case RAWFILE:

			CRaw::LoadPicture(fileName, bitmap);

			if(bitmap != nullptr)
				bitmap->SetFilename(fileName);

			applyExif = false;
			break;
#endif


		case JBIG:
			{
				auto _cxImage = new CxImage(fileName.ToStdWstring(),
				                            CxImage::GetTypeIdFromName("jbg"));
				bitmap->SetPicture(_cxImage);

                delete _cxImage;
			}
			break;

		case PDF:
			{
				CRegardsPDF regardsPdf(fileName);
				wxImage image = regardsPdf.GetPicture(numPicture);
				bitmap->SetPicture(image);
			}
			break;

		case ANI:
			{
				wxImage image;
				wxBitmapType bitmapType = wxBITMAP_TYPE_ANY;
				image.LoadFile(fileName, bitmapType, numPicture);
				bitmap->SetPicture(image);
			}
			break;

		case GIF:
			{
				wxFileName fichier(fileName);
				wxString extension = fichier.GetExt();

				auto _cxImage = new CxImage();
				_cxImage->SetRetreiveAllFrames(true);
				_cxImage->Load(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName(extension));
				if (_cxImage->GetNumFrames() > 1)
				{
					CxImage* frame = _cxImage->GetFrame(numPicture);
					bitmap->SetPicture(frame);
				}
				else
					bitmap->SetPicture(_cxImage);

                delete _cxImage;
			}
			break;

		case MPG2:
		case MPEG:
		case AVCHD:
		case WINREC:
		case AVI:
		case MP4:
		case WMV:
		case WEBM:
		case MKV:
		case AV1:
		case Y4M:
		case MOV:
			{
				CThumbnailVideo video(fileName, false);
				int percent = ((float)numPicture / (float)20) * 100.0f;
				cv::Mat mat = video.GetVideoFramePercent(percent, 0, 0);
				bitmap->SetPicture(mat);
				bitmap->SetOrientation(video.GetOrientation());
				bitmap->SetFilename(fileName);
			}
			break;

		default:
			cv::Mat mat = LoadFromFreeImage(CConvertUtility::ConvertToUTF8(fileName));
			if (!mat.empty())
			{
				bitmap->SetPicture(mat);
				bitmap->SetFilename(fileName);
			}
			break;
		}

#if defined(WIN32) && defined(_DEBUG)
	DWORD LasttickCount = GetTickCount();				// Get The Tick Count
	DWORD Result = LasttickCount - tickCount;
#endif

#if defined(WIN32) && defined(_DEBUG)
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"Convert\n");
#endif

    ApplyOrientation(fileName, applyExif, bitmap);

#ifdef ROTDETECT
		if(orientation == -1 || orientation == 1)
		{
			if (!isThumbnail)
			{
				CImageLoadingFormat * thumbnail = LoadThumbnail(fileName);
				int exifOrientation;
				float result[4];
				CRotDetect rotDetect;
				rotDetect.rotdetect(thumbnail, result, true);

				CRotDetect::rotation rot = rotDetect.analyzeResult(result);
				//printf("Orientation of %s is ", argv[optIndex]);
				switch (rot)
				{
				case CRotDetect::NOT_ROTATED:
					exifOrientation = 1;
					break;
				case CRotDetect::ROTATED90CW:
					exifOrientation = 6;
					break;
				case CRotDetect::ROTATED90CCW:
					exifOrientation = 5;
					break;
				default:
					exifOrientation = 1;
				}
				bitmap->SetOrientation(exifOrientation);

				delete thumbnail;
			}
		}
#endif
	}
	catch (...)
	{
	}

#if defined(WIN32) && defined(_DEBUG)
	DWORD LasttickCount = GetTickCount();				// Get The Tick Count
	DWORD Result = LasttickCount - tickCount;
	/*
	wchar_t Temp[10];
	swprintf_s(Temp, L"%d", Result);
	OutputDebugString(L"Render Time : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");
	*/
#endif

	if (!bitmap->IsOk())
	{
		//delete bitmap;
		//bitmap = LoadPicture(CPictureUtility::GetPhotoCancel());

		cv::Mat picture = cv::imread(CConvertUtility::ConvertToStdString(CPictureUtility::GetPhotoCancel()));
		bitmap->SetPicture(picture);
		bitmap->SetFilename(fileName);
	}
}

void CLibPicture::ApplyOrientation(const wxString & fileName, const bool & applyExif, CImageLoadingFormat * bitmap)
{
    int orientation = -1;
    if (TestIsExifCompatible(fileName) && applyExif)
    {
        CMetadataExiv2 metadata(fileName);
        orientation = metadata.GetOrientation();
        bitmap->SetOrientation(orientation);
    }   
}

bool CLibPicture::HasThumbnail(const wxString& filename)
{
#if defined(EXIV2)
	const int iFormat = TestImageFormat(filename);
	if (iFormat == RAWFILE)
		return true;
	if (iFormat == TIFF || iFormat == JPEG)
	{
		CMetadataExiv2 pictureMetadata(filename);
		return pictureMetadata.HasThumbnail();
	}
#endif
	return false;
}


//--------------------------------------------------------------------------------------------------
//Fonction d'appel pour l'obtention des dimensions d'une image
//--------------------------------------------------------------------------------------------------
int CLibPicture::GetPictureDimensions(const wxString& fileName, int& width, int& height, int& rotation)
{
	int iFormat = TestImageFormat(fileName);
	width = 0;
	height = 0;
	CxImage* image = nullptr;
	wxImage imageWx;
	int typeImage = TYPE_IMAGE_CXIMAGE;
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	switch (iFormat)
	{

	case JXL:
	{
		CJxl::GetDimensions(fileName,width,height);
		break;
	}

	case HDR:
		{
			cv::Mat hdr = cv::imread(CConvertUtility::ConvertToStdString(fileName), -1); // correct element size should be CV_32FC3
			width = hdr.cols;
			height = hdr.rows;
		}
		break;

	case EXR:
	{
		RgbaInputFile file(CConvertUtility::ConvertToUTF8(fileName));
		Box2i dw = file.dataWindow();
		width = dw.max.x - dw.min.x + 1;
		height = dw.max.y - dw.min.y + 1;
	}
	break;

	case PFM:
	{
		CPfm::GetDimensions(fileName, width, height);
	}
	break;

	case WEBP:
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			CRegardsWebp::GetPictureDimension(fileName, width, height);
			break;
		}


	case JPEG:
#ifdef TURBOJPEG
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			size_t _jpegSize;
			uint8_t* _compressedImage = CPictureUtility::readfile(fileName, _jpegSize);
			if (_compressedImage != nullptr && _jpegSize > 0)
			{
				int jpegSubsamp = 0;
				tjhandle _jpegDecompressor = tjInitDecompress();
				tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);
				tjDestroy(_jpegDecompressor);
				delete[] _compressedImage;
			}
		}
#else

		image = new CxImage(fileName.ToStdWstring(), CxImage::GetTypeIdFromName("jpg"), true);
#endif

		break;

	case JBIG:
		image = new CxImage(fileName.ToStdWstring(), CxImage::GetTypeIdFromName("jbg"), true);
		break;

	case PDF:
		{
			CRegardsPDF regardsPdf(fileName);
			imageWx = regardsPdf.GetPicture(0);
			typeImage = TYPE_IMAGE_WXIMAGE;
		}

		break;

	case IFF:
		imageWx.LoadFile(fileName, wxBITMAP_TYPE_IFF);
		typeImage = TYPE_IMAGE_WXIMAGE;
		break;

	case XPM:
		imageWx.LoadFile(fileName, wxBITMAP_TYPE_XPM);
		typeImage = TYPE_IMAGE_WXIMAGE;
		break;

	case RAWFILE:
		typeImage = TYPE_IMAGE_REGARDSIMAGE;
		CRaw::GetDimensions(fileName, width, height);
		break;

	case SVG:
		typeImage = TYPE_IMAGE_REGARDSIMAGE;
		width = this->svgWidth;
		height = this->svgHeight;
		break;
#ifdef LIBBPG
	case BPG:
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			size_t data_size;
			uint8_t* _compressedImage = CPictureUtility::readfile(fileName, data_size);
			if (_compressedImage != nullptr && data_size > 0)
			{
#if defined(WIN32)
				BPG_GetDimensions(_compressedImage, data_size, width, height);
#else
                void(*BPG_GetDimensions)(uint8_t * , uint64_t , int & , int & ) = (void(*)(uint8_t * , uint64_t , int & , int & ))dlsym(lib_handle, "BPG_GetDimensions");
                if (BPG_GetDimensions) {
                    printf("[%s] dlsym(lib_handle, \"BPG_GetDimensions\"): Successful\n", __FILE__);
                    BPG_GetDimensions(_compressedImage, data_size, width, height);
                }
                else {
                    printf("[%s] Unable to get symbol: %s\n",
                           __FILE__, dlerror());
                }
#endif
				delete[] _compressedImage;
			}
		}
		break;
#endif
	case MPG2:
	case MPEG:
	case AVCHD:
	case WINREC:
	case AVI:
	case MP4:
	case WMV:
	case WEBM:
	case MKV:
	case AV1:
	case Y4M:
	case MOV:
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			CThumbnailVideo video(fileName, false);
			video.GetVideoDimensions(width, height);
		}
		break;
#ifdef LIBHEIC
	case HEIC:
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			CHeic::GetPictureDimension(CConvertUtility::ConvertToStdString(fileName), width, height);
			//video.GetVideoDimensions(fileName, width, height, rotation);
		}
		break;
	case AVIF:
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			CAvif::GetPictureDimension(CConvertUtility::ConvertToStdString(fileName), width, height);
			//video.GetVideoDimensions(fileName, width, height, rotation);
		}
		break;
#endif

	default:
		PictureDimensionFreeImage(CConvertUtility::ConvertToUTF8(fileName), width, height);
		break;
	}


	if (typeImage == TYPE_IMAGE_CXIMAGE)
	{
		if (image != nullptr)
		{
			width = image->GetWidth();
			height = image->GetHeight();
			EXIFINFO* exif = image->GetExifInfo();
			if (exif != nullptr)
				rotation = exif->Orientation;
			else
				rotation = 0;
			delete image;
		}
	}
	else if (typeImage == TYPE_IMAGE_WXIMAGE)
	{
		if (imageWx.IsOk())
		{
			width = imageWx.GetWidth();
			height = imageWx.GetHeight();
		}
	}


	return 0;
}

void CLibPicture::InitFreeImage()
{
	FreeImage_Initialise(true);
	FreeImage_SetOutputMessage(&FreeImageErrorHandler);
}

void CLibPicture::UninitFreeImage()
{
	FreeImage_DeInitialise();
}
