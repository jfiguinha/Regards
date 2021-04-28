#include <header.h>
// DllPicture.cpp : définit les fonctions exportées pour l'application DLL.
//
#include "libPicture.h"
#define __FREEIMAGE__
#include <FreeImage.h>
#include "MetadataExiv2.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core.hpp>
#include <webp/decode.h>
#include <webp/encode.h>
#include <ximage.h>
#include <xfile.h>
#include <xiofile.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <DeepLearning.h>

#ifdef ROTDETECT
#include <rotdetect.h>
#endif
#include <RegardsBitmap.h>
#include <wx/wxpoppler.h>
#include <RegardsFloatBitmap.h>
#include <ImageVideoThumbnail.h>
#include <ImageLoadingFormat.h>
#include <ConvertUtility.h>
#include <picture_id.h>
#include <LibResource.h>
#include <wx/filename.h>
#include <wx/progdlg.h>
#include <wx/pdfdocument.h>
#include "pfm.h"
#ifdef LIBHEIC
#include <Heic.h>
#include <avif.h>
#endif
#ifdef LIBBPG
#if defined(WIN32)
#include <DllBpg.h>
#else
#include <dlfcn.h>
#endif
#endif
#include "PictureData.h"
#include <SqlPhotos.h>

#ifdef TURBOJPEG
#include <turbojpeg.h>
#endif
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/mstream.h>
#if defined(LIBRAW)
#include "raw.h"
#endif
#include <wx/wxpoppler.h>

#include "ScaleThumbnail.h"

#if defined(FFMPEG)
#include <videothumb.h>
//using namespace Regards::Video;
#endif

#if defined(EXIV2)
#include <MetadataExiv2.h>
using namespace Regards::exiv2;
#endif

#include <wx/image.h>

#include <FileUtility.h>

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

#include <wxSVG/SVGDocument.h>

#define TYPE_IMAGE_CXIMAGE 0
#define TYPE_IMAGE_WXIMAGE 1
#define TYPE_IMAGE_REGARDSIMAGE 2
#define OR ||

using namespace Regards::Sqlite;
using namespace Regards::Picture;



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

static void MyMessageFunction(FREE_IMAGE_FORMAT fif, const char * msg)
{
#ifndef NDEBUG
    wxMessageBox(msg,"FreeImage error", wxICON_ERROR);
#endif
}


CLibPicture::CLibPicture()
{
	svgWidth = 1024;
	svgHeight = 1024;
	configRegards = CParamInit::getInstance();
}

CLibPicture::~CLibPicture()
{
}


CRegardsBitmap * CLibPicture::LoadPictureToBGRA(const wxString &filename, bool &pictureOK, const int &resizeWidth, const int &resizeHeight)
{
	CRegardsBitmap * pictureData = nullptr;
	CLibPicture libPicture;

	CImageLoadingFormat * imageLoading = libPicture.LoadPicture(filename);

	if (imageLoading != nullptr)
	{
		if (resizeWidth != 0 && resizeHeight != 0)
		{
			float ratio = CalculPictureRatio(imageLoading->GetWidth(), imageLoading->GetHeight(), resizeWidth, resizeHeight);
			int width = imageLoading->GetWidth() * ratio;
			int height = imageLoading->GetHeight() * ratio;
			imageLoading->Resize(width, height, 1);
		}

		pictureOK = true;
		imageLoading->ApplyExifOrientation();
		pictureData = imageLoading->GetRegardsBitmap(true);
	}
	else
		pictureOK = false;


	if (imageLoading != nullptr)
		delete imageLoading;

	imageLoading = nullptr;

	return pictureData;
}

int CLibPicture::TestExtension(const wxString & ext)
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

bool CLibPicture::TestIsPicture(const wxString & szFileName)
{
    int numExt = 0;
    wxFileName fichier(szFileName);
    wxString extension = fichier.GetExt();

	if (ImageFormat(CConvertUtility::ConvertToUTF8(szFileName)) != FIF_UNKNOWN)
		return true;
    
    numExt = TestExtension(extension.Lower());
    if (numExt < 100 && numExt != ANI && numExt != 0)
        return true;
    return false;
}

bool CLibPicture::TestIsPDF(const wxString & szFileName)
{
	int numExt = 0;
	wxFileName fichier(szFileName.c_str());
	wxString extension = fichier.GetExt();

	numExt = TestExtension(extension.Lower());
	if (numExt == PDF)
		return true;
	return false;
}

bool CLibPicture::TestIsVideo(const wxString & szFileName)
{
	int numExt = 0;
	wxFileName fichier(szFileName.c_str());
	wxString extension = fichier.GetExt();

	numExt = TestExtension(extension.Lower());
	if (numExt > 100)
		return true;
	return false;
}

bool CLibPicture::TestIsAnimation(const wxString & szFileName)
{
	bool returnValue = false;
	int nbFrame = this->GetNbImage(szFileName);
	if (nbFrame > 1)
		returnValue = true;

    return returnValue;
}

//----------------------------------------------------------------------------
//Test si le format de l'image est exploitable
//----------------------------------------------------------------------------
int CLibPicture::TestImageFormat(const wxString & szFileName)
{
	int numExt = 0;
	wxDir testDir(szFileName);

	if (!testDir.IsOpened())
	{
		//int numExt = 0;
		wxFileName fichier(szFileName.c_str());
		wxString extension = fichier.GetExt();
		if (extension.size() < 3)
            return 0;

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		numExt = TestExtension(extension.Lower());
		if (numExt == 0)
		{
			//Test if I can found a valid extension
			fif = ImageFormat(CConvertUtility::ConvertToUTF8(szFileName));
		}
            
        numExt = TestExtension(extension.Lower());

	}
	return numExt;
}

//-----------------------------------------------------------------------------
//Conversion du format CXImage vers RegardsBitmap
//-----------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::ConvertCXImageToRegardsBitmap(CxImage * image, const int &orientation)
{
	CRegardsBitmap * bitmap = new CRegardsBitmap(image->GetWidth(), image->GetHeight());
	image->Encode2BGRA(bitmap->GetPtBitmap(), bitmap->GetBitmapSize(), false);
	if(!image->IsBGR())
		bitmap->ConvertToBgr();
	bitmap->RotateExif(orientation);
	return bitmap;
}

//-----------------------------------------------------------------------------
//Conversion du format CXImage vers RegardsBitmap
//-----------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::ConvertCXImageToRegardsBitmap(CxImage * image)
{

#if defined(WIN32) && defined(_DEBUG)
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"ConvertCXImageToRegardsBitmap\n");
#endif


	CRegardsBitmap * bitmap = nullptr;
	try
	{
		bitmap = new CRegardsBitmap(image->GetWidth(), image->GetHeight());
		image->Encode2BGRA(bitmap->GetPtBitmap(), bitmap->GetBitmapSize(), false);		
	}
	catch (...)
	{

	}

#if defined(WIN32) && defined(_DEBUG)
	/*
	DWORD LasttickCount = GetTickCount();				// Get The Tick Count
	DWORD Result = LasttickCount - tickCount;

	wchar_t Temp[10];
	swprintf_s(Temp, L"%d", Result);
	OutputDebugString(L"Render Time : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");
	*/
#endif


	return bitmap;
}

//-----------------------------------------------------------------------------
//Conversion du format CXImage vers RegardsBitmap
//-----------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::ConvertCXImageToScaleRegardsBitmapCRgbaquad(CxImage * image, const int &width, const int &height)
{
#if defined(WIN32) && defined(_DEBUG)
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"ConvertCXImageToScaleRegardsBitmap\n");
#endif

	float ratio = CScaleThumbnail::CalculRatio(image->GetWidth(), image->GetHeight(), width, height);

	CRegardsBitmap * bitmapOut = new CRegardsBitmap(image->GetWidth() * ratio, image->GetHeight() * ratio);
	uint8_t * data = bitmapOut->GetPtBitmap();
	image->InterpolationBicubicRGB(data, image->GetWidth() * ratio, image->GetHeight() * ratio);
	EXIFINFO * exif = image->GetExifInfo();
	if (exif != nullptr)
		bitmapOut->RotateExif(exif->Orientation);

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

	return bitmapOut;
}

//-----------------------------------------------------------------------------
//Conversion du format CXImage vers RegardsBitmap
//-----------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::ConvertCXImageToScaleRegardsBitmapBGR(CxImage * image, const int &width, const int &height)
{
#if defined(WIN32) && defined(_DEBUG)
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"ConvertCXImageToScaleRegardsBitmap\n");
#endif

	float ratio = CScaleThumbnail::CalculRatio(image->GetWidth(), image->GetHeight(), width, height);
	CRegardsBitmap * bitmapOut = new CRegardsBitmap(image->GetWidth() * ratio, image->GetHeight() * ratio);
	uint8_t * data = bitmapOut->GetPtBitmap();
	image->InterpolationBicubicBGR(data, image->GetWidth() * ratio, image->GetHeight() * ratio);

	EXIFINFO * exif = image->GetExifInfo();
	if (exif != nullptr)
		bitmapOut->RotateExif(exif->Orientation);

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

	return bitmapOut;

}

CxImage * CLibPicture::ConvertRegardsBitmapToCXImage(CRegardsBitmap * bitmap)
{
	CxImage * image = nullptr;
	if (bitmap != nullptr)
	{
		image = new CxImage();
		image->CreateFromArray(bitmap->GetPtBitmap(), bitmap->GetBitmapSize(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetBitmapDepth(), bitmap->GetWidthSize(), false, false);
	}
	return image;
}

int CLibPicture::SavePictureOption(const int &format, int &option, int &quality)
{
	int returnValue = 0;
	switch (format)
	{
	case BMP:
	{
		BmpOption bmpOption(nullptr);
		bmpOption.ShowModal();
		if (bmpOption.IsOk())
		{
			quality = bmpOption.CompressionOption();
			returnValue = 1;
		}

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
		PDFOption pdfOption(nullptr);
		pdfOption.ShowModal();
		if (pdfOption.IsOk())
		{
			option = pdfOption.CompressionOption();
			switch (option)
			{
				case 0:
				{
					CompressionOption jpegOption(nullptr);
					jpegOption.ShowModal();
					if (jpegOption.IsOk())
					{
						quality = jpegOption.CompressionLevel();
					}
					break;
				}
				case 1:
				{
					TiffOption tiffOption(nullptr);
					tiffOption.ShowModal();
					if (tiffOption.IsOk())
						quality = tiffOption.CompressionOption();
				}
				break;
			}
			returnValue = 1;
		}
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

	case TGA:
	case PCX:
	case MNG:
	case PPM:
	case XPM:
	case IFF:
	case PFM:
	case PNM:
	case JPC:
	{
		returnValue = 1;
	}
	break;

	}

	return returnValue;
}

int CLibPicture::SavePicture(const  wxString & fileName, CImageLoadingFormat * bitmap, const int &option, const int &quality)
{
	int iFormat = 0;

    //const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	iFormat = TestImageFormat(fileName);
    wxString informations_error = CLibResource::LoadStringFromResource(L"informationserror",1);
	switch (iFormat)
	{
	case PFM:
	{
		CRegardsFloatBitmap * regards = bitmap->GetFloatBitmap(true);
		CPfm::WriteFilePFM(regards, fileName, 1.0f);
		delete regards;
		break;
	}

	case BMP:
	{
		CRegardsBitmap * regards = bitmap->GetRegardsBitmap();
		int _option = BMP_DEFAULT;
		if(option == 1)
			_option = BMP_SAVE_RLE;
		int pitch = regards->GetBitmapWidth() * 4;
		FIBITMAP * Image = FreeImage_ConvertFromRawBits(regards->GetPtBitmap(), regards->GetBitmapWidth(), regards->GetBitmapHeight(), pitch, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
		if(!FreeImage_Save(FIF_BMP, Image, fileName, _option))
        {
			wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);//L"&Help";
			wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
            wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
        }
		FreeImage_Unload(Image);
		delete regards;
	}
	break;
    
    case IFF:
    {
		CRegardsBitmap * regards = bitmap->GetRegardsBitmap();
		int _option = IFF_DEFAULT;
		int pitch = regards->GetBitmapWidth() * 4;
		FIBITMAP * Image = FreeImage_ConvertFromRawBits(regards->GetPtBitmap(), regards->GetBitmapWidth(), regards->GetBitmapHeight(), pitch, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
		if(!FreeImage_Save(FIF_IFF, Image, fileName, _option))
        {
			wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);//L"&Help";
			wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
			wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
        }
		FreeImage_Unload(Image);
		delete regards;
    }

	case J2K:
	{
		CRegardsBitmap * regards = bitmap->GetRegardsBitmap();
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
		int pitch = regards->GetBitmapWidth() * 4;
		FIBITMAP * Image = FreeImage_ConvertFromRawBits(regards->GetPtBitmap(), regards->GetBitmapWidth(), regards->GetBitmapHeight(), pitch, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
		if(!FreeImage_Save(FIF_J2K, Image, fileName, _option))
        {
			wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);//L"&Help";
			wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
			wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
        }
		FreeImage_Unload(Image);
		delete regards;
	}
	break;

	case JP2:
	{
		CRegardsBitmap * regards = bitmap->GetRegardsBitmap();
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
		int pitch = regards->GetBitmapWidth() * 4;
		FIBITMAP * Image = FreeImage_ConvertFromRawBits(regards->GetPtBitmap(), regards->GetBitmapWidth(), regards->GetBitmapHeight(), pitch, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
		if(!FreeImage_Save(FIF_JP2, Image, fileName, _option))
        {
			wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);//L"&Help";
			wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
			wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
        }
		FreeImage_Unload(Image);
		delete regards;
	}
	break;

	case JXR:
	{
		CRegardsBitmap * regards = bitmap->GetRegardsBitmap();
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
		int pitch = regards->GetBitmapWidth() * 4;
		FIBITMAP * Image = FreeImage_ConvertFromRawBits(regards->GetPtBitmap(), regards->GetBitmapWidth(), regards->GetBitmapHeight(), pitch, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
		if(!FreeImage_Save(FIF_JXR, Image, fileName, _option))
        {
			wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);//L"&Help";
			wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
			wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
        }
		FreeImage_Unload(Image);
		delete regards;
	}
	break;

	case EXR:
	{
		CRegardsBitmap * regards = bitmap->GetRegardsBitmap();
		regards->ConvertToBgr();
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

		int pitch = regards->GetBitmapWidth() * 4;
		FIBITMAP * Image = FreeImage_ConvertFromRawBits(regards->GetPtBitmap(), regards->GetBitmapWidth(), regards->GetBitmapHeight(), pitch, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
		FIBITMAP * floatImage = FreeImage_ConvertToRGBAF(Image);
		if(!FreeImage_Save(FIF_EXR, floatImage, fileName, _option))
        {
			wxString labelInformations = CLibResource::LoadStringFromResource(L"LBLUNABLETOSAVEFILE", 1);//L"&Help";
			wxString errorinfos = CLibResource::LoadStringFromResource(L"informationserror", 1);
			wxMessageBox(labelInformations, errorinfos, wxICON_ERROR);
        }
		FreeImage_Unload(floatImage);
		FreeImage_Unload(Image);
		
		delete regards;
	}
	break;

	case WEBP:
	{
		CRegardsBitmap * regards = bitmap->GetRegardsBitmap();

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

		//float quality_factor = 100 - quality;
		uint8_t * output = nullptr;

		regards->VertFlipBuf();

		size_t size = WebPEncodeBGRA(regards->GetPtBitmap(),
			regards->GetBitmapWidth(), regards->GetBitmapHeight(), regards->GetWidthSize(),
			_option, &output);
		writefile(fileName, output, size);

		delete regards;
		free(output);
		break;
	}

	case TGA:
	{
		CxImage * image = bitmap->GetCxImage();
		image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("tga"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			informations_error, wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case PCX:
	{
		CxImage * image = bitmap->GetCxImage();
		image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("pcx"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			informations_error, wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case MNG:
	{
		CxImage * image = bitmap->GetCxImage();
		image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("mng"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			informations_error, wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case TIFF:
	{
		CxImage * image = bitmap->GetCxImage();
		image->SetCodecOption(option, CXIMAGE_FORMAT_TIF);
		image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("tif"));

		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			informations_error, wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case PNG:
	{
		CxImage * image = bitmap->GetCxImage();
		image->SetCodecOption(option, CXIMAGE_FORMAT_PNG);
		image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("png"));
		delete image;
	}
	break;

	case GIF:
	{
		CxImage * image = bitmap->GetCxImage();
		image->SetCodecOption(option, CXIMAGE_FORMAT_GIF);
		image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("gif"));

		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			informations_error, wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	#ifdef LIBBPG
		case BPG:
		{
			//int width = bitmap->GetWidth();
			//int height = bitmap->GetHeight();
			int lossless_mode = option;
			int compress_level = quality;

			CxImage * image = bitmap->GetCxImage();
			//image->SetCodecOption(option, CXIMAGE_FORMAT_PNG);
		

			uint8_t * buffer = 0;
			int32_t sizeLen = 0;
			image->Encode(buffer, sizeLen, CxImage::GetTypeIdFromName("png"));
	#if defined(WIN32)
			BPG_SavePNGPicture(buffer, sizeLen, compress_level, lossless_mode, 8, CConvertUtility::ConvertToUTF8(fileName));
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
			image->FreeMemory(buffer);
			delete image;
		}
		break;
	#endif

		case HEIC:
			{
				CRegardsBitmap * image = bitmap->GetRegardsBitmap();
				CHeic::SavePicture(fileName.ToStdString(), image, 100 - quality);
				break;
			}
		case AVIF:
		{
			CRegardsBitmap * image = bitmap->GetRegardsBitmap();
			CAvif::SavePicture(fileName.ToStdString(), image, 100 - quality);
            image->DeleteMemory(false);
			delete image;
            /*
			CRegardsBitmap * image = bitmap->GetRegardsBitmap();
			if (iFormat == HEIC)
			{
				CHeifAvif::SavePicture(fileName.ToStdString(), iFormat, image, 100 - quality);
				
			}
			else if (iFormat == AVIF)
			{
				CHeifAvif::SavePicture(fileName.ToStdString(), iFormat, image, 100 - quality);
				//CAvif::SavePicture(fileName.ToStdString(), image, 100 - quality);
				//image->DeleteMemory(false);
			}
			delete image;
            */
			break;
		}

		case JPEG:
		{
			CxImage * image = bitmap->GetCxImage();
			image->SetCodecOption(option, CXIMAGE_FORMAT_JPG);
			image->SetJpegQualityF((float)quality);
			image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jpg"));

			wxString error = image->GetLastError();
			if (error != "")
				wxMessageBox(error,
				informations_error, wxOK | wxICON_ERROR);
			delete image;
		}
		break;

		case PNM:
		{
			CxImage * image = bitmap->GetCxImage();
			image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("pnm"));
			wxString error = image->GetLastError();
			if (error != "")
				wxMessageBox(error,
				informations_error, wxOK | wxICON_ERROR);
			delete image;
		}
		break;

		case JPC:
		{
			CxImage * image = bitmap->GetCxImage();
			image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jpc"));
			wxString error = image->GetLastError();
			if (error != "")
				wxMessageBox(error,
				informations_error, wxOK | wxICON_ERROR);
			delete image;
		}
		break;

		case JPEG2000:
		{
			CxImage * image = bitmap->GetCxImage();
			image->SetJpegQualityF((float)quality);
			image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jp2"));
			wxString error = image->GetLastError();
			if (error != "")
				wxMessageBox(error,
				informations_error, wxOK | wxICON_ERROR);
			delete image;

		}
		break;

		case PPM:
		{
			CxImage * image = bitmap->GetCxImage();
			image->Save(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("ppm"));
			wxString error = image->GetLastError();
			if (error != "")
				wxMessageBox(error,
				informations_error, wxOK | wxICON_ERROR);
			delete image;

		}
		break;

		case PDF:
			{
				wxString fileToAdd = "";
				wxString file = "";
				wxString documentPath = CFileUtility::GetDocumentFolderPath();

	#ifdef WIN32
				wxString tempFolder = documentPath + "\\temp";
				if (!wxMkDir(tempFolder)) {
	#else
				wxString tempFolder = documentPath + "/temp";
				if (!wxMkDir(tempFolder, wxS_DIR_DEFAULT)) {
	#endif
					// handle the error here
				}
				//Save

			if (option == 0)
			{
#ifdef WIN32
				file = tempFolder + "\\temporary.jpg";
#else
				file = tempFolder + "/temporary.jpg";
#endif


				if (wxFileExists(file))
					wxRemoveFile(file);

				//JPEG
				CxImage * image = bitmap->GetCxImage();
				image->SetJpegQualityF((float)quality);
				image->Save(CConvertUtility::ConvertToUTF8(file), CxImage::GetTypeIdFromName("jpg"));
				delete image;

			}
			else
			{
#ifdef WIN32
				file = tempFolder + "\\temporary.tiff";
#else
				file = tempFolder + "/temporary.tiff";
#endif

				if (wxFileExists(file))
					wxRemoveFile(file);

				//JPEG
				CxImage * image = bitmap->GetCxImage();
				image->SetCodecOption(quality, CXIMAGE_SUPPORT_TIF);
				image->Save(CConvertUtility::ConvertToUTF8(file), CxImage::GetTypeIdFromName("tif"));
				delete image;
			}

				wxImage image;
				image.LoadFile(file);
				//int nResolutionX = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX);
				//int nResolutionY = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY);
			//	double dblDpiBy72 = (double)m_nDpi / 72.0;
				
				image.SetOption(wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_INCHES);
				//image.SetOption(wxIMAGE_OPTION_RESOLUTIONX, 200);// bitmap->GetResolution());
				//image.SetOption(wxIMAGE_OPTION_RESOLUTIONY, 200);// bitmap->GetResolution());
				//image.SetOption(wxIMAGE_OPTION_RESOLUTION, 200);//bitmap->GetResolution());
				
				SaveToPDF(&image, fileName, file, option, quality);
				::wxRemoveFile(file);
			}
			break;

		case XPM:
		{
			wxImage * image = bitmap->GetwxImage();
			image->SaveFile(fileName, wxBITMAP_TYPE_XPM);
			delete image;
		}
		break;
	}

    
    wxString openfile_error = CLibResource::LoadStringFromResource(L"ImpossibleOpenFile",1);
	if (TestIsExifCompatible(fileName))
	{
        
		if (wxFileName::FileExists(fileName))
		{
#ifdef EXIV2
            CMetadataExiv2 pictureMetadata(bitmap->GetFilename());
            pictureMetadata.CopyMetadata(fileName);
#endif
		}
		else
			wxMessageBox(openfile_error,
			informations_error, wxOK | wxICON_ERROR);
	}

	if (!wxFileName::FileExists(fileName))
		wxMessageBox(openfile_error,
		informations_error, wxOK | wxICON_ERROR);

	return 0;
}


bool CLibPicture::SaveToPDF( wxImage* poImage, const wxString &fileName, const wxString &pictureName, int option, int quality)
{
    if( poImage->HasOption( wxIMAGE_OPTION_RESOLUTIONUNIT ) )
    {
        //int nResolutionUnit= poImage->GetOptionInt( wxIMAGE_OPTION_RESOLUTIONUNIT );
        int nResolution= 0;

        // Get image resolution-
        if(  poImage->HasOption( wxIMAGE_OPTION_RESOLUTION ) )
        {
            nResolution= poImage->GetOptionInt( wxIMAGE_OPTION_RESOLUTION );
        }
        else if(  poImage->HasOption( wxIMAGE_OPTION_RESOLUTIONX ) && (  poImage->HasOption( wxIMAGE_OPTION_RESOLUTIONY ) ) )
        {
            int nResolutionX= poImage->GetOptionInt( wxIMAGE_OPTION_RESOLUTIONX );
            int nResolutionY= poImage->GetOptionInt( wxIMAGE_OPTION_RESOLUTIONY );

            if( nResolutionX == nResolutionY )
            {
                nResolution= nResolutionX;
            }
        }
        if( nResolution )
        {
        
			/*
            // Save image in a temporary file.
            wxString strTempFileName= wxFileName::CreateTempFileName( wxEmptyString );
            poImage->SaveFile(strTempFileName, wxBITMAP_TYPE_TIFF);

            // Create a PDF document, add a page, and put the image on it.
            wxPdfDocument oPdfDocument;
            oPdfDocument.AddPage( ( poImage->GetHeight() > poImage->GetWidth() ) ? wxPORTRAIT : wxLANDSCAPE );
            oPdfDocument.SetImageScale( (double)nResolution * ( nResolutionUnit == wxIMAGE_RESOLUTION_CM ? 2.54 : 1.0 ) / 72.0 );
            oPdfDocument.Image( strTempFileName, 0 , 0, 0, 0, wxT( "image/tiff" ) );            
            // Remove temporary file.
            ::wxRemoveFile( strTempFileName );
            oPdfDocument.Close();
            oPdfDocument.SaveAsFile(fileName);
			*/

			// Create a PDF document, add a page, and put the image on it.
			wxPdfDocument oPdfDocument;
			//int tpl = oPdfDocument.BeginTemplate(0, 0, poImage->GetWidth(), poImage->GetHeight());
			wxPrintOrientation orientation = (poImage->GetHeight() > poImage->GetWidth()) ? wxPORTRAIT : wxLANDSCAPE;
			oPdfDocument.AddPage(orientation);
			
			//float nResolutionUnit = wxIMAGE_RESOLUTION_CM ? 2.54 : 1.0;
			//float imageScale = (double)nResolution * (float)nResolutionUnit / (float)72.0;
			//double pictureWidth = oPdfDocument.GetPageWidth() * nResolutionUnit;
			//double pictureHeight = oPdfDocument.GetPageHeight() * nResolutionUnit;

			if (option == 0)
				oPdfDocument.Image(pictureName, 0, 0, oPdfDocument.GetPageWidth(), oPdfDocument.GetPageHeight(), wxT("image/jpeg"));
			else
				oPdfDocument.Image(pictureName, 0, 0, oPdfDocument.GetPageWidth(), oPdfDocument.GetPageHeight(), wxT("image/tiff"));

			oPdfDocument.Close();
			oPdfDocument.SaveAsFile(fileName);

            return true;
        }
    }
    // File saving not possible or not implemented.
    return false;
}


bool CLibPicture::TestIsExifCompatible(const wxString &filename)
{
	int iFormat = TestImageFormat(filename);
	if ((iFormat == JPEG || iFormat == PNG || iFormat == TIFF || iFormat == WEBP || iFormat == RAWFILE || iFormat == HEIC || iFormat == AVIF))
		return true;
	return false;
}

//-----------------------------------------------------------------------------
//Sauvegarde
//-----------------------------------------------------------------------------
int CLibPicture::SavePicture(const wxString & fileName, CImageLoadingFormat * bitmap)
{
    wxString wxfileName;
    int iFormat = 0;

    iFormat = TestImageFormat(fileName);
    wxfileName = fileName;
 
	int option = 0;
	int quality = 0;

	if (SavePictureOption(iFormat, option, quality) == 1)
	{
		if(!TestIsExifCompatible(fileName))
			bitmap->ApplyExifOrientation(bitmap->GetOrientation());

		SavePicture(wxfileName, bitmap, option, quality);
	}

	return 0;
}


//-----------------------------------------------------------------------------
//Sauvegarde
//-----------------------------------------------------------------------------
int CLibPicture::SavePicture(const wxString & fileNameIn, const wxString & fileNameOut)
{
	int option = 0;
	int quality = 0;
	int iFormat = TestImageFormat(fileNameOut);
	if (SavePictureOption(iFormat, option, quality) == 1)
	{
		int nbPicture = GetNbImage(fileNameIn);
		wxString msg = "in progress";
		wxProgressDialog dialog("Export File", "Checking...", nbPicture, nullptr, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
		int updatesize = 0;
		dialog.Update(updatesize, msg);
		
		for (int i = 0; i < nbPicture; i++)
		{
			int j = i + 1;
			wxString filenameOutput = "";
			CImageLoadingFormat * picture = LoadPicture(fileNameIn, false, i);
			if (!TestIsExifCompatible(fileNameOut))
				picture->ApplyExifOrientation(picture->GetOrientation());

			wxString message = "In progress : " + to_string(j) + "/" + to_string(nbPicture);
			if (false == dialog.Update(i, message))
			{
				break;
			}
			if (nbPicture > 1)
			{
				
				wxFileName filename(fileNameOut);
				filenameOutput = filename.GetPathWithSep() +  filename.GetName() + "_page_" + to_string(j) + "." + filename.GetExt();
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

CxImage * CLibPicture::ConvertwxImageToCxImage(const wxImage & image)
{
    CxImage * _image = nullptr;
    if (image.IsOk())
    {
        wxMemoryOutputStream outputStream;
        image.SaveFile(outputStream, wxBITMAP_TYPE_PNG);
        uint8_t * data = new uint8_t[outputStream.GetSize()];
        if(data != nullptr)
		{
			outputStream.CopyTo(data, outputStream.GetSize());
			CxMemFile memFile(data, outputStream.GetSize());
			_image = new CxImage(&memFile, CxImage::GetTypeIdFromName("png"));
			delete[] data;
		}
    }

    return _image;
}

CRegardsBitmap * CLibPicture::ConvertwxImageToRegardsBitmap(const wxImage & image)
{
    CRegardsBitmap * bitmap = nullptr;
    if (image.IsOk())
    {
        
		CxImage * _image = ConvertwxImageToCxImage(image);
        wxString error = _image->GetLastError();
        if (error == "")
            bitmap = ConvertCXImageToRegardsBitmap(_image);
    }

    return bitmap;
}


//--------------------------------------------------------------------------------------------
//Obtention d'un thumbnail à partir d'un buffer
//--------------------------------------------------------------------------------------------
/*
CRegardsBitmap * CLibPicture::LoadThumbnailFromBuffer(uint8_t * data, size_t size, const char * ext, const int &orientation)
{
	wxString sExt = ext;
    wxString wExt = sExt.SubString(1, sExt.size());
	//wExt.assign(sExt.begin()+1, sExt.end());
	int numExtension = TestExtension(wExt.c_str());
	return LoadThumbnailFromBuffer(data, size, numExtension, orientation);
}
*/


//--------------------------------------------------------------------------------------------
//Obtention d'un thumbnail à partir des informations exif
//--------------------------------------------------------------------------------------------
CImageLoadingFormat * CLibPicture::LoadVideoThumbnail(const  wxString & szFileName, const int &percent, int &timePosition)
{
	int iFormat = TestImageFormat(szFileName);
	CImageLoadingFormat * bitmap = nullptr;
	

#ifdef WIN32

	HDC screen = GetDC(0);
	RECT rcClip;
	GetClipBox(screen, &rcClip);
	ReleaseDC(0, screen);

	int widthThumbnail = max((int32_t)(rcClip.right / 4), (int32_t)200);
	int heightThumbnail = max((int32_t)(rcClip.bottom / 4), (int32_t)200);

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
					int rotation = 0;
					CThumbnailVideo video(szFileName);
					bitmap->SetPicture(video.GetVideoFrame(rotation, percent, timePosition, widthThumbnail, heightThumbnail));
					bitmap->SetOrientation(rotation);
					bitmap->SetFilename(szFileName);
					break;
				}
        }

	}
	catch (...)
	{

	}


	if(bitmap == nullptr || bitmap->GetWidth() == 0 || bitmap->GetHeight() == 0)
	{
		bitmap = GetCancelPhoto(szFileName, widthThumbnail, heightThumbnail);
	}

	return bitmap;
}

vector<CImageVideoThumbnail *> CLibPicture::LoadDefaultVideoThumbnail(const  wxString & szFileName, const int &size)
{
#ifdef WIN32

	HDC screen = GetDC(0);
	RECT rcClip;
	GetClipBox(screen, &rcClip);
	ReleaseDC(0, screen);

	int widthThumbnail = max((int32_t)(rcClip.right / 4), (int32_t)200);
	int heightThumbnail = max((int32_t)(rcClip.bottom / 4), (int32_t)200);

#else
	int widthThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, 200);
	int heightThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 4, 200);
#endif

	vector<CImageVideoThumbnail *> listThumbnail;
    int rotation = 0;


	//CThumbnailVideo video(szFileName);
	//int movieDuration = 0;
	int pourcentage = 0;

	//Test if is animation
	bool isAnimation = false;
	isAnimation = TestIsAnimation(szFileName);

	for (auto i = 0; i < size; i++)
	{
		float percent = ((float)i / (float)size) * 100.0f;
		CImageVideoThumbnail * cxVideo = new CImageVideoThumbnail();
		//CRegardsBitmap * picture = nullptr;
		//int timePosition = 0;
		cxVideo->rotation = rotation;
		cxVideo->percent = percent;

#ifdef WIN32
		wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "\\loading.png";
#else
		wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "/loading.png";
#endif

		CImageLoadingFormat * picture = LoadPicture(photoCancel);
		picture->Resize(widthThumbnail, heightThumbnail, 0);
		picture->SetFilename(CConvertUtility::ConvertToStdString(szFileName));
		cxVideo->image = new CImageLoadingFormat();
		cxVideo->image->SetPicturToJpeg(picture->GetRegardsBitmap());
		if (isAnimation)
			cxVideo->timePosition = i;
		else
			cxVideo->timePosition = pourcentage * percent;
		cxVideo->image->SetFilename(szFileName);
		cxVideo->image->SetOrientation(rotation);
		delete picture;
		listThumbnail.push_back(cxVideo);
	}

	return listThumbnail;
}


//--------------------------------------------------------------------------------------------
//Obtention d'un thumbnail à partir des informations exif
//--------------------------------------------------------------------------------------------
/*
vector<CImageVideoThumbnail *> CLibPicture::LoadAllVideoThumbnail(const  wxString & szFileName, const bool &compressJpeg)
{
	int iFormat = TestImageFormat(szFileName);
	vector<CImageVideoThumbnail *> listThumbnail;
	LoadAllVideoThumbnail(szFileName, &listThumbnail, compressJpeg);
	return listThumbnail;
}
*/

void CLibPicture::LoadwxImageThumbnail(const wxString & szFileName, vector<CImageVideoThumbnail *> * listThumbnail, const int & bitmapType, const int &width, const int &height, const bool &compressJpeg, const bool & isThumbnail)
{
	wxPoppler poppler;
    wxImage image;
    wxBitmapType bitmapTypeWxWidget;
    
	//int iFormat = TestImageFormat(szFileName);
	//bool needresize = true;
    int m_ani_images = 0;
	if (bitmapType == PDF)
	{
		try
		{
			bool isValid = poppler.Open(szFileName);
			if (isValid)
			{
				m_ani_images = poppler.GetPageCount();
				poppler.SetDpi(75);
			}
		}
		catch (...)
		{

		}

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
		wxLogError(wxT("No ANI-format images found"));
		CImageLoadingFormat * photo_cancel = LoadPicture(CLibResource::GetPhotoCancel());
		if (photo_cancel->IsOk())
		{
			CRegardsBitmap * bitmap = photo_cancel->GetRegardsBitmap();
			bitmap->SetFilename(szFileName);
			CImageVideoThumbnail * imageVideoThumbnail = new CImageVideoThumbnail();
			imageVideoThumbnail->image = new CImageLoadingFormat();
			if (compressJpeg)
			{
				imageVideoThumbnail->image->SetPicturToJpeg(bitmap);
				delete bitmap;
			}
			else
				imageVideoThumbnail->image->SetPicture(bitmap);
			imageVideoThumbnail->image->SetFilename(szFileName);

			imageVideoThumbnail->rotation = 0;
			imageVideoThumbnail->delay = 4;
			imageVideoThumbnail->percent = 100.0f;
			imageVideoThumbnail->timePosition = 0;
			listThumbnail->push_back(imageVideoThumbnail);
			
			
		}
		delete photo_cancel;
	}
	else
	{
		for (auto i = 0; i < m_ani_images; i++)
		{
			bool returnValue = false;
			image.Destroy();
			if (bitmapType == PDF)
			{
				returnValue = poppler.SelectPage(i);
				if (returnValue)
					returnValue = poppler.RenderPage();

				if (returnValue)
					image = poppler.GetImage();
			}
			else
			{
				returnValue = image.LoadFile(szFileName, bitmapTypeWxWidget, i);
			}


			if (!returnValue)
			{
				wxString tmp = wxT("Can't load image number ");
				tmp << i;
				wxLogError(tmp);
			}
			else
			{
				CRegardsBitmap * bitmap = nullptr;
				//wxMemoryDC memdc;
				if (isThumbnail)
				{
					int bw, bh;

					float scale_x = float(width) / float(image.GetWidth());
					float scale_y = float(height) / float(image.GetHeight());

					double m_zoomFactor = min(scale_x, scale_y);
					bw = (int)(image.GetWidth() * m_zoomFactor);
					bh = (int)(image.GetHeight() * m_zoomFactor);
					wxImage bitmapResize = image.ResampleBicubic(bw, bh);
					bitmap = ConvertwxImageToRegardsBitmap(bitmapResize);
				}
				else
					bitmap = ConvertwxImageToRegardsBitmap(image);
				//bitmap->SetFilename(szFileName);
				//CScaleThumbnail::CreateScaleBitmap(bitmap, width, height);
				CImageVideoThumbnail * imageVideoThumbnail = new CImageVideoThumbnail();
				imageVideoThumbnail->image = new CImageLoadingFormat();
				if (compressJpeg)
				{
					imageVideoThumbnail->image->SetPicturToJpeg(bitmap);
					delete bitmap;
				}
				else
					imageVideoThumbnail->image->SetPicture(bitmap);
				imageVideoThumbnail->image->SetFilename(szFileName);

				imageVideoThumbnail->rotation = 0;
				imageVideoThumbnail->delay = 4;
				imageVideoThumbnail->percent = (int)((float)i / (float)m_ani_images) * 100.0f;
				imageVideoThumbnail->timePosition = i;
				listThumbnail->push_back(imageVideoThumbnail);
			}
		}
	}

}

int CLibPicture::GetNbImage(const  wxString & szFileName)
{
	int iFormat = TestImageFormat(szFileName);

	switch (iFormat)
	{
#ifdef LIBHEIC
		case AVIF:
		{
			return CAvif::GetNbFrame(szFileName.ToStdString());
		}
		break;
		case HEIC:
		{
			return CHeic::GetNbFrame(szFileName.ToStdString());
		}
		break;
#endif

		case PDF:
		{
			try
			{
				wxPoppler poppler;
				bool isValid = poppler.Open(szFileName);
				if(isValid)
					return poppler.GetPageCount();
			}
			catch (...)
			{
			}
			return 0;
		}
		
        
		case TIFF:
		case ANI:
		{
			wxBitmapType bitmapType = wxBITMAP_TYPE_ANY;
			return wxImage::GetImageCount(szFileName, bitmapType);
			break;
		}
        
        case PNG:
        case GIF:
        {
            wxFileName fichier(szFileName);
            wxString extension = fichier.GetExt();            
            
            CxImage * _cxImage = new CxImage();	 
            _cxImage->SetRetreiveAllFrames(true);
            _cxImage->Load(CConvertUtility::ConvertToUTF8(szFileName), CxImage::GetTypeIdFromName(extension.c_str()));
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
			return 20;
			break;
		}
	}

	return 1;
}

CImageLoadingFormat * CLibPicture::LoadThumbnailFromRawPicture(const wxString & szFilename, const int &width, const int &height, int &returnValue)
{
	CImageLoadingFormat * image = CRaw::GetThumbnail(szFilename, true);
	if (image == nullptr)
	{
		return nullptr;
	}

	return image;
}

uint32_t CLibPicture::GetFrameDelay(const  wxString & szFileName)
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

			CxImage * _cxImage = new CxImage();
			_cxImage->SetRetreiveAllFrames(true);
			_cxImage->Load(CConvertUtility::ConvertToUTF8(szFileName), CxImage::GetTypeIdFromName("gif"));
			delay = _cxImage->GetFrameDelay();
			delete _cxImage;

		}
		break;
	}
	return delay;
}

CImageLoadingFormat * CLibPicture::GetCancelPhoto(const wxString &szFileName, const int &widthThumbnail, const int &heightThumbnail)
{
#ifdef WIN32
	wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
	wxString photoCancel = CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif

	CImageLoadingFormat * bitmap = new CImageLoadingFormat();
	bitmap = LoadPicture(photoCancel);
	if(widthThumbnail > 0 && heightThumbnail > 0)
		bitmap->Resize(widthThumbnail, heightThumbnail, 0);
	bitmap->SetFilename(szFileName);

	return bitmap;
}

void CLibPicture::LoadAllVideoThumbnail(const  wxString & szFileName, vector<CImageVideoThumbnail *> * listThumbnail, const bool &compressJpeg, const bool &isThumbnail)
{
	int iFormat = TestImageFormat(szFileName);
#ifdef WIN32

	HDC screen = GetDC(0);
	RECT rcClip;
	GetClipBox(screen, &rcClip);
	ReleaseDC(0, screen);

	int widthThumbnail = max((int32_t)(rcClip.right / 4), (int32_t)200);
	int heightThumbnail = max((int32_t)(rcClip.bottom / 4), (int32_t)200);

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
			case AVIF:
			case HEIC:
			{
				int delay = 4;
				bool isMaster;
				vector<CRegardsBitmap *> listPicture;
				if(iFormat == HEIC)
					listPicture = CHeic::GetAllPicture(CConvertUtility::ConvertToStdString(szFileName), isMaster, delay);
				else if (iFormat == AVIF)
					listPicture = CAvif::GetAllPicture(CConvertUtility::ConvertToStdString(szFileName), delay);
				for (auto i = 0; i < listPicture.size(); i++)
				{
					CImageVideoThumbnail * imageVideoThumbnail = new CImageVideoThumbnail();
					imageVideoThumbnail->image = new CImageLoadingFormat();
					imageVideoThumbnail->image->SetFilename(szFileName);
					if (compressJpeg)
					{
						imageVideoThumbnail->image->SetPicturToJpeg(listPicture.at(i));
						delete listPicture.at(i);
					}
					else
						imageVideoThumbnail->image->SetPicture(listPicture.at(i));
					
					imageVideoThumbnail->rotation = 0;
					imageVideoThumbnail->delay = delay;
					imageVideoThumbnail->percent = (int)(((float)i / (float)listPicture.size()) * 100.0f);
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
                LoadwxImageThumbnail(szFileName, listThumbnail, iFormat, widthThumbnail, heightThumbnail, compressJpeg, isThumbnail);
				break;

			
            case PNG:
			case GIF:
				{
                    
                    wxFileName fichier(szFileName);
                    wxString extension = fichier.GetExt();  
					CxImage * _cxImage = new CxImage();	 
					_cxImage->SetRetreiveAllFrames(true);
					_cxImage->Load(CConvertUtility::ConvertToUTF8(szFileName), CxImage::GetTypeIdFromName(extension.c_str()));
					if (_cxImage->GetNumFrames() > 1)
					{
						for (auto i = 0; i < _cxImage->GetNumFrames(); i++)
						{
							CImageVideoThumbnail * imageVideoThumbnail = new CImageVideoThumbnail();
							CxImage * frame = _cxImage->GetFrame(i);
                            CRegardsBitmap * _local = ConvertCXImageToRegardsBitmap(frame,0);
                            _local->SetFilename(szFileName);
							imageVideoThumbnail->image = new CImageLoadingFormat();
							imageVideoThumbnail->image->SetFilename(szFileName);
							if (compressJpeg)
							{
								imageVideoThumbnail->image->SetPicturToJpeg(_local);
								delete _local;
							}
							else
								imageVideoThumbnail->image->SetPicture(_local);
							imageVideoThumbnail->rotation = 0;
							imageVideoThumbnail->delay = _cxImage->GetFrameDelay();
							imageVideoThumbnail->percent = ((float)i / (float)_cxImage->GetNumFrames()) * 100.0f;
							imageVideoThumbnail->timePosition = i;
							listThumbnail->push_back(imageVideoThumbnail);
						}
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
				CThumbnailVideo video(szFileName);
                //CConvertUtility::ConvertToStdString
				vector<CImageVideoThumbnail *> listVideo = video.GetVideoListFrame(widthThumbnail, heightThumbnail, compressJpeg);
				for (CImageVideoThumbnail * cxVideo : listVideo)
				{
					listThumbnail->push_back(cxVideo);
				}
				break;
			}

			default:
			{
				CImageVideoThumbnail * imageVideoThumbnail = new CImageVideoThumbnail();
				imageVideoThumbnail->image = LoadThumbnail(szFileName);
				imageVideoThumbnail->image->SetFilename(szFileName);
				imageVideoThumbnail->rotation = 0;
				imageVideoThumbnail->delay = 0;
				imageVideoThumbnail->percent = 0;
				imageVideoThumbnail->timePosition = 0;
				listThumbnail->push_back(imageVideoThumbnail);

				
			}
		}
    }
    catch (...)
	{

		CImageLoadingFormat * bitmap = GetCancelPhoto(szFileName, widthThumbnail, heightThumbnail);

		CImageVideoThumbnail * imageVideoThumbnail = new CImageVideoThumbnail();
		imageVideoThumbnail->image = bitmap;
		imageVideoThumbnail->image->SetFilename(szFileName);
		imageVideoThumbnail->rotation = 0;
		imageVideoThumbnail->delay = 0;
		imageVideoThumbnail->percent = 0;
		imageVideoThumbnail->timePosition = 0;
		listThumbnail->push_back(imageVideoThumbnail);
	}

}

bool CLibPicture::TestIsVideoValid(const wxString & szFileName)
{
	bool isValid = true;

	if (fileValid.find(szFileName) != fileValid.end())
	{
		isValid = fileValid[szFileName];
	}
	else
	{
		CThumbnailVideo video(szFileName);
		isValid = video.IsOk();
		fileValid.insert(std::make_pair(szFileName, isValid));
	}
	return isValid;

}

//--------------------------------------------------------------------------------------------
//Obtention d'un thumbnail à partir des informations exif
//--------------------------------------------------------------------------------------------
CImageLoadingFormat * CLibPicture::LoadThumbnail(const wxString & fileName, const bool &fromExifOnly)
{
    //const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	CImageLoadingFormat * imageLoading = nullptr;
	
#ifdef WIN32

	HDC screen = GetDC(0);
	RECT rcClip;
	GetClipBox(screen, &rcClip);
	ReleaseDC(0, screen);

	int widthThumbnail = max((int32_t)(rcClip.right / 4), (int32_t)200);
	int heightThumbnail = max((int32_t)(rcClip.bottom / 4), (int32_t)200);

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
		CMetadataExiv2 pictureMetadata(fileName);
		//int orientation = pictureMetadata.GetOrientation();
		imageLoading = CRaw::GetThumbnail(fileName,true);
		if(imageLoading != nullptr && imageLoading->IsOk())
		{
			imageLoading->Resize(widthThumbnail, heightThumbnail, 0);
			imageLoading->ApplyExifOrientation();
            
		}

	}
    else if(TestIsVideo(fileName))
    {
        imageLoading = new CImageLoadingFormat();
        imageLoading->SetFilename(fileName);
        imageLoading = LoadPicture(fileName, true);
		if(imageLoading != nullptr && imageLoading->IsOk())
			imageLoading->Resize(widthThumbnail, heightThumbnail, 0);
    }
	else if (iFormat == HEIC)
	{
		CRegardsBitmap * bitmap = CHeic::GetThumbnailPicture(fileName.ToStdString());
		if (bitmap != nullptr)
		{
			imageLoading = new CImageLoadingFormat();
			bitmap->SetFilename(fileName);
			imageLoading->SetPicture(bitmap);
			if (imageLoading != nullptr && imageLoading->IsOk())
			{
				imageLoading->Resize(widthThumbnail, heightThumbnail, 1);
				imageLoading->ApplyExifOrientation();
			}
		}
	}
    else
    {
#endif		

         printf("CLibPicture::LoadThumbnail \n");
        int orientation = -1;
        wxString extension;
		CxMemFile * memFile = nullptr;
        CMetadataExiv2 pictureMetadata(fileName);
		if(pictureMetadata.HasThumbnail())
		{
			memFile = pictureMetadata.DecodeThumbnail(extension, orientation);
		}
        if (memFile == nullptr && !fromExifOnly)
        {
			imageLoading = new CImageLoadingFormat();
			imageLoading->SetFilename(fileName);
			CRegardsBitmap * thumbnail = nullptr;//LoadThumbnailFromFreeImage(fileName);
			if (thumbnail == nullptr)
			{
				imageLoading = LoadPicture(fileName, true);
				if (imageLoading != nullptr && imageLoading->IsOk())
				{
					imageLoading->Resize(widthThumbnail, heightThumbnail, 1);
					imageLoading->ApplyExifOrientation();
				}
			}
			else
			{
				imageLoading->SetPicture(thumbnail);
			}
        }
        else if(memFile != nullptr)
        {
			CxImage * image = new CxImage(memFile, CxImage::GetTypeIdFromName(CConvertUtility::ConvertToUTF8(extension)));
			if(image->GetWidth() > 0 && image->GetHeight() > 0)
			{
				imageLoading = new CImageLoadingFormat();
				imageLoading->SetFilename(fileName);
				imageLoading->SetPicture(image);
				if(imageLoading->IsOk())
				{
					//imageLoading->ConvertToBGR(true);
					imageLoading->Resize(widthThumbnail, heightThumbnail, 1);
					imageLoading->ApplyExifOrientation();
				}
			}
			else
			{
				imageLoading = LoadPicture(fileName, true);
				if(imageLoading != nullptr && imageLoading->IsOk())
				{
					imageLoading->Resize(widthThumbnail, heightThumbnail, 1);
					imageLoading->ApplyExifOrientation();
				}
			}
			delete memFile;
        }


#ifdef LIBRAW			
	}
#endif
      
#endif


	if(imageLoading == nullptr || !imageLoading->IsOk())
	{
		if (imageLoading != nullptr)
			delete imageLoading;

		imageLoading = GetCancelPhoto(fileName, widthThumbnail, heightThumbnail);
	}
	

	if (imageLoading->GetOrientation() == -1 && configRegards->GetDetectOrientation())
	{
		CSqlPhotos sqlPhotos;
		int exif = sqlPhotos.GetPhotoExif(fileName);
		if (exif == -1)
		{
			bool pictureOK;
			CPictureData* pictureData = LoadPictureData(fileName, pictureOK);
			if (pictureData != nullptr)
			{
				if (pictureOK)
				{
					int exif = Regards::DeepLearning::CDeepLearning::GetExifOrientation(pictureData);
					imageLoading->ApplyExifOrientation(exif);
					sqlPhotos.InsertPhotoExif(fileName, exif);
				}
				delete pictureData;
			}
		}

	}
	
	return imageLoading;
}

void CLibPicture::DefineSVGSize(const int &width, const int &height)
{
    svgWidth = width;
    svgHeight = height;
}


float CLibPicture::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthThumbnail, const int &heightThumbnail)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		newRatio = (float)widthThumbnail / (float)(pictureWidth);
	else
		newRatio = (float)heightThumbnail / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > heightThumbnail)
	{
		newRatio = (float)heightThumbnail / (float)(pictureHeight);
	}
	else
	{
		if ((pictureWidth * newRatio) > widthThumbnail)
		{
			newRatio = (float)widthThumbnail / (float)(pictureWidth);
		}
	}

	return newRatio;
}

bool CLibPicture::PictureDimensionFreeImage(const char* filename, int &width, int &height)
{
#ifdef __FREEIMAGE__
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
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

CRegardsBitmap * CLibPicture::LoadFromFreeImage(const char* filename)
{
	CRegardsBitmap * bitmap = nullptr;
#ifdef __FREEIMAGE__
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	FIBITMAP *dibRgba(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return bitmap;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return bitmap;

	dibRgba = FreeImage_ConvertTo32Bits(dib);

	//retrieve the image data
	bits = FreeImage_GetBits(dibRgba);
	//get the image width and height
	width = FreeImage_GetWidth(dibRgba);
	height = FreeImage_GetHeight(dibRgba);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return bitmap;

	bitmap = new CRegardsBitmap();
	bitmap->SetBitmap(bits, width, height);
	bitmap->SetFilename(filename);
	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);
	FreeImage_Unload(dibRgba);
	//return success
#endif
	return bitmap;
}

//------------------------------------------------------------------------------
//Chargement d'une image par son nom
//------------------------------------------------------------------------------
CImageLoadingFormat * CLibPicture::LoadPicture(const wxString & fileName, const bool &isThumbnail, const int &numPicture)
{
#if defined(WIN32) && defined(_DEBUG)
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"LoadPicture\n");
#endif

    //const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
    printf("CLibPicture LoadPicture \n");
	CImageLoadingFormat * bitmap = new CImageLoadingFormat();
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
			CRegardsBitmap * picture = nullptr;

			if (numPicture == 0)
			{
				if (isThumbnail)
					picture = CHeic::GetThumbnailPicture(CConvertUtility::ConvertToStdString(fileName));
				
                if(picture == nullptr)
					picture = CHeic::GetPicture(CConvertUtility::ConvertToStdString(fileName));
			}
			else
			{
				int delay = 4;
				bool isMaster;
				picture = CHeic::GetPicture(CConvertUtility::ConvertToStdString(fileName), isMaster, delay, numPicture);
			}

			if (picture != nullptr)
			{
				bitmap->SetPicture(picture);
				bitmap->SetFilename(fileName);
			}
			break;
		}
		case AVIF:
		{
			CRegardsBitmap * picture = nullptr;

			if (numPicture == 0)
			{
                picture = CAvif::GetPicture(CConvertUtility::ConvertToStdString(fileName));
			}
			else
			{
				int delay = 4;
				picture = CAvif::GetPicture(CConvertUtility::ConvertToStdString(fileName), delay, numPicture);
			}

			if (picture != nullptr)
			{
				bitmap->SetPicture(picture);
				bitmap->SetFilename(fileName);
			}
		}
		break;
#endif

		case PFM:
		{
			CRegardsFloatBitmap * test = CPfm::ReadFilePFM(fileName, isThumbnail);
			bitmap->SetPicture(test);
			break;
		}

		/*/
		cout << "Width : " << src.cols << endl;
		cout << "Height: " << src.rows << endl;
		*/
		case EXR:
        case HDR:
            {
				CRegardsBitmap* picture = nullptr;
				cv::Mat hdr = cv::imread(fileName.ToStdString(), -1); // correct element size should be CV_32FC3
				//cv::Mat ldr;
				cv::Ptr<cv::TonemapReinhard> tonemap = cv::createTonemapReinhard(1.0f);
				tonemap->process(hdr, hdr);
				hdr.convertTo(hdr, CV_8UC3, 255);
				cv::cvtColor(hdr, hdr, cv::COLOR_RGB2BGRA);
				picture = new CRegardsBitmap();
				picture->SetBitmap(hdr.data, hdr.cols, hdr.rows);
				picture->VertFlipBuf();
				picture->SetFilename(fileName);
				bitmap->SetPicture(picture);
            }
            break; 

		case PPM:
			{
			CxImage * _cxImage = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("ppm"));
			bitmap->SetPicture(_cxImage);
			}
			break;

		case IFF:
			{
			wxImage * image = new wxImage();
			image->LoadFile(fileName, wxBITMAP_TYPE_IFF); 
			bitmap->SetPicture(image);
			}
			break;

        case ICO:
			{
			wxImage * image = new wxImage();
            image->LoadFile(fileName, wxBITMAP_TYPE_ICON);
			bitmap->SetPicture(image);
			}
            break;
 
        case CUR:
			{
			wxImage * image = new wxImage();
            image->LoadFile(fileName, wxBITMAP_TYPE_CUR); 
			bitmap->SetPicture(image);
			}
            break;
                
		case XPM:
			{
			wxImage * image = new wxImage();
			image->LoadFile(fileName, wxBITMAP_TYPE_XPM);
			bitmap->SetPicture(image);
			}
			break;
                
        case SVG:
        {
			wxImage * image = new wxImage();
            wxSVGDocument* svgDoc = new wxSVGDocument;
            svgDoc->Load(fileName);
			*image = svgDoc->Render(svgWidth,svgHeight);
            bitmap->SetPicture(image);
            delete svgDoc;
        }
        break;
#ifdef LIBBPG
		case BPG:
		{
			size_t data_size;
			uint8_t* _compressedImage = readfile(fileName, data_size);
			if(_compressedImage != nullptr && data_size > 0)
			{
#if defined(WIN32)
				int returnValue = 0;
				int width = 0, height = 0;
				//size_t size = 0;
                uint8_t * data = nullptr;
                if(BPG_GetDimensions(_compressedImage, data_size, width, height) == 0)
                {
                    data = new uint8_t[width * height * 4];
                    size_t data_len = width * height * 4;
                    returnValue = BPG_GetPictureBGRA(_compressedImage, data_size, data, data_len, width, height, true);
                }
#else
                int returnValue = 0;
                int width = 0, height = 0;
                size_t size = 0;
                
                void(*BPG_GetDimensions)(uint8_t * , uint64_t , int & , int & ) = (void(*)(uint8_t * , uint64_t , int & , int & ))dlsym(lib_handle, "BPG_GetDimensions");
                if (BPG_GetDimensions) {
                    printf("[%s] dlsym(lib_handle, \"BPG_GetDimensions\"): Successful\n", __FILE__);
                    BPG_GetDimensions(_compressedImage, data_size, width, height);
                }
                else {
                    printf("[%s] Unable to get symbol: %s\n",
                           __FILE__, dlerror());
                }

                uint8_t * data = new uint8_t[width * height * 4];
                size_t data_len = width * height * 4;
                
                int(*BPG_GetPictureBGRA)(uint8_t * , uint64_t , uint8_t * , uint64_t , int & , int & , bool  ) = (int(*)(uint8_t * , uint64_t , uint8_t * , uint64_t , int & , int & , bool ))dlsym(lib_handle, "BPG_GetPictureBGRA");
                if (BPG_GetPictureBGRA) {
                    printf("[%s] dlsym(lib_handle, \"BPG_GetPictureBGRA\"): Successful\n", __FILE__);
                    returnValue = BPG_GetPictureBGRA(_compressedImage, data_size, data, data_len, width, height, true);
                }
                else {
                    printf("[%s] Unable to get symbol: %s\n",
                           __FILE__, dlerror());
                }

#endif
                if(data != nullptr)
                {
                    CRegardsBitmap * picture = new CRegardsBitmap();
                    picture->SetBitmap(data, width, height, false, false);
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
			bitmap = CRaw::GetThumbnail(fileName, false);
			break;
#endif
                
			/*
		case EXR:
		{
			if (isThumbnail)
			{
				CRegardsBitmap * picture = nullptr;
				Array2D<Rgba> pixels;
				RgbaInputFile file(CConvertUtility::ConvertToUTF8(fileName));
				Imath::Box2i dw = file.dataWindow();
				int width = dw.max.x - dw.min.x + 1;
				int height = dw.max.y - dw.min.y + 1;
				pixels.resizeErase(height, width);
				file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
				file.readPixels(dw.min.y, dw.max.y);

				if (width > 0 && height > 0)
				{
					picture = new CRegardsBitmap(width, height);
					int k = 0;
					uint8_t * data = picture->GetPtBitmap();
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
					picture->VertFlipBuf();
					bitmap->SetPicture(picture);
					bitmap->SetFilename(fileName);
				}
			}
			else
			{
				CRegardsFloatBitmap * picture = nullptr;
				Array2D<Rgba> pixels;
				RgbaInputFile file(CConvertUtility::ConvertToUTF8(fileName));
				Imath::Box2i dw = file.dataWindow();
				int width = dw.max.x - dw.min.x + 1;
				int height = dw.max.y - dw.min.y + 1;
				pixels.resizeErase(height, width);
				file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
				file.readPixels(dw.min.y, dw.max.y);

				if (width > 0 && height > 0)
				{
					picture = new CRegardsFloatBitmap(width, height);
					int k = 0;
					float * data = picture->GetData();
					for (int i = height - 1; i >= 0; i--)
					{
						for (int j = 0; j < width; j++, k += 4)
						{
							data[k] = clamp(float(pixels[i][j].r), 0.0f, 1.0f);
							data[k + 1] = clamp(float(pixels[i][j].g), 0.0f, 1.0f);
							data[k + 2] = clamp(float(pixels[i][j].b), 0.0f, 1.0f);
							data[k + 3] = clamp(float(pixels[i][j].a), 0.0f, 1.0f);
						}
					}
					bitmap->SetPicture(picture);
					bitmap->SetFilename(fileName);
				}
			}
		}
		break;*/

		case WEBP:
		{
			size_t data_size;
			uint8_t* _compressedImage = readfile(fileName, data_size);
			if (_compressedImage != nullptr && data_size > 0)
			{
				CRegardsBitmap * picture = new CRegardsBitmap();
				int width = 0, height = 0;
				uint8_t * data = WebPDecodeBGRA(_compressedImage, data_size, &width, &height);
				picture->SetBitmap(data, width, height, true, false);
				bitmap->SetPicture(picture);
				bitmap->SetFilename(fileName);
				delete[] _compressedImage;
			}

		}
		break;
            
        case JPEG:
			{
				/*
				
				wxImage * image = new wxImage();
				image->LoadFile(fileName, wxBITMAP_TYPE_JPEG);
				bitmap->SetPicture(image);
				break;

				*/
                
                printf("CLibPicture LoadPicture JPEG \n");

				//CxImage * _cxImage = nullptr;
				

				if(isThumbnail)
				{
#ifdef TURBOJPEG
					tjscalingfactor *scalingfactors=NULL, sf={1, 1};  int nsf=0;
					size_t _jpegSize;
					uint8_t* _compressedImage = readfile(fileName, _jpegSize);

					//unsigned char buffer[width*height*COLOR_COMPONENTS]; //!< will contain the decompressed image
					//Getting the size
					if (_compressedImage != nullptr && _jpegSize > 0)
					{
						//int COLOR_COMPONENTS = 4;
						//unsigned char* _compressedImage; //!< _compressedImage from above
						CRegardsBitmap * picture;
						int jpegSubsamp, width = 0, height = 0;
					

						tjhandle _jpegDecompressor = tjInitDecompress();
						 
						
						tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);

						
						if((scalingfactors=tjGetScalingFactors(&nsf))==NULL || nsf==0)
							throw("executing tjGetScalingFactors()");
						
						//int num = 1;
						//int denom = 4;
						//int defaultscaling = 0;
						int match = 0;

						#ifdef WIN32

							HDC screen = GetDC(0);
							RECT rcClip;
							GetClipBox(screen, &rcClip);
							ReleaseDC(0, screen);

							int widthThumbnail = max((int32_t)(rcClip.right / 4), (int32_t)200);
							int heightThumbnail = max((int32_t)(rcClip.bottom / 4), (int32_t)200);

						#else
							int widthThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, 200);
							int heightThumbnail = max(wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 4, 200);
						#endif

						float ratio = CalculPictureRatio(width, height, widthThumbnail, heightThumbnail);

						for(int j=0; j<nsf; j++)
						{
							sf=scalingfactors[j];
							float localRatio = (float)sf.num / (float)sf.denom;
							if(localRatio < ratio)
							{
								if(j > 0)
									sf=scalingfactors[j - 1];

								match=1; 
								break;
							}
						}

						if(match == 0)
						{
							sf=scalingfactors[nsf - 1];
							match = 1;
						}
						
						if(match==1)
						{
							width = TJSCALED(width, sf);
							height = TJSCALED(height, sf);
						}
						
						picture = new CRegardsBitmap(width, height);

						tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, picture->GetPtBitmap(), picture->GetBitmapWidth(), 0, picture->GetBitmapHeight(), TJPF_BGRX, TJFLAG_FASTDCT | TJFLAG_BOTTOMUP);

						tjDestroy(_jpegDecompressor);
					
						bitmap->SetPicture(picture);
						bitmap->SetFilename(fileName);
					
						delete[] _compressedImage;
					}
#else

					_cxImage =  new CxImage();
					_cxImage->SetJpegScale(4);
					_cxImage->Load(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jpg"));
					bitmap->SetPicture(_cxImage);
#endif
					
				}
				else
				{

#ifdef TURBOJPEG

                    printf("CLibPicture LoadPicture TURBOJPEG \n");
					//unsigned char buffer[width*height*COLOR_COMPONENTS]; //!< will contain the decompressed image
					//Getting the size
					FILE *file = NULL;
                    

					if ((file = fopen(CConvertUtility::ConvertToUTF8(fileName), "rb")) == NULL)
						cout << "File Failed To Load\n";
					else
					{
						//int COLOR_COMPONENTS = 4;
						long unsigned int _jpegSize; //!< _jpegSize from above
						//unsigned char* _compressedImage; //!< _compressedImage from above
						CRegardsBitmap * picture;
						int jpegSubsamp, width, height;

						cout << "File Loaded Successfully\n";
						long prev=ftell(file);
						fseek(file, 0L, SEEK_END);
						_jpegSize =ftell(file);
						fseek(file,prev,SEEK_SET);
						

						//Creating a buffer and saving it back
						uint8_t * _compressedImage = new uint8_t[_jpegSize];
						//cout << "fileSize" << fileSize;
						fread(_compressedImage, _jpegSize, 1, file);
						fclose(file);

						tjhandle _jpegDecompressor = tjInitDecompress();
						 

						tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);

						picture = new CRegardsBitmap(width, height);

						tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, picture->GetPtBitmap(), width, 0, height, TJPF_BGRX, TJFLAG_FASTDCT | TJFLAG_BOTTOMUP);

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
						_cxImage =  new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jpg"));
						bitmap->SetPicture(_cxImage);
#endif
					}
			
			}
			break;

        case JBIG:
        {
   			CxImage * _cxImage = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jbg"));
			bitmap->SetPicture(_cxImage);         
        }
        break;

		case BMP:
			{
			CxImage * _cxImage = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("bmp"));
			bitmap->SetPicture(_cxImage);
			}
			break;

		case PDF:
		{
			try
			{
				bool value = false;
				wxPoppler poppler;
				value = poppler.Open(fileName);
				if (value)
					value = poppler.SelectPage(numPicture);

				if (value)
					value = poppler.RenderPage();

				if (value)
				{
					wxImage * image = new wxImage(poppler.GetImage());
					bitmap->SetPicture(image);
				}
			}
			catch (...)
			{
			}

		}
		break;

           
		
		case TIFF:
        case ANI:
			{
				wxImage * image = new wxImage();
				wxBitmapType bitmapType = wxBITMAP_TYPE_ANY;
				image->LoadFile(fileName, bitmapType, numPicture);
				bitmap->SetPicture(image);
			}
			break;        
               
        case PNG: 
        case GIF:
			{
                wxFileName fichier(fileName);
                wxString extension = fichier.GetExt();                  
                
				CxImage * _cxImage = new CxImage();
				_cxImage->SetRetreiveAllFrames(true);
				_cxImage->Load(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName(extension.c_str()));
                if (_cxImage->GetNumFrames() > 1)
                {
                    CxImage * frame = _cxImage->GetFrame(numPicture);
                    bitmap->SetPicture(frame);                    
                }
                else
                    bitmap->SetPicture(_cxImage);
			}
     		break;
            
        case TGA:
			{
            CxImage * _cxImage =  new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("tga"));
			bitmap->SetPicture(_cxImage);
			}
            break;


		case PNM:
			{
			CxImage * _cxImage =  new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("ppm"));
			bitmap->SetPicture(_cxImage);
			}
			break;

		case PCX:
			{
			CxImage * _cxImage =  new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("pcx"));
			bitmap->SetPicture(_cxImage);
			}
			break;

		case JPEG2000:
			{
			CxImage * _cxImage =  new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jp2"));
			bitmap->SetPicture(_cxImage);
			}
			break;

		case PGX:
			{
			CxImage * _cxImage = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("pgx"));
			bitmap->SetPicture(_cxImage);
			}
			break;

        case JPC:
			{
				CxImage * _cxImage = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jpc"));
				bitmap->SetPicture(_cxImage);
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
				int orientation = 0;
				CThumbnailVideo video(fileName);
				int percent = ((float)numPicture / (float)20) * 100.0f;
				bitmap->SetPicture(video.GetVideoFrame(0, 0, orientation, percent));
				bitmap->SetOrientation(orientation);
				bitmap->SetFilename(fileName);
			}
			break;
            
        default:
            CRegardsBitmap * _bitmap = LoadFromFreeImage(CConvertUtility::ConvertToUTF8(fileName));
            if (_bitmap != nullptr)
            {
                _bitmap->SetFilename(fileName);
                bitmap->SetPicture(_bitmap);
            }
            break;
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

#if defined(WIN32) && defined(_DEBUG)
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"Convert\n");
#endif

		int orientation = -1;
		if(TestIsExifCompatible(fileName) && applyExif)
		{
			CMetadataExiv2 metadata(fileName);
			orientation = metadata.GetOrientation();
			bitmap->SetOrientation(orientation);

		}
		
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
		return nullptr;
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
		delete bitmap;
		bitmap = LoadPicture(CLibResource::GetPhotoCancel());
		bitmap->SetFilename(fileName);
	}

	return bitmap;
}

wxImage CLibPicture::ConvertRegardsBitmapToWXImage(CRegardsBitmap * bitmap, const bool &flip, const bool &loadAlpha)
{
	int width = bitmap->GetBitmapWidth();
	int height = bitmap->GetBitmapHeight();
	int widthSrcSize = width * 4;
	unsigned char * data = bitmap->GetPtBitmap();
	int posData = 0;
	int posDataOut = 0;
	int posAlpha = 0;
	wxImage anImage(width, height, true);
	if (loadAlpha)
		anImage.InitAlpha();

	unsigned char * dataOut = anImage.GetData();
	unsigned char * dataAlpha = anImage.GetAlpha();

	if (data != nullptr)
	{
		for (auto y = 0; y < height; y++)
		{
			if(flip)
				posData = y * widthSrcSize;
			else
				posData = ((height - y) * widthSrcSize) - widthSrcSize;
			posDataOut = y * (width * 3);
			posAlpha = y * width;
			for (auto x = 0; x < width; x++)
			{
				dataOut[posDataOut] = data[posData + 2];
				dataOut[posDataOut + 1] = data[posData + 1];
				dataOut[posDataOut + 2] = data[posData];

				if (loadAlpha)
					dataAlpha[posAlpha++] = data[posData + 3];
				posData += 4;
				posDataOut += 3;
			}
		}
	}

	return anImage;
}

wxImage * CLibPicture::ConvertRegardsBitmapToWXImage(CRegardsBitmap * bitmap, const bool &loadAlpha)
{
	int width = bitmap->GetBitmapWidth();
	int height = bitmap->GetBitmapHeight();
	int widthSrcSize = width * 4;
	unsigned char * data = bitmap->GetPtBitmap();
	int posData = 0;
	int posDataOut = 0;
	int posAlpha = 0;
	wxImage * anImage = new wxImage(width, height, true);
	if (loadAlpha)
		anImage->InitAlpha();

	unsigned char * dataOut = anImage->GetData();
	unsigned char * dataAlpha = anImage->GetAlpha();

	if (data != nullptr)
	{
		for (auto y = 0; y < height; y++)
		{
			posData = ((height - y) * widthSrcSize) - widthSrcSize;
			posDataOut = y * (width * 3);
			posAlpha = y * width;
			for (auto x = 0; x < width; x++)
			{
				dataOut[posDataOut] = data[posData + 2];
				dataOut[posDataOut + 1] = data[posData + 1];
				dataOut[posDataOut + 2] = data[posData];

				if (loadAlpha)
					dataAlpha[posAlpha++] = data[posData + 3];
				posData += 4;
				posDataOut += 3;
			}
		}
	}

	return anImage;
}

bool CLibPicture::HasThumbnail(const wxString &filename)
{

#if defined(EXIV2)
    int iFormat = TestImageFormat(filename);
    if(iFormat == RAWFILE)
        return true;
    else if(iFormat == TIFF || iFormat == JPEG)
    {
		CMetadataExiv2 pictureMetadata(filename);
		return pictureMetadata.HasThumbnail();
    }
#endif    
    return false;
}

void  CLibPicture::writefile(const wxString & fileName, uint8_t * data, size_t & size)
{
    //const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	FILE *file = NULL;
	if ((file = fopen(CConvertUtility::ConvertToUTF8(fileName), "wb")) == NULL)
		cout << "File Failed To Load\n";
	else
	{
		fwrite(data, size, 1, file);
		fclose(file);
	}
}

uint8_t * CLibPicture::readfile(const wxString & fileName, size_t &_fileSize)
{
    _fileSize = 0;
    //const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	uint8_t * _compressedImage = nullptr;
	FILE *file = NULL;
	if ((file = fopen(CConvertUtility::ConvertToUTF8(fileName), "rb")) == NULL)
		cout << "File Failed To Load\n";
	else
	{
		long prev=ftell(file);
		fseek(file, 0L, SEEK_END);
		_fileSize =ftell(file);
		fseek(file,prev,SEEK_SET);
						
		//Creating a buffer and saving it back
		_compressedImage = new uint8_t[_fileSize];
		//cout << "fileSize" << fileSize;
		fread(_compressedImage, _fileSize, 1, file);
		fclose(file);
	}
	return _compressedImage;
}

//--------------------------------------------------------------------------------------------------
//Fonction d'appel pour l'obtention des dimensions d'une image
//--------------------------------------------------------------------------------------------------
int CLibPicture::GetPictureDimensions(const wxString & fileName, int & width, int & height, int & rotation)
{
	int iFormat = TestImageFormat(fileName);
	width = 0;
	height = 0;
	CxImage * image = nullptr;
    wxImage imageWx;
    int typeImage = TYPE_IMAGE_CXIMAGE;
    //const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	switch (iFormat)
	{
	case PFM:
	{
		CPfm::GetDimensions(fileName, width, height);
	}
	break;

    case EXR:
    case HDR:
        {
			cv::Mat hdr = cv::imread(fileName.ToStdString(), -1); // correct element size should be CV_32FC3
			width = hdr.cols;
			height = hdr.rows;
        }
        break;        

	case WEBP:
	{
		typeImage = TYPE_IMAGE_REGARDSIMAGE;
		int result = 0;
		size_t data_size;
		uint8_t* data = readfile(fileName, data_size);
		if (data != nullptr && data_size > 0)
		{
			result = WebPGetInfo(data, data_size, &width, &height);
			delete[] data;
		}
		break;
	}

            
	case JPEG:
#ifdef TURBOJPEG
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			size_t _jpegSize;
			uint8_t* _compressedImage = readfile(fileName, _jpegSize);
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
		
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jpg"), true);
#endif
		
		break;

	case BMP:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("bmp"), true);
		break;
        
    case JBIG:
   		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jbg"), true);
		break; 

	case TIFF:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("tif"), true);
		break;
            
    case PDF:
        {
			try
			{
				wxPoppler poppler;
				bool isValid = poppler.Open(fileName);
				if (isValid)
					isValid = poppler.SetDpi(300);
				if (isValid)
					isValid = poppler.SelectPage(0);
				if (isValid)
					isValid = poppler.RenderPage();
				if (isValid)
				{
					imageWx = poppler.GetImage();
					typeImage = TYPE_IMAGE_WXIMAGE;
				}
			}
			catch (...)
			{
			}

        }

        break;            
            
    case IFF:
        imageWx.LoadFile(fileName, wxBITMAP_TYPE_IFF); typeImage = TYPE_IMAGE_WXIMAGE;
        break;
            
    case XPM:
        imageWx.LoadFile(fileName, wxBITMAP_TYPE_XPM); typeImage = TYPE_IMAGE_WXIMAGE;
        break;

	case RAWFILE:
		typeImage = TYPE_IMAGE_REGARDSIMAGE;
		CRaw::GetDimensions(fileName, width, height);
		break;


	case GIF:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("gif"), true);
		break;

	case PNM:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("pnm"));
		break;

	case PCX:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("pcx"), true);
		break;

	case PNG:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("png"), true);
		break;

	case TGA:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("tga"), true);
		break;

	case JPEG2000:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("jp2"));
		break;

	case PPM:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("ppm"));
		break;


	case PCD:
	case MNG:
	case PSD:
		//image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("psd"), true);
		PictureDimensionFreeImage(CConvertUtility::ConvertToUTF8(fileName), width, height);
		break;
	case PGX:
		image = new CxImage(CConvertUtility::ConvertToUTF8(fileName), CxImage::GetTypeIdFromName("pgx"), true);
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
			uint8_t* _compressedImage = readfile(fileName, data_size);
			if(_compressedImage != nullptr && data_size > 0)
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
			CThumbnailVideo video(fileName);
			video.GetVideoDimensions(width, height, rotation);
		}
		break;
#ifdef LIBHEIC
	case HEIC:
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			CHeic::GetPictureDimension(CConvertUtility::ConvertToUTF8(fileName), width, height);
			//video.GetVideoDimensions(fileName, width, height, rotation);
		}
		break;
	case AVIF:
	{
		typeImage = TYPE_IMAGE_REGARDSIMAGE;
		CAvif::GetPictureDimension(CConvertUtility::ConvertToUTF8(fileName), width, height);
		//video.GetVideoDimensions(fileName, width, height, rotation);
	}
	break;
#endif

    default:
		PictureDimensionFreeImage(CConvertUtility::ConvertToUTF8(fileName), width, height);
		break;
	}

	 

    if(typeImage == TYPE_IMAGE_CXIMAGE)
    {
        if (image != nullptr)
        {
            width = image->GetWidth();
            height = image->GetHeight();
            EXIFINFO * exif = image->GetExifInfo();
            if (exif != nullptr)
                rotation = exif->Orientation;
            else
                rotation = 0;
            delete image;
        }
    }
    else if(typeImage == TYPE_IMAGE_WXIMAGE)
    {
        if(imageWx.IsOk())
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
    FreeImage_SetOutputMessage(&MyMessageFunction);
}

void CLibPicture::UninitFreeImage()
{
    FreeImage_DeInitialise();
}


CPictureData * CLibPicture::LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth, const int &resizeHeight)
{
	CPictureData * pictureData = nullptr;
	CLibPicture libPicture;
	CImageLoadingFormat * imageLoading = libPicture.LoadPicture(filename);

	if (imageLoading != nullptr)
	{
		pictureOK = imageLoading->IsOk();
		if (pictureOK)
		{
			imageLoading->ApplyExifOrientation(1);
			imageLoading->ConvertToRGB24(true);
			//Calcul Resize Size
			pictureData = new CPictureData();
			if (resizeWidth != 0 && resizeHeight != 0)
			{
				float ratio = CalculPictureRatio(imageLoading->GetWidth(), imageLoading->GetHeight(), resizeWidth, resizeHeight);
				pictureData->SetWidth(imageLoading->GetWidth() * ratio);
				pictureData->SetHeight(imageLoading->GetHeight() * ratio);
				imageLoading->Resize(pictureData->GetWidth(), pictureData->GetHeight(), 1);
			}
			else
			{
				pictureData->SetWidth(imageLoading->GetWidth());
				pictureData->SetHeight(imageLoading->GetHeight());
			}

			unsigned long outputsize = 0;
			int compressMethod = 0;
			uint8_t * data = imageLoading->GetJpegData(outputsize, compressMethod);
			pictureData->SetJpegData(data, outputsize);
			imageLoading->DestroyJpegData(data, compressMethod);
		}
	}

	if (imageLoading != nullptr)
		delete imageLoading;

	imageLoading = nullptr;

	return pictureData;
}

CPictureData * CLibPicture::LoadPictureData(const wxString &filename, bool &pictureOK)
{
	CPictureData * pictureData = nullptr;
	pictureData = LoadPictureToJpeg(filename, pictureOK);
	return pictureData;
}

bool CLibPicture::SaveToPDF(wxImage * poImage, const wxString &pdfFile, int option, int quality)
{
	int _option = option;
	int _quality = quality;

	if(_option == -1)
		SavePictureOption(PDF, _option, _quality);

	wxString fileToSave;
	if (option == 0)
		fileToSave = CFileUtility::GetTempFile("scanner.jpg");
	else
		fileToSave = CFileUtility::GetTempFile("scanner.tif");

	if (wxFileExists(fileToSave))
		wxRemoveFile(fileToSave);

	if (option == 0)
		poImage->SaveFile(fileToSave, wxBITMAP_TYPE_JPEG);
	else
		poImage->SaveFile(fileToSave, wxBITMAP_TYPE_TIFF);

	SaveToPDF(poImage, pdfFile, fileToSave, option, _quality);

	return true;
}
