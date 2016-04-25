// DllPicture.cpp : définit les fonctions exportées pour l'application DLL.
//

#include "libPicture.h"
#include <RegardsBitmap.h>
#include <videothumbnail.h>
#include <wx/mstream.h>
#include "CPCD.h"
#include <ImageVideoThumbnail.h>
#ifdef LIBBPG
#ifndef WIN32
    #include <dlfcn.h>
#endif
#endif
#ifdef LIBRAW
#include <raw.h>
#endif

#ifdef __APPLE__
#include "AppleReadPicture.h"
#endif

#include <ximage.h>
#include <xfile.h>
#include <xiofile.h>
#include "ScaleThumbnail.h"

//#include <sstream>
#include <string>


#ifdef WIN32
#include <video.h>
#include <ImageDecoder.h>
#include <PictureMetadata.h>
#endif

#ifdef FFMPEG 
#include <videothumb.h>
using namespace Regards::Video;
#endif

#ifdef EXIV2
#include <PictureMetadata.h>
#endif

#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/image.h>
#ifdef LIBPDF
	#include <DllTiff2Pdf.h>
#endif
#include <FileUtility.h>
#include <cctype>
#include <string>
//#include <algorithm>
//Dialog Save
#include <GifOption.h>
#include <JpegOption.h>
#include <TiffOption.h>
#include <PngOption.h>
#include <CompressionOption.h>
#include <SaveFileFormat.h>
#include <wxSVG/SVGDocument.h>

#define TYPE_IMAGE_CXIMAGE 0
#define TYPE_IMAGE_WXIMAGE 1
#define TYPE_IMAGE_REGARDSIMAGE 2
#define OR ||

#ifdef LIBBPG


#ifndef WIN32
CBpg * CLibPicture::libbpg = nullptr;
void * CLibPicture::lib_handle = nullptr;
#endif
#endif

CLibPicture::CLibPicture()
{
#ifdef LIBBPG
#ifndef WIN32
    
    if(libbpg == nullptr)
    {
        wxString path = CFileUtility::GetProgramFolderPath();
#ifdef __APPLE__		
        path.Append("/liblibRegardsBpg.dylib");
#else
        path.Append("/RegardsBpg.so");
#endif
        lib_handle = dlopen(path.ToStdString().c_str(), RTLD_LOCAL|RTLD_LAZY);
        if (!lib_handle) {
            printf("[%s] Unable to load library: %s\n", __FILE__, dlerror());
            exit(EXIT_FAILURE);
        }
        
        // Get the NewPerson function.
        Bpg_creator * NewBpg = (Bpg_creator*)dlsym(lib_handle, "NewBpg");
        if (!NewBpg) {
            cerr << "[" << __FILE__ << "] main: Unable to find NewPerson method: "
            << dlerror() << "\n";
            exit(EXIT_FAILURE);
        }
        

        
        libbpg = (CBpg*)NewBpg();
    }
#endif
#endif
}

CLibPicture::~CLibPicture()
{
    /*
    // Get the DeletePerson function.
    Bpg_disposer* DeleteBpg =(Bpg_disposer*)dlsym(lib_handle, "DeleteBpg");
    if (!DeleteBpg) {
        cerr << "[" << __FILE__
        << "] main: Unable to find DeletePerson method: "
        << dlerror() << "\n";
        exit(EXIT_FAILURE);
    }
    
    // Destroy Person object.
    DeleteBpg(libbpg);
    
    // Close the library.
    if (dlclose(lib_handle) != 0) {
        cerr << "[" << __FILE__ << "] main: Unable to close library: "
        << dlerror() << "\n";
    }
    */
}

int CLibPicture::TestExtension(const wxString & ext)
{
	if (ext == L"pgm" ||
		ext == L"pnm")
		return PNM;

	if (ext == L"bmp")
		return BMP;

	if (ext == L"pcx")
		return PCX;

	if (ext == L"jpg" ||
		ext == L"jpe" ||
		ext == L"jpeg" ||
		ext == L"jfif" ||
		ext == L"jif" ||
		ext == L"jfi")
		return JPEG;

	if (ext == L"tif" ||
		ext == L"tiff")
		return TIFF;

	if (ext == L"gif")
		return GIF;

	if (ext == L"png")
		return PNG;

	if (ext == L"tga")
		return TGA;

	if (ext == L"pcd")
		return PCD;

#ifdef LIBBPG	
	if (ext == L"bpg")
		return BPG;
#endif

	if (ext == L"jp2" ||
		ext == L"j2k")
		return JPEG2000;

	if (ext == L"jpc" ||
		ext == L"j2c")
		return JPC;

	if (ext == L"pgx")
		return PGX;
#ifdef LIBPDF
	if (ext == L"pdf")
		return PDF;
#endif
	if (ext == L"ppm")
		return PPM;

	if (ext == L"psd" ||
		ext == L"pdd")
		return PSD;

	if (ext == L"mng" ||
		ext == L"jng")
		return MNG;

	if (ext == L"iff")
		return IFF;

	if (ext == L"xpm")
		return XPM;

	if (ext == L"ico")
		return ICO;

	if (ext == L"cur")
		return CUR;

	if (ext == L"ani")
		return ANI;

    if (ext == L"svg")
        return SVG;
    
#if defined(LIBRAW) || defined(WIN32) || defined(__APPLE__)
	//Test RAW File
	if (ext == L"nef" ||
		ext == L"crw" ||
		ext == L"cr2" ||
		ext == L"dng" ||
		ext == L"arw" ||
		ext == L"erf" ||
		ext == L"3fr" ||
		ext == L"dcr" ||
		ext == L"raw" ||
		ext == L"x3f" ||
		ext == L"mef" ||
		ext == L"raf" ||
		ext == L"mrw" ||
		ext == L"pef" ||
		ext == L"sr2" ||
		ext == L"orf")
		return RAW;
#endif

#if defined(WIN32) || defined(__APPLE__)

	if (ext == L"mp4")
		return MP4;

	if (ext == L"mpg")
		return MPG2;

	if (ext == L"mpeg")
		return MPG2;

	if (ext == L"avi")
		return AVI;

	if (ext == L"wmv")
		return WMV;

	if (ext == L"mov")
		return MOV;

	if (ext == L"qt")
		return MOV;

#else

	if (ext == L"mp4")
		return MP4;

	if (ext == L"dat")
		return MPEG;

	if (ext == L"m4s")
		return MP4;

	if (ext == L"vob")
		return MPG2;

	if (ext == L"mod")
		return MPG2;

	if (ext == L"mpv2")
		return MPG2;

	if (ext == L"mp2")
		return MPG2;

	if (ext == L"m1v")
		return MPG2;

	if (ext == L"mpe")
		return MPG2;

	if (ext == L"mpg")
		return MPG2;

	if (ext == L"mpeg")
		return MPG2;

	if (ext == L"wtv")
		return WINREC;

	if (ext == L"dvr-ms")
		return WINREC;

	if (ext == L"m2ts")
		return AVCHD;

	if (ext == L"m2t")
		return AVCHD;

	if (ext == L"avi")
		return AVI;

	if (ext == L"wmv")
		return WMV;

	if (ext == L"asf")
		return WMV;

	if (ext == L"vm")
		return WMV;

	if (ext == L"mov")
		return MOV;

	if (ext == L"qt")
		return MOV;

#endif
	return 0;
}

bool CLibPicture::TestIsPicture(const wxString & szFileName)
{
    int numExt = 0;
    wxFileName fichier(szFileName.c_str());
    wxString extension = fichier.GetExt();
    
    numExt = TestExtension(extension.Lower());
    if (numExt < 100 && numExt != ANI && numExt != 0)
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
    int numExt = 0;
    wxFileName fichier(szFileName.c_str());
    wxString extension = fichier.GetExt();
    
    numExt = TestExtension(extension.Lower());
    if(numExt == GIF OR numExt == ANI)
    {
        if(numExt == GIF)
        {
            CxImage * image = new CxImage(szFileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("gif"));
            if(image->GetNumFrames() > 1)
                returnValue = true;
            delete image;
        }
        return returnValue;
    }
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


		return TestExtension(extension.Lower());
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
	bitmap->RotateExif(orientation);
	return bitmap;
}

//-----------------------------------------------------------------------------
//Conversion du format CXImage vers RegardsBitmap
//-----------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::ConvertCXImageToRegardsBitmap(CxImage * image)
{

#if _DEBUG
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"ConvertCXImageToRegardsBitmap\n");
#endif


	CRegardsBitmap * bitmap = nullptr;
	try
	{
		//float power_of_two_that_gives_correct_width = std::log((float)(image->GetWidth())) / std::log(2.0);
		//float power_of_two_that_gives_correct_height = std::log((float)(image->GetHeight())) / std::log(2.0);

		//int newWidth = (int)std::pow(2.0, (int)(std::ceil(power_of_two_that_gives_correct_width)));
		//int newHeight = (int)std::pow(2.0, (int)(std::ceil(power_of_two_that_gives_correct_height)));
		bitmap = new CRegardsBitmap(image->GetWidth(), image->GetHeight());
		image->Encode2BGRA(bitmap->GetPtBitmap(), bitmap->GetBitmapSize(), false);


		EXIFINFO * exif = image->GetExifInfo();
		if (exif != nullptr)
		{
			bitmap->RotateExif(exif->Orientation);
		}
	}
	catch (...)
	{

	}

#if _DEBUG
	DWORD LasttickCount = GetTickCount();				// Get The Tick Count
	DWORD Result = LasttickCount - tickCount;

	wchar_t Temp[10];
	swprintf_s(Temp, L"%d", Result);
	OutputDebugString(L"Render Time : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");
#endif


	return bitmap;
}

//-----------------------------------------------------------------------------
//Conversion du format CXImage vers RegardsBitmap
//-----------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::ConvertCXImageToScaleRegardsBitmapCRgbaquad(CxImage * image, const int &width, const int &height)
{
#if _DEBUG
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

#if _DEBUG
	DWORD LasttickCount = GetTickCount();				// Get The Tick Count
	DWORD Result = LasttickCount - tickCount;

	wchar_t Temp[10];
	swprintf_s(Temp, L"%d", Result);
	OutputDebugString(L"Render Time : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");
#endif

	return bitmapOut;
}

//-----------------------------------------------------------------------------
//Conversion du format CXImage vers RegardsBitmap
//-----------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::ConvertCXImageToScaleRegardsBitmapBGR(CxImage * image, const int &width, const int &height)
{
#if _DEBUG
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

#if _DEBUG
	DWORD LasttickCount = GetTickCount();				// Get The Tick Count
	DWORD Result = LasttickCount - tickCount;

	wchar_t Temp[10];
	swprintf_s(Temp, L"%d", Result);
	OutputDebugString(L"Render Time : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");
#endif

	return bitmapOut;

}

CxImage * CLibPicture::ConvertRegardsBitmapToCXImage(CRegardsBitmap * bitmap)
{
	CxImage * image = nullptr;
	if (bitmap != nullptr)
	{
		image = new CxImage();
		image->CreateFromArray(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetBitmapDepth(), bitmap->GetWidthSize(), false);
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
		returnValue = 1;
	}
	break;

	case TGA:
	{
		returnValue = 1;
	}
	break;

	case PCX:
	{
		returnValue = 1;
	}
	break;

	case MNG:
	{
		returnValue = 1;
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

#ifdef LIBBPG
	case BPG:
	{
		int lossless_mode = 0;
		int compress_level = 8;
		returnValue = 1;
	}
	break;
#endif



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

	case PNM:
	{
		returnValue = 1;
	}
	break;

	case JPC:
	{
		returnValue = 1;
	}
	break;

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

	case PPM:
	{
		returnValue = 1;
	}
	break;

	case XPM:
	{
		returnValue = 1;
	}
	break;


	}

	return returnValue;
}

int CLibPicture::SavePicture(const  wxString & fileName, CRegardsBitmap * bitmap, const int &option, const int &quality)
{
	int iFormat = 0;


	iFormat = TestImageFormat(fileName);

	switch (iFormat)
	{
	case BMP:
	{
		//iReturn = CBmp::SaveBMP(bitmap, fileName);
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("bmp"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case TGA:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("tga"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case PCX:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("pcx"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case MNG:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("mng"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case TIFF:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->SetCodecOption(option, CXIMAGE_FORMAT_TIF);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("tif"));

		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case PNG:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->SetCodecOption(option, CXIMAGE_FORMAT_PNG);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("png"));
		delete image;
	}
	break;

	case GIF:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->SetCodecOption(option, CXIMAGE_FORMAT_GIF);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("gif"));

		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;
	}
	break;
#ifdef LIBBPG
	case BPG:
	{
		int width = bitmap->GetBitmapWidth();
		int height = bitmap->GetBitmapHeight();
		int lossless_mode = 0;
		int compress_level = 8;

		bitmap->VertFlipBuf();
#ifdef WIN32
		int result = WritePictureBpg(fileName.ToStdString().c_str(), bitmap->GetPtBitmap(), width, height, lossless_mode, compress_level);
#else
		int result = libbpg->WritePictureBpg(fileName.ToStdString().c_str(), bitmap->GetPtBitmap(), width, height, lossless_mode, compress_level);
#endif
		bitmap->VertFlipBuf();


	}
	break;
#endif


	case JPEG:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->SetCodecOption(option, CXIMAGE_FORMAT_JPG);
		image->SetJpegQualityF((float)quality);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jpg"));

		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case PNM:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("pnm"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case JPC:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jpc"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;
	}
	break;

	case JPEG2000:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->SetJpegQualityF((float)quality);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jp2"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;

	}
	break;

	case PPM:
	{
		CxImage * image = ConvertRegardsBitmapToCXImage(bitmap);
		image->Save(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("ppm"));
		wxString error = image->GetLastError();
		if (error != "")
			wxMessageBox(error,
			"Error Information", wxOK | wxICON_ERROR);
		delete image;

	}
	break;

	case XPM:
	{
		wxImage * image = ConvertRegardsBitmapToWXImage(bitmap);
		image->SaveFile(fileName, wxBITMAP_TYPE_XPM);
		delete image;
	}
	break;
	}

#if defined(WIN32) || defined(EXIV2)

	CPictureMetadata pictureMetadata(bitmap->GetFilename());
	pictureMetadata.CopyMetadata(fileName);

#elif __APPLE__

	if ((iFormat == JPEG || iFormat == PNG || iFormat == TIFF))
	{
		if (wxFileName::FileExists(fileName))
		{
			//Copy Metadata
			CAppleReadPicture readPicture;
			readPicture.CopyMetadata(bitmap->GetFilename(), fileName);
		}
		else
			wxMessageBox("Impossible to create this file !",
			"Error Information", wxOK | wxICON_ERROR);
	}

	if (!wxFileName::FileExists(fileName))
		wxMessageBox("Impossible to create this file !",
		"Error Information", wxOK | wxICON_ERROR);

#endif

	return 0;
}

//-----------------------------------------------------------------------------
//Sauvegarde
//-----------------------------------------------------------------------------
int CLibPicture::SavePicture(const wxString & fileName, CRegardsBitmap * bitmap)
{
    wxString wxfileName;
    int iFormat = 0;

    iFormat = TestImageFormat(fileName);
    wxfileName = fileName;
 
	int option = 0;
	int quality = 0;

	if (SavePictureOption(iFormat, option, quality) == 1)
	{
		SavePicture(wxfileName, bitmap, option, quality);
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
//Ouverture d'un fichier avec Scaling
//--------------------------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::LoadThumbnailPicture(const wxString & fileName, const int &width, const int &height)
{
    wxImage imageWx;
	CRegardsBitmap * bitmap = nullptr;
	int iFormat = TestImageFormat(fileName);
	bool value = true;

	//float ratio = 1.0f;
	int RGBFormat = 0;
	int rotation = 0;
    
    int typeImage = TYPE_IMAGE_CXIMAGE;

	try
	{
		CxImage * image = nullptr;
		switch (iFormat)
		{
		case JPEG:
			image = new CxImage();	
			image->SetJpegScale(4);
			image->Load(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jpg"));
			break;

		case PPM:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("ppm"));
			break;

		case BMP:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("bmp"));
			break;

		case TIFF:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("tif"));
			break;

        case IFF:
            imageWx.LoadFile(fileName, wxBITMAP_TYPE_IFF); typeImage = TYPE_IMAGE_WXIMAGE;
            break;
                
        case ANI:
            imageWx.LoadFile(fileName, wxBITMAP_TYPE_ANI); typeImage = TYPE_IMAGE_WXIMAGE;
            break;
            
        case XPM:
            imageWx.LoadFile(fileName, wxBITMAP_TYPE_XPM); typeImage = TYPE_IMAGE_WXIMAGE;
            break;
                
            case ICO:
                imageWx.LoadFile(fileName, wxBITMAP_TYPE_ICON); typeImage = TYPE_IMAGE_WXIMAGE;
                break;
                
            case CUR:
                imageWx.LoadFile(fileName, wxBITMAP_TYPE_CUR); typeImage = TYPE_IMAGE_WXIMAGE;
                break;
                
        case SVG:
            {
                wxSVGDocument* svgDoc = new wxSVGDocument;
                svgDoc->Load(fileName);
                imageWx = svgDoc->Render(width,height);
                typeImage = TYPE_IMAGE_WXIMAGE;
                delete svgDoc;
            }
            break;

#ifdef WIN32

		case RAW:
		{
			CImageDecoder imageDecoder;
			bitmap = imageDecoder.DecodePicture(fileName);
			bitmap->VertFlipBuf();
		}
		break;

#endif

#ifdef LIBRAW
		case RAW:
			//image = new CxImage(file, CxImage::GetTypeIdFromName("raw"));
			bitmap = CRaw::GetPicture(fileName.ToStdString().c_str());
			break;
#endif			
#ifdef LIBBPG
		case BPG:
			{
                int size = 0;
                int width = 0;
                int height = 0;

#ifdef WIN32
				uint8_t * data = nullptr;
				int value = GetPictureBpg(fileName.c_str(), size, width, height, data, 0);
				if (value == 0)
				{
					int sizeData = size;
					data = new uint8_t[size];
					value = GetPictureBpg(fileName.c_str(), size, width, height, data, sizeData);
					bitmap = new CRegardsBitmap();
					bitmap->SetBitmap(data, width, height);
					delete[] data;
					data = nullptr;
				}
#else
				std::vector<uint8_t> myVector = libbpg->GetPngPicture(fileName.ToStdString().c_str(), size, width, height);
				CxMemFile memFile(&myVector[0], myVector.size());
				image = new CxImage(&memFile, CxImage::GetTypeIdFromName("png"));
				myVector.clear();
#endif
			}
			break;
#endif
                
		case GIF:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("gif"));
			break;

		case PNM:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("ppm"));
			break;

		case PCX:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("pcx"));
			break;

		case PNG:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("png"));
			break;

		case TGA:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("tga"));
			break;

		case JPEG2000:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jp2"));
			break;

		case PCD:
		{
			CCPCD * pPCD = new CCPCD();
			bitmap = pPCD->readPCD(fileName.ToStdString().c_str());
			CScaleThumbnail::CreateScaleBitmap(bitmap, 200, 200);
			delete pPCD;
		}
			break;

		case PSD:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("psd"));
			break;

		case PGX:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("pgx"));
			break;

		case MNG:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("mng"));
			break;

#ifdef WIN32
		case MPG2:
		case MPEG:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			CVideo video;
			bitmap = video.GetVideoFrame(fileName, rotation);
			break;
		}
#endif

#ifdef __APPLE__

		case RAW:
		{
			bitmap = new CRegardsBitmap();
			CAppleReadPicture readPicture;
			readPicture.ReadPicture(fileName, bitmap);
		}
		break;

		case MPG2:
		case MPEG:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			bitmap = new CRegardsBitmap();
			CAppleReadPicture readPicture;
			readPicture.GetVideoThumbnail(fileName, bitmap, width, height);
			break;
		}

#endif

#ifdef FFMPEG
		case MPG2:
		case MPEG:
		case AVCHD:
		case WINREC:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			CThumbnailVideo video;
			image = video.GetVideoFrame(fileName, rotation);
			RGBFormat = 1;
			break;
		}
#endif
		}

		if (bitmap == nullptr)
		{
            if (image != nullptr && typeImage == TYPE_IMAGE_CXIMAGE)
            {
                if (image != nullptr)
                {
                    wxString error = image->GetLastError();
                    if (error == "")
                    {
                        if (RGBFormat == 1)
                            bitmap = ConvertCXImageToScaleRegardsBitmapCRgbaquad(image, width, height);
                        else
                            bitmap = ConvertCXImageToScaleRegardsBitmapBGR(image, width, height);
                        
                        if (rotation == 90)
                        {
                            bitmap->Rotation90();
                            bitmap->VertFlipBuf();
                        }
                        else if (rotation == 270)
                        {
                            bitmap->Rotation90();
                        }
                    }
                    else
                    {
                        bitmap = new CRegardsBitmap(width, height);
                        bitmap->SetBackgroundColor(CRgbaquad(0, 0, 0));
                    }
                }
                else
                {
                    bitmap = new CRegardsBitmap(width, height);
                    bitmap->SetBackgroundColor(CRgbaquad(0, 0, 0));
                }
                delete image;
            }
            else if (typeImage == TYPE_IMAGE_WXIMAGE)
            {
                if (imageWx.IsOk())
                    bitmap = ConvertwxImageToRegardsBitmap(imageWx);
            }
		}

		if (iFormat != 0 && value && bitmap != nullptr)
		{
			bitmap->SetFilename(fileName);
		}
	}
	catch (...)
	{
		return nullptr;
	}


	return bitmap;
}

CRegardsBitmap * CLibPicture::ConvertwxImageToRegardsBitmap(const wxImage & image)
{
    CRegardsBitmap * bitmap = nullptr;
    if (image.IsOk())
    {
        wxMemoryOutputStream outputStream;
        image.SaveFile(outputStream, wxBITMAP_TYPE_PNG);
        uint8_t * data = new uint8_t[outputStream.GetSize()];
        outputStream.CopyTo(data, outputStream.GetSize());
        CxMemFile memFile(data, outputStream.GetSize());
        CxImage * image = new CxImage(&memFile, CxImage::GetTypeIdFromName("png"));
        wxString error = image->GetLastError();
        if (error == "")
            bitmap = ConvertCXImageToRegardsBitmap(image);
        
        delete[] data;
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
CRegardsBitmap * CLibPicture::LoadVideoThumbnail(const  wxString & szFileName, const int &percent, int &timePosition)
{
	CRegardsBitmap * bitmap = nullptr;
	int iFormat = TestImageFormat(szFileName);
	bool value = true;
	//float ratio = 1.0f;
	int RGBFormat = 0;
	int rotation = 0;
	const int &width = 200;
	const int &height = 200;

	try
	{
		CxImage * image = nullptr;
		switch (iFormat)
		{

#ifdef WIN32
		case MPG2:
		case MPEG:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			CVideo video;
			bitmap = video.GetVideoFrame(szFileName, rotation, percent, timePosition);
			break;
		}
#endif


#ifdef __APPLE__
		case MPG2:
		case MPEG:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			CAppleReadPicture readPicture;
			readPicture.GetVideoThumbnail(szFileName, bitmap, width, height);
			break;
		}
#endif

#ifdef FFMPEG
		case MPG2:
		case MPEG:
		case AVCHD:
		case WINREC:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			CThumbnailVideo video;
			image = video.GetVideoFrame(szFileName, rotation, percent, timePosition);
			RGBFormat = 1;
			break;
		}
#endif
		}

		if (bitmap == nullptr)
		{
			if (image != nullptr)
			{
				wxString error = image->GetLastError();
				if (error == "")
				{
					if (RGBFormat == 1)
						bitmap = ConvertCXImageToScaleRegardsBitmapCRgbaquad(image, width, height);
					else
						bitmap = ConvertCXImageToScaleRegardsBitmapBGR(image, width, height);

					if (rotation == 90)
					{
						bitmap->Rotation90();
						bitmap->VertFlipBuf();
					}
					else if (rotation == 270)
					{
						bitmap->Rotation90();
					}
				}
				else
				{
					bitmap = new CRegardsBitmap(width, height);
					bitmap->SetBackgroundColor(CRgbaquad(0, 0, 0));
				}
			}
			else
			{
				bitmap = new CRegardsBitmap(width, height);
				bitmap->SetBackgroundColor(CRgbaquad(0, 0, 0));
			}
			delete image;
		}

		if (iFormat != 0 && value && bitmap != nullptr)
		{
			bitmap->SetFilename(szFileName);
		}
	}
	catch (...)
	{
		return nullptr;
	}

	return bitmap;
}

//--------------------------------------------------------------------------------------------
//Obtention d'un thumbnail à partir des informations exif
//--------------------------------------------------------------------------------------------
CVideoThumbnail * CLibPicture::LoadAllVideoThumbnail(const  wxString & szFileName, int &width, int &height)
{
	int iFormat = TestImageFormat(szFileName);
	vector<CImageVideoThumbnail *> listThumbnail;
	CVideoThumbnail * videoThumbnail = new CVideoThumbnail();
	int widthOut = 200;
	int heightOut = 200;
    int rotation = 0;
	try
	{
#ifdef WIN32
		switch (iFormat)
		{
		case MPG2:
		case MPEG:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			CVideo video;
			listThumbnail = video.GetVideoFrame(szFileName, width, height);
			break;
		}
		}
#endif

#ifdef __APPLE__

		//CxImage * image = nullptr;
		switch (iFormat)
		{
		case MPG2:
		case MPEG:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			CAppleReadPicture readPicture;
			listThumbnail = readPicture.GetVideoFrame(szFileName, width, height);
			break;
		}
		}
#endif

#ifdef FFMPEG
		//CxImage * image = nullptr;
		switch (iFormat)
		{
			case MPG2:
			case MPEG:
			case AVCHD:
			case WINREC:
			case AVI:
			case MP4:
			case WMV:
			case MOV:
			{
				CThumbnailVideo video;	               
				listThumbnail = video.GetVideoListFrame(szFileName, width, height);
				break;
			}
		}
#endif
	}
	catch (...)
	{
		return nullptr;
	}

#ifdef FFMPEG
	int i = 0;
	for (CImageVideoThumbnail * thumb : listThumbnail)
	{
		videoThumbnail->bitmap[i] = ConvertCXImageToScaleRegardsBitmapCRgbaquad(thumb->image, widthOut, heightOut);


		if (thumb->rotation == 90)
		{
			videoThumbnail->bitmap[i]->Rotation90();
			videoThumbnail->bitmap[i]->VertFlipBuf();
		}
		else if (thumb->rotation == 270)
		{
			videoThumbnail->bitmap[i]->Rotation90();
		}

		if (videoThumbnail->bitmap[i] != nullptr)
			videoThumbnail->bitmap[i]->SetFilename(szFileName);



		videoThumbnail->timePosition[i] = thumb->timePosition;
		delete thumb->image;
		delete thumb;
		thumb = nullptr;
		i++;
	}
#endif

#if defined(WIN32) || defined(__APPLE__)

	int i = 0;
	for (CImageVideoThumbnail * thumb : listThumbnail)
	{
		videoThumbnail->bitmap[i] = thumb->image;

		if (thumb->rotation == 90)
		{
			videoThumbnail->bitmap[i]->Rotation90();
			videoThumbnail->bitmap[i]->VertFlipBuf();
		}
		else if (thumb->rotation == 270)
		{
			videoThumbnail->bitmap[i]->Rotation90();
		}

		if (videoThumbnail->bitmap[i] != nullptr)
			videoThumbnail->bitmap[i]->SetFilename(szFileName);

		videoThumbnail->timePosition[i] = thumb->timePosition;
		delete thumb;
		thumb = nullptr;
		i++;
	}

#endif

	listThumbnail.clear();

	return videoThumbnail;
}
#ifdef LIBRAW	
CRegardsBitmap * CLibPicture::LoadThumbnailFromRawPicture(const wxString & szFilename, int &returnValue)
{
	CRegardsBitmap * pBitmap = nullptr;
	CxImage * image = CRaw::GetThumbnail(szFilename);
	if (image == nullptr)
	{
		returnValue = -1;
	}
	else
	{
		wxString error = image->GetLastError();
		if (error == "")
		{
			pBitmap = ConvertCXImageToRegardsBitmap(image, 0);
		}
		delete image;


		returnValue = 0;
	}
	return pBitmap;
}
#endif

//--------------------------------------------------------------------------------------------
//Obtention d'un thumbnail à partir des informations exif
//--------------------------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::LoadThumbnail(const wxString & fileName, const bool &fromExifOnly)
{

#ifdef WIN32

	CImageDecoder imageDecoder;
	CRegardsBitmap * pBitmap = nullptr;
	pBitmap = imageDecoder.DecodeThumbnail(fileName);
	if (pBitmap == nullptr && !fromExifOnly)
		pBitmap = LoadThumbnailPicture(fileName, 200, 200);
	else
		pBitmap->VertFlipBuf();



	if (pBitmap != nullptr)
    {
        CPictureMetadata pictureMetadata(fileName);
        int orientation = pictureMetadata.GetOrientation();
        pBitmap->RotateExif(orientation);
        pBitmap->SetFilename(fileName);
    }
		
    

	return pBitmap;

#elif __APPLE__

	CRegardsBitmap * pBitmap = new CRegardsBitmap();
	CAppleReadPicture readPicture;
	if(!readPicture.ReadThumbnail(fileName, pBitmap) && !fromExifOnly)
	{
		pBitmap = LoadThumbnailPicture(fileName, 200, 200);
	}

	if (pBitmap != nullptr)
		pBitmap->SetFilename(fileName);
	return pBitmap;

#elif EXIV2

	//bool exif = false;
	//bool iptc = false;
	//bool xmp = false;
	int returnValue = -1;
	CRegardsBitmap * pBitmap = nullptr;

	int iFormat = TestImageFormat(fileName);

#ifdef LIBRAW		
	if (iFormat == RAW)
	{
		pBitmap = LoadThumbnailFromRawPicture(fileName, returnValue);
	}
	else
	{
#endif		

        wxString extension;
        CPictureMetadata pictureMetadata(fileName);
        int orientation = pictureMetadata.Orientation();
        CxImage * image = pictureMetadata.DecodeThumbnail(extension);
        if (image == nullptr && !fromExifOnly)
            pBitmap = LoadThumbnailPicture(fileName, 200, 200);
        else if(image != nullptr)
        {
			if (image != nullptr)
			{
				wxString error = image->GetLastError();
				printf("Message : %s\n",error.ToStdString().c_str());
				if (error == "")
					pBitmap = ConvertCXImageToRegardsBitmap(image, orientation);
				delete image;
			}
        }


#ifdef LIBRAW			
	}
#endif
	if (pBitmap != nullptr)
		pBitmap->SetFilename(fileName);
	return pBitmap;

#endif


}

void CLibPicture::DefineSVGSize(const int &width, const int &height)
{
    svgWidth = width;
    svgHeight = height;
}

//------------------------------------------------------------------------------
//Chargement d'une image par son nom
//------------------------------------------------------------------------------
CRegardsBitmap * CLibPicture::LoadPicture(const wxString & fileName)
{
	CRegardsBitmap * bitmap = nullptr;
	int iFormat = TestImageFormat(fileName);
	bool value = true;
	int rotation = 0;
	try
	{
		int typeImage = TYPE_IMAGE_CXIMAGE;
		wxImage imageWx;
		CxImage * image = nullptr;
		switch (iFormat)
		{


		case PPM:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("ppm"));
			break;

		case IFF:
			imageWx.LoadFile(fileName, wxBITMAP_TYPE_IFF); typeImage = TYPE_IMAGE_WXIMAGE;
			break;

        case ICO:
            imageWx.LoadFile(fileName, wxBITMAP_TYPE_ICON); typeImage = TYPE_IMAGE_WXIMAGE;
            break;
 
        case CUR:
            imageWx.LoadFile(fileName, wxBITMAP_TYPE_CUR); typeImage = TYPE_IMAGE_WXIMAGE;
              break;
                
		case XPM:
			imageWx.LoadFile(fileName, wxBITMAP_TYPE_XPM); typeImage = TYPE_IMAGE_WXIMAGE;
			break;
                
        case SVG:
        {
            wxSVGDocument* svgDoc = new wxSVGDocument;
            svgDoc->Load(fileName);
            imageWx = svgDoc->Render(svgWidth,svgHeight);
            typeImage = TYPE_IMAGE_WXIMAGE;
            delete svgDoc;
        }
        break;

#ifdef LIBBPG
		case BPG:
		{
            int size = 0;
            int width = 0;
            int height = 0;
#ifdef WIN32
			uint8_t * data = nullptr;
			int value = GetPictureBpg(fileName.c_str(), size, width, height, data, 0);
			if (value == 0)
			{
				int sizeData = size;
				data = new uint8_t[size];
				value = GetPictureBpg(fileName.c_str(), size, width, height, data, sizeData);
				bitmap = new CRegardsBitmap();
				bitmap->SetBitmap(data, width, height);
				delete[] data;
				data = nullptr;
			}
#else
			std::vector<uint8_t> myVector = libbpg->GetPngPicture(fileName.ToStdString().c_str(), size, width, height);
			CxMemFile memFile(&myVector[0], myVector.size());
			image = new CxImage(&memFile, CxImage::GetTypeIdFromName("png"));
			myVector.clear();
#endif
		}
		break;
#endif
           
	

#ifdef WIN32

		case PNG:
		case TIFF:
		case BMP:
		case JPEG:
		case RAW:
		{
			CImageDecoder imageDecoder;
			bitmap = imageDecoder.DecodePicture(fileName);
			bitmap->VertFlipBuf();

			CPictureMetadata pictureMetadata(fileName);
			int orientation = pictureMetadata.GetOrientation();
			bitmap->RotateExif(orientation);
		}
		break;
                
        case GIF:
            image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("gif"));
            break;
                
        case TGA:
            image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("tga"));
            break;

#endif
                
#ifdef __APPLE__

        case BMP:
            image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("bmp"));
            break;
                
		case TGA:
		case PNG:
		case TIFF:
		case GIF:
		case JPEG:
		case RAW:
		{
			bitmap = new CRegardsBitmap();
			CAppleReadPicture readPicture;
			readPicture.ReadPicture(fileName, bitmap);
		}
		break;

#endif
                
#ifdef __WXGTK__

		case JPEG:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jpg"));
			break;

		case BMP:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("bmp"));
			break;

		case TIFF:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("tif"));
			break;

		case PNG:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("png"));
			break;
                
        case GIF:
            image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("gif"));
            break;
                
        case TGA:
            image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("tga"));
            break;
                
#ifdef LIBRAW
		case RAW:
			bitmap = CRaw::GetPicture(fileName.ToStdString());
			break;
#endif

#endif

		case PNM:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("ppm"));
			break;

		case PCX:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("pcx"));
			break;




		case JPEG2000:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jp2"));
			break;

		case PCD:
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			CCPCD * pPCD = new CCPCD();
			bitmap = pPCD->readPCD(fileName.ToStdString());
			delete pPCD;
		}
		break;

		case PSD:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("psd"));
			break;
		case PGX:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("pgx"));
			break;
		case MNG:
			image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("mng"));
			break;
        case JPC:
            image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jpc"));
            break;

#ifdef WIN32
		case MPG2:
		case MPEG:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			CVideo video;
			bitmap = video.GetVideoFrame(fileName.ToStdString(), rotation);
			return bitmap;
		}
#endif

#ifdef __APPLE__
		case MPG2:
		case MPEG:
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			bitmap = new CRegardsBitmap();
			CAppleReadPicture readPicture;
			readPicture.GetVideoThumbnail(fileName, bitmap, 200, 200);
		}
		break;
#endif


#ifdef FFMPEG
		case AVI:
		case MP4:
		case WMV:
		case MOV:
		{
			typeImage = TYPE_IMAGE_REGARDSIMAGE;
			CThumbnailVideo video;
			image = video.GetVideoFrame(fileName.ToStdString(), rotation);
			bitmap = ConvertCXImageToRegardsBitmap(image);
			bitmap->SetFilename(fileName);
			delete image;
			return bitmap;
		}
		break;
#endif
		}



		if (image != nullptr && typeImage == TYPE_IMAGE_CXIMAGE)
		{
			wxString error = image->GetLastError();
			if (error == "")
				bitmap = ConvertCXImageToRegardsBitmap(image);
			else
				wxMessageBox(error,"Error Loading");
			delete image;
		}
		else if (typeImage == TYPE_IMAGE_WXIMAGE)
		{
			if (imageWx.IsOk())
            {
                bitmap = ConvertwxImageToRegardsBitmap(imageWx);
            }
		}

		if (iFormat != 0 && value && bitmap != nullptr)
		{
			bitmap->SetFilename(fileName);
		}
	}
	catch (...)
	{
		return nullptr;
	}


	return bitmap;
}

CRegardsBitmap * CLibPicture::ConvertWXImageToRegardsBitmap(wxImage * image)
{
	CRegardsBitmap * bitmap = new CRegardsBitmap(image->GetWidth(), image->GetHeight());
	uint8_t * dstData = bitmap->GetPtBitmap();
	int widthSrcSize = image->GetWidth() << 2;
	uint8_t * data = image->GetData();

#pragma omp parallel for
	for (int y = 0; y < image->GetHeight(); y++)
	{
		for (int x = 0; x < image->GetWidth(); x++)
		{
			int posDest = ((image->GetHeight() - y) * widthSrcSize) - widthSrcSize;
			int posSrc = x * 3 + (y * image->GetWidth()) * 3;

			dstData[posDest + 2] = data[posSrc];
			dstData[posDest + 1] = data[posSrc+1];
			dstData[posDest + 0] = data[posSrc+2];
            dstData[posDest + 3] = 255;
		}
	}

	return bitmap;
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
#pragma omp parallel for
		for (int y = 0; y < height; y++)
		{
			posData = ((height - y) * widthSrcSize) - widthSrcSize;
			posDataOut = y * (width * 3);
			posAlpha = y * width;
			for (int x = 0; x < width; x++)
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

#ifdef WIN32

	CPictureMetadata pictureMetadata(filename);
	return pictureMetadata.HasThumbnail();

#elif __APPLE__

	CAppleReadPicture readPicture;
	return readPicture.HasThumbnail(filename);

#else
    int iFormat = TestImageFormat(filename);
    if(iFormat == RAW)
        return true;
    else if(iFormat == TIFF || iFormat == JPEG)
    {
		CPictureMetadata pictureMetadata(filename);
		return pictureMetadata.HasThumbnail();
    }
#endif    
    return false;
}

//--------------------------------------------------------------------------------------------------
//Fonction d'appel pour l'obtention des dimensions d'une image
//--------------------------------------------------------------------------------------------------
int CLibPicture::GetDimensions(const wxString & fileName, int & width, int & height, int & rotation)
{
	int iFormat = TestImageFormat(fileName);
	width = 0;
	height = 0;
	CxImage * image = nullptr;
    wxImage imageWx;
    int typeImage = TYPE_IMAGE_CXIMAGE;

	switch (iFormat)
	{
    
            
	case JPEG:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jpg"), true);
		break;

	case BMP:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("bmp"), true);
		break;

	case TIFF:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("tif"), true);
		break;
            
    case IFF:
        imageWx.LoadFile(fileName, wxBITMAP_TYPE_IFF); typeImage = TYPE_IMAGE_WXIMAGE;
        break;
            
    case XPM:
        imageWx.LoadFile(fileName, wxBITMAP_TYPE_XPM); typeImage = TYPE_IMAGE_WXIMAGE;
        break;
#ifdef LIBRAW	
	case RAW:
		//image = new CxImage(file, CxImage::GetTypeIdFromName("raw"), true);
		CRaw::GetDimensions(fileName.ToStdString(), width, height);
		break;
#endif

#ifdef WIN32

	case RAW:
		{
			unsigned int originalWidth, originalHeight;
			CImageDecoder imageDecoder;
			imageDecoder.GetPictureDimension(fileName, &originalWidth, &originalHeight);
		}
		break;

#endif


#ifdef __APPLE__

	case RAW:
	{
		int _w = 0;
		int _h = 0;
		CAppleReadPicture readPicture;
		readPicture.GetPictureDimensions(fileName, &_w, &_h);
		width = _w;
		height = _h;
		break;
	}
#endif


	case GIF:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("gif"), true);
		break;

	case PNM:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("ppm"), true);
		break;

	case PCX:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("pcx"), true);
		break;

	case PNG:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("png"), true);
		break;

	case TGA:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("tga"), true);
		break;

	case JPEG2000:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("jp2"), true);
		break;

	case PCD:
		{
			CCPCD * pPCD = new CCPCD();
			pPCD->GetJDimensions(fileName.ToStdString(), width, height);
			delete pPCD;
		}
		break;
	case PSD:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("psd"), true);
		break;
	case PGX:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("pgx"), true);
		break;
	case MNG:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("mng"), true);
		break;


	case PPM:
		image = new CxImage(fileName.ToStdString().c_str(), CxImage::GetTypeIdFromName("ppm"), true);
		break;
            
    case SVG:
        width = this->svgWidth;
        height = this->svgHeight;
        break;
            
#ifdef LIBBPG
	case BPG:
		{
			int _w = 0;
			int _h = 0;
            
#ifdef WIN32
			GetDimensionsBpg(fileName.ToStdString().c_str(), _w, _h);
#else
			libbpg->GetDimensionsBpg(fileName.ToStdString().c_str(), _w, _h);
#endif
			width = _w;
			height = _h;

		}
		break;
#endif


#ifdef WIN32
	case MPG2:
	case MPEG:
	case AVI:
	case MP4:
	case WMV:
	case MOV:
	{
		CVideo video;
		video.GetVideoDimensions(fileName, width, height, rotation);
	}
	break;
#endif

#ifdef __APPLE__
	case MPG2:
	case MPEG:
	case AVI:
	case MP4:
	case WMV:
	case MOV:
	{
		int _w = 0;
		int _h = 0;
		CAppleReadPicture readPicture;
		readPicture.GetVideoDimensions(fileName,&_w,&_h);
	}
	break;

#endif

#ifdef FFMPEG
	case MPG2:
	case MPEG:
	case AVCHD:
	case WINREC:
	case AVI:
	case MP4:
	case WMV:
	case MOV:
		{
			CThumbnailVideo video;
			video.GetVideoDimensions(fileName, width, height, rotation);
		}
		break;
#endif
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
