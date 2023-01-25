// xImaCodec.cpp : Encode Decode functions
/* 07/08/2001 v1.00 - Davide Pizzolato - www.xdp.it
 * CxImage version 7.0.1 07/Jan/2011
 */

#include "ximage.h"
#include <tbb/parallel_for.h>
//#if defined(__x86_64__) || defined(_M_AMD64)	
//#include <tmmintrin.h>
//#endif 

#if CXIMAGE_SUPPORT_JPG
#include "ximajpg.h"
#endif

#if CXIMAGE_SUPPORT_GIF
#include "ximagif.h"
#endif

#if CXIMAGE_SUPPORT_PNG
#include "ximapng.h"
#endif

#if CXIMAGE_SUPPORT_MNG
#include "ximamng.h"
#endif

#if CXIMAGE_SUPPORT_BMP
#include "ximabmp.h"
#endif

#if CXIMAGE_SUPPORT_ICO
#include "ximaico.h"
#endif

#if CXIMAGE_SUPPORT_TIF
#include "ximatif.h"
#endif

#if CXIMAGE_SUPPORT_TGA
#include "ximatga.h"
#endif

#if CXIMAGE_SUPPORT_PCX
#include "ximapcx.h"
#endif

#if CXIMAGE_SUPPORT_WBMP
#include "ximawbmp.h"
#endif

#if CXIMAGE_SUPPORT_WMF
#include "ximawmf.h" // <vho> - WMF/EMF support
#endif

#if CXIMAGE_SUPPORT_JBG
#include "ximajbg.h"
#endif

#if CXIMAGE_SUPPORT_JASPER
#include "ximajas.h"
#endif

#if CXIMAGE_SUPPORT_SKA
#include "ximaska.h"
#endif

#if CXIMAGE_SUPPORT_RAW
#include "ximaraw.h"
#endif

#if CXIMAGE_SUPPORT_PSD
#include "ximapsd.h"
#endif

struct weightX
{
public:
	float tabF[4];
};


float Filter(const float& f)
{
	return (f < -2.0 || f > 2.0)
		       ? 0.0
		       : (
			       (f < -1.0)
				       ? (2.0 + f) * (2.0 + f) * (2.0 + f) / 6.0
				       : (
					       (f < 0.0)
						       ? (4.0 + f * f * (-6.0 - 3.0 * f)) / 6.0
						       : (
							       (f < 1.0)
								       ? (4.0 + f * f * (-6.0 + 3.0 * f)) / 6.0
								       : (2.0 - f) * (2.0 - f) * (2.0 - f) / 6.0)));
}


////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_ENCODE
////////////////////////////////////////////////////////////////////////////////
bool CxImage::EncodeSafeCheck(CxFile* hFile)
{
	if (hFile == nullptr)
	{
		strcpy(info.szLastError,CXIMAGE_ERR_NOFILE);
		return true;
	}

	if (pDib == nullptr)
	{
		strcpy(info.szLastError,CXIMAGE_ERR_NOIMAGE);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
//#ifdef WIN32
//bool CxImage::Save(LPCWSTR filename, uint32_t imagetype)
//{
//	FILE* hFile;	//file handle to write the image
//	if ((hFile=_wfopen(filename,L"wb"))==NULL)  return false;
//	bool bOK = Encode(hFile,imagetype);
//	fclose(hFile);
//	return bOK;
//}
//#endif //WIN32
////////////////////////////////////////////////////////////////////////////////
// For UNICODE support: char -> char
/**
 * Saves to disk the image in a specific format.
 * \param filename: file name
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 */
bool CxImage::Save(const char* filename, uint32_t imagetype)
{
	FILE* hFile; //file handle to write the image

#ifdef WIN32
	if ((hFile = _tfopen(filename,_T("wb"))) == nullptr) return false; // For UNICODE support
#else
	if ((hFile=fopen(filename,"wb"))==NULL)  return false;
#endif

	bool bOK = Encode(hFile, imagetype);
	fclose(hFile);
	return bOK;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Saves to disk the image in a specific format.
 * \param hFile: file handle, open and enabled for writing.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 */
bool CxImage::Encode(FILE* hFile, uint32_t imagetype)
{
	CxIOFile file(hFile);
	return Encode(&file, imagetype);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Saves to memory buffer the image in a specific format.
 * \param buffer: output memory buffer pointer. Must be NULL,
 * the function allocates and fill the memory,
 * the application must free the buffer, see also FreeMemory().
 * \param size: output memory buffer size.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 */
bool CxImage::Encode(uint8_t* & buffer, int32_t& size, uint32_t imagetype)
{
	if (buffer != nullptr)
	{
		strcpy(info.szLastError, "the buffer must be empty");
		return false;
	}
	CxMemFile file;
	file.Open();
	if (Encode(&file, imagetype))
	{
		buffer = file.GetBuffer();
		size = file.Size();
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Saves to disk the image in a specific format.
 * \param hFile: file handle (CxMemFile or CxIOFile), with write access.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 * \sa ENUM_CXIMAGE_FORMATS
 */
bool CxImage::Encode(CxFile* hFile, uint32_t imagetype)
{
#if CXIMAGE_SUPPORT_BMP
	if (CXIMAGE_FORMAT_BMP==imagetype){
		CxImageBMP *newima = new CxImageBMP;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_ICO
	if (CXIMAGE_FORMAT_ICO==imagetype){
		CxImageICO *newima = new CxImageICO;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_TIF
	if (CXIMAGE_FORMAT_TIF==imagetype){
		CxImageTIF *newima = new CxImageTIF;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_JPG
	if (CXIMAGE_FORMAT_JPG==imagetype){
		CxImageJPG *newima = new CxImageJPG;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_GIF
	if (CXIMAGE_FORMAT_GIF == imagetype)
	{
		auto newima = new CxImageGIF;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile))
		{
			delete newima;
			return true;
		}
		strcpy(info.szLastError, newima->GetLastError());
		delete newima;
		return false;
	}
#endif
#if CXIMAGE_SUPPORT_PNG
	if (CXIMAGE_FORMAT_PNG == imagetype)
	{
		auto newima = new CxImagePNG;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile))
		{
			delete newima;
			return true;
		}
		strcpy(info.szLastError, newima->GetLastError());
		delete newima;
		return false;
	}
#endif
#if CXIMAGE_SUPPORT_MNG
	if (CXIMAGE_FORMAT_MNG==imagetype){
		CxImageMNG *newima = new CxImageMNG;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_TGA
	if (CXIMAGE_FORMAT_TGA==imagetype){
		CxImageTGA *newima = new CxImageTGA;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_PCX
	if (CXIMAGE_FORMAT_PCX==imagetype){
		CxImagePCX *newima = new CxImagePCX;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_WBMP
	if (CXIMAGE_FORMAT_WBMP==imagetype){
		CxImageWBMP *newima = new CxImageWBMP;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_WMF && CXIMAGE_SUPPORT_WINDOWS // <vho> - WMF/EMF support
	if (CXIMAGE_FORMAT_WMF==imagetype){
		CxImageWMF *newima = new CxImageWMF;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_JBG
	if (CXIMAGE_FORMAT_JBG == imagetype)
	{
		auto newima = new CxImageJBG;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile))
		{
			delete newima;
			return true;
		}
		strcpy(info.szLastError, newima->GetLastError());
		delete newima;
		return false;
	}
#endif
#if CXIMAGE_SUPPORT_JASPER
	if (
#if	CXIMAGE_SUPPORT_JP2
		CXIMAGE_FORMAT_JP2==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_JPC
		CXIMAGE_FORMAT_JPC==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_PGX
		CXIMAGE_FORMAT_PGX==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_PNM
		CXIMAGE_FORMAT_PNM==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_RAS
		CXIMAGE_FORMAT_RAS==imagetype || 
#endif
		 false ){
		CxImageJAS *newima = new CxImageJAS;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile,imagetype)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif

#if CXIMAGE_SUPPORT_SKA
	if (CXIMAGE_FORMAT_SKA == imagetype)
	{
		auto newima = new CxImageSKA;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile))
		{
			delete newima;
			return true;
		}
		strcpy(info.szLastError, newima->GetLastError());
		delete newima;
		return false;
	}
#endif

#if CXIMAGE_SUPPORT_RAW
	if (CXIMAGE_FORMAT_RAW==imagetype){
		CxImageRAW *newima = new CxImageRAW;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif

#if CXIMAGE_SUPPORT_PSD
	if (CXIMAGE_FORMAT_PSD==imagetype){
		CxImagePSD *newima = new CxImagePSD;
		if (!newima) return false;
		newima->Ghost(this);
		if (newima->Encode(hFile)){
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			delete newima;
			return false;
		}
	}
#endif

	strcpy(info.szLastError, "Encode: Unknown format");
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Saves to disk or memory pagecount images, referenced by an array of CxImage pointers.
 * \param hFile: file handle.
 * \param pImages: array of CxImage pointers.
 * \param pagecount: number of images.
 * \param imagetype: can be CXIMAGE_FORMAT_TIF or CXIMAGE_FORMAT_GIF.
 * \return true if everything is ok
 */
bool CxImage::Encode(FILE* hFile, CxImage** pImages, int32_t pagecount, uint32_t imagetype)
{
	CxIOFile file(hFile);
	return Encode(&file, pImages, pagecount, imagetype);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Saves to disk or memory pagecount images, referenced by an array of CxImage pointers.
 * \param hFile: file handle (CxMemFile or CxIOFile), with write access.
 * \param pImages: array of CxImage pointers.
 * \param pagecount: number of images.
 * \param imagetype: can be CXIMAGE_FORMAT_TIF, CXIMAGE_FORMAT_GIF or CXIMAGE_FORMAT_ICO.
 * \return true if everything is ok
 */
bool CxImage::Encode(CxFile* hFile, CxImage** pImages, int32_t pagecount, uint32_t imagetype)
{
#if CXIMAGE_SUPPORT_TIF
	if (imagetype==CXIMAGE_FORMAT_TIF){
		CxImageTIF newima;
		newima.Ghost(this);
		if (newima.Encode(hFile,pImages,pagecount)){
			return true;
		} else {
			strcpy(info.szLastError,newima.GetLastError());
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_GIF
	if (imagetype == CXIMAGE_FORMAT_GIF)
	{
		CxImageGIF newima;
		newima.Ghost(this);
		if (newima.Encode(hFile, pImages, pagecount))
		{
			return true;
		}
		strcpy(info.szLastError, newima.GetLastError());
		return false;
	}
#endif
#if CXIMAGE_SUPPORT_ICO
	if (imagetype==CXIMAGE_FORMAT_ICO){
		CxImageICO newima;
		newima.Ghost(this);
		if (newima.Encode(hFile,pImages,pagecount)){
			return true;
		} else {
			strcpy(info.szLastError,newima.GetLastError());
			return false;
		}
	}
#endif
	strcpy(info.szLastError, "Multipage Encode, Unsupported operation for this format");
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * exports the image into a RGBA buffer, Useful for OpenGL applications.
 * \param buffer: output memory buffer pointer. Must be NULL,
 * the function allocates and fill the memory,
 * the application must free the buffer, see also FreeMemory().
 * \param size: output memory buffer size.
 * \param bFlipY: direction of Y axis. default = false.
 * \return true if everything is ok
 */
bool CxImage::Encode2RGBA(uint8_t* & buffer, int32_t& size, bool bFlipY)
{
	if (buffer != nullptr)
	{
		strcpy(info.szLastError, "the buffer must be empty");
		return false;
	}
	CxMemFile file;
	file.Open();
	if (Encode2RGBA(&file, bFlipY))
	{
		buffer = file.GetBuffer();
		size = file.Size();
		return true;
	}
	return false;
}

void CxImage::InterpolationBicubicRGB(uint8_t* & dataOut, const int& width, const int& height)
{
	int widthIn = head.biWidth;
	int heightIn = head.biHeight;

	float ratioX = static_cast<float>(widthIn) / static_cast<float>(width);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(height);

	auto wX = new weightX[width];
	auto wY = new weightX[height];

	tbb::parallel_for(0, height, 1, [=](int y)
	{
		float posY = static_cast<float>(y) * ratioY;
		int valueB = static_cast<int>(posY);
		float realB = posY - valueB;
		wY[y].tabF[0] = Filter(-(-1.0f - realB));
		wY[y].tabF[1] = Filter(-(0.0f - realB));
		wY[y].tabF[2] = Filter(-(1.0f - realB));
		wY[y].tabF[3] = Filter(-(2.0f - realB));
	});

	tbb::parallel_for(0, width, 1, [=](int x)
	{
		float posX = static_cast<float>(x) * ratioX;
		int valueA = static_cast<int>(posX);
		float realA = posX - valueA;
		wX[x].tabF[0] = Filter((-1.0f - realA));
		wX[x].tabF[1] = Filter((0.0f - realA));
		wX[x].tabF[2] = Filter((1.0f - realA));
		wX[x].tabF[3] = Filter((2.0f - realA));
	});


	for (auto y = 0; y < height; y++)
	{
		float posY = static_cast<float>(height - y - 1) * ratioY;
		int tailleYOut = y * width;


		for (auto x = 0; x < width; x++)
		{
			float posX = static_cast<float>(x) * ratioX;
			int position = (x + tailleYOut) << 2;
			uint8_t data[4] = {0, 0, 0, 0};
			BicubicRGB(data, widthIn, heightIn, posX, posY, wY[y].tabF, wX[x].tabF);
			memcpy(dataOut + position, data, 4);
		}
	}
	delete[] wX;
	delete[] wY;
}

void CxImage::BicubicRGB(uint8_t* data, const int& width, const int& height, const float& x, const float& y,
                         float* tabF1, float* tabF)
{
	float nDenom = 0.0;
	int valueA = static_cast<int>(x);
	int valueB = static_cast<int>(y);

	float r = 0.0, g = 0.0, b = 0.0, a = 0.0;

	int debutN = -1;
	int finN = 2;
	//Calcul démarrage du y;
	if (valueB == 0)
	{
		debutN = 0;
	}

	if (valueB == height - 2)
	{
		finN = 1;
	}

	if (valueB == height - 1)
	{
		finN = 0;
	}

	int debutM = -1;
	int finM = 2;
	//Calcul démarrage du y;
	if (valueA == 0)
	{
		debutM = 0;
	}

	if (valueA == width - 2)
	{
		finM = 1;
	}

	if (valueA == width - 1)
	{
		finM = 0;
	}

	int posX = valueA + debutM;
	if (valueA == 1)
		posX = valueA;

	int posY = valueB + debutN;
	if (valueB == 1)
		posY = valueB;

	for (auto n = debutN; n <= finN; n++)
	{
		for (auto m = debutM; m <= finM; m++)
		{
			RGBQUAD color = BlindGetPixelColor(posX + m, posY + n);
			float f = tabF1[n + 1] * tabF[m + 1];
			nDenom += f;
			r += static_cast<float>(color.rgbRed) * f;
			g += static_cast<float>(color.rgbGreen) * f;
			b += static_cast<float>(color.rgbBlue) * f;
			a += static_cast<float>(color.rgbReserved) * f;
		}
	}

	data[0] = static_cast<uint8_t>(r / nDenom);
	data[1] = static_cast<uint8_t>(g / nDenom);
	data[2] = static_cast<uint8_t>(b / nDenom);
	data[3] = static_cast<uint8_t>(a / nDenom);
}


void CxImage::InterpolationBicubicBGR(uint8_t* & dataOut, const int& width, const int& height)
{
	int widthIn = head.biWidth;
	int heightIn = head.biHeight;

	float ratioX = static_cast<float>(widthIn) / static_cast<float>(width);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(height);

	auto wX = new weightX[width];
	auto wY = new weightX[height];


	for (auto y = 0; y < height; y++)
	{
		float posY = static_cast<float>(y) * ratioY;
		int valueB = static_cast<int>(posY);
		float realB = posY - valueB;
		wY[y].tabF[0] = Filter(-(-1.0f - realB));
		wY[y].tabF[1] = Filter(-(0.0f - realB));
		wY[y].tabF[2] = Filter(-(1.0f - realB));
		wY[y].tabF[3] = Filter(-(2.0f - realB));
	}


	for (auto x = 0; x < width; x++)
	{
		float posX = static_cast<float>(x) * ratioX;
		int valueA = static_cast<int>(posX);
		float realA = posX - valueA;
		wX[x].tabF[0] = Filter((-1.0f - realA));
		wX[x].tabF[1] = Filter((0.0f - realA));
		wX[x].tabF[2] = Filter((1.0f - realA));
		wX[x].tabF[3] = Filter((2.0f - realA));
	}


	for (auto y = 0; y < height; y++)
	{
		float posY = static_cast<float>(y) * ratioY;
		int tailleYOut = y * width;


		for (auto x = 0; x < width; x++)
		{
			float posX = static_cast<float>(x) * ratioX;
			int position = (x + tailleYOut) << 2;
			uint8_t data[4] = {0, 0, 0, 0};
			BicubicBGR(data, widthIn, heightIn, posX, posY, wY[y].tabF, wX[x].tabF);
			memcpy(dataOut + position, data, 4);
		}
	}
	delete[] wX;
	delete[] wY;
}

void CxImage::BicubicBGR(uint8_t* data, const int& width, const int& height, const float& x, const float& y,
                         float* tabF1, float* tabF)
{
	float nDenom = 0.0;
	int valueA = static_cast<int>(x);
	int valueB = static_cast<int>(y);

	float r = 0.0, g = 0.0, b = 0.0, a = 0.0;

	int debutN = -1;
	int finN = 2;

	//Calcul démarrage du y;
	if (valueB == 0)
	{
		debutN = 0;
	}

	if (valueB == height - 2)
	{
		finN = 1;
	}

	if (valueB == height - 1)
	{
		finN = 0;
	}

	int debutM = -1;
	int finM = 2;
	//Calcul démarrage du y;
	if (valueA == 0)
	{
		debutM = 0;
	}

	if (valueA == width - 2)
	{
		finM = 1;
	}

	if (valueA == width - 1)
	{
		finM = 0;
	}

	int posX = valueA + debutM;
	if (valueA == 1)
		posX = valueA;

	int posY = valueB + debutN;
	if (valueB == 1)
		posY = valueB;

	for (auto n = debutN; n <= finN; n++)
	{
		//uint8_t * color = (uint8_t*)BlindGetPixelPointer(posX + debutM, posY + n);

		for (auto m = debutM; m <= finM; m++)
		{
			RGBQUAD color = BlindGetPixelColor(posX + m, posY + n);

			float f = tabF1[n + 1] * tabF[m + 1];
			nDenom += f;

			/*
			b += value[*color++] * f;
			g += value[*color++] * f;
			r += value[*color++] * f;
			*/
			r += static_cast<float>(color.rgbRed) * f;
			g += static_cast<float>(color.rgbGreen) * f;
			b += static_cast<float>(color.rgbBlue) * f;
			a += static_cast<float>(color.rgbReserved) * f;
		}
	}

	data[0] = static_cast<uint8_t>(b / nDenom);
	data[1] = static_cast<uint8_t>(g / nDenom);
	data[2] = static_cast<uint8_t>(r / nDenom);
	data[3] = static_cast<uint8_t>(a / nDenom);
}


bool CxImage::Encode2RGBA32F(float* buffer, bool bFlipY)
{
	//float * buffer = new float[head.biHeight * head.biWidth * 4];
	long yPos = 0;
	if (head.biClrUsed)
	{
		for (long y1 = 0; y1 < head.biHeight; y1++, yPos += info.dwEffWidth)
		{
			//long y = bFlipY ? head.biHeight - 1 - y1 : y1;

			//uint8_t* iDst = info.pImage + yPos;

			for (long x = 0, xPos = 0; x < head.biWidth; x++, xPos += 3)
			{
				uint8_t idx;

				if (head.biBitCount == 8)
				{
					idx = info.pImage[yPos + x];
				}
				else
				{
					uint8_t pos;
					uint8_t iDst = info.pImage[yPos + (x * head.biBitCount >> 3)];
					if (head.biBitCount == 4)
					{
						pos = static_cast<uint8_t>(4 * (1 - x % 2));
						iDst &= (0x0F << pos);
						idx = static_cast<uint8_t>(iDst >> pos);
					}
					else if (head.biBitCount == 1)
					{
						pos = static_cast<uint8_t>(7 - x % 8);
						iDst &= (0x01 << pos);
						idx = static_cast<uint8_t>(iDst >> pos);
					}
				}

				if ((pDib) && (head.biClrUsed))
				{
					uint8_t* iDst = static_cast<uint8_t*>(pDib) + sizeof(BITMAPINFOHEADER);
					if (idx < head.biClrUsed)
					{
						RGBQUAD color;
						long ldx = idx * sizeof(RGBQUAD);
						memcpy(&color, iDst + ldx, sizeof(RGBQUAD));
						*buffer++ = static_cast<int>(color.rgbRed);
						*buffer++ = static_cast<int>(color.rgbGreen);
						*buffer++ = static_cast<int>(color.rgbBlue);
						*buffer++ = static_cast<int>(color.rgbReserved);
					}
				}

				//buffer += 4;
			}
		}
	}
	else
	{
		if (pAlpha)
		{
			for (auto y = 0; y < head.biHeight; y++)
			{
				for (auto x = 0; x < head.biWidth; x++)
				{
					int position = (y * head.biWidth << 2) + (x << 2);
					RGBQUAD color = BlindGetPixelColor(x, y);
					buffer[position] = static_cast<float>(color.rgbRed);
					buffer[position + 1] = static_cast<float>(color.rgbGreen);
					buffer[position + 2] = static_cast<float>(color.rgbBlue);
					buffer[position + 3] = static_cast<float>(color.rgbReserved);
				}
			}
		}
		else
		{
			for (long y1 = 0; y1 < head.biHeight; y1++)
			{
				for (long x = 0; x < head.biWidth; x++)
				{
					int position = (y1 * head.biWidth * 4) + (x * 4);
					int imagePos = (y1 * info.dwEffWidth) + (x * 3);

					buffer[position] = static_cast<float>(*(info.pImage + imagePos));
					buffer[position + 1] = static_cast<float>(*(info.pImage + imagePos + 1));
					buffer[position + 2] = static_cast<float>(*(info.pImage + imagePos + 2));
					buffer[position + 3] = 0.0f;
				}
			}
		}
	}

	return false;
}


bool CxImage::Encode2BGRAFloat(float* buffer, long size, bool bFlipY)
{
	for (long y = 0; y < head.biHeight; y++)
	{
		for (long x = 0; x < head.biWidth; x++)
		{
			RGBQUAD color = BlindGetPixelColor(x, y);
			*buffer++ = static_cast<float>(color.rgbRed) / 255.0f;
			*buffer++ = static_cast<float>(color.rgbGreen) / 255.0f;
			*buffer++ = static_cast<float>(color.rgbBlue) / 255.0f;
			*buffer++ = static_cast<float>(color.rgbReserved) / 255.0f;
		}
	}
	return true;
}

bool CxImage::Encode2BGRA(uint8_t* buffer, long size, bool bFlipY)
{
	//uint8_t* iDst  = info.pImage;
	long yPos = 0;
	if (head.biClrUsed)
	{
		for (long y1 = 0; y1 < head.biHeight; y1++, yPos += info.dwEffWidth)
		{
			//long y = bFlipY ? head.biHeight - 1 - y1 : y1;

			//uint8_t* iDst = info.pImage + yPos;

			for (long x = 0, xPos = 0; x < head.biWidth; x++, xPos += 3)
			{
				//RGBQUAD rgb;
				uint8_t idx;

				if (head.biBitCount == 8)
				{
					idx = info.pImage[yPos + x];
				}
				else
				{
					uint8_t pos;
					uint8_t iDst = info.pImage[yPos + (x * head.biBitCount >> 3)];
					if (head.biBitCount == 4)
					{
						pos = static_cast<uint8_t>(4 * (1 - x % 2));
						iDst &= (0x0F << pos);
						idx = static_cast<uint8_t>(iDst >> pos);
					}
					else if (head.biBitCount == 1)
					{
						pos = static_cast<uint8_t>(7 - x % 8);
						iDst &= (0x01 << pos);
						idx = static_cast<uint8_t>(iDst >> pos);
					}
				}

				if ((pDib) && (head.biClrUsed))
				{
					uint8_t* iDst = static_cast<uint8_t*>(pDib) + sizeof(BITMAPINFOHEADER);
					if (idx < head.biClrUsed)
					{
						long ldx = idx * sizeof(RGBQUAD);
						memcpy(buffer, iDst + ldx, sizeof(uint8_t) << 2);
					}
				}

				buffer += 4;
			}
		}
	}
	else
	{
		if (pAlpha)
		{
			for (long y = 0; y < head.biHeight; y++)
			{
				for (long x = 0; x < head.biWidth; x++)
				{
					RGBQUAD color = BlindGetPixelColor(x, y);
					memcpy(buffer + (y * head.biWidth << 2) + (x << 2), &color, sizeof(uint8_t) << 2);
				}
			}
		}
		else
		{
			/*
			#if defined(__x86_64__) || defined(_M_AMD64)			
						int restant = head.biWidth % 16;
						int width = head.biWidth - restant;
						int sizeofCopy = sizeof(uint8_t) * 3;
						__m128i mask = _mm_setr_epi8(0, 1, 2, -1, 3, 4, 5, -1, 6, 7, 8, -1, 9, 10, 11, -1);
			
			
						for (long y = 0; y < head.biHeight; y++)
						{
							int ySrcWidth = y * info.dwEffWidth;
							int yDstWidth = y * head.biWidth << 2;
							for (long x = 0; x < width; x += 16)
							{
								uint8_t * dataDst = buffer + yDstWidth + (x << 2);
								uint8_t * data = info.pImage + ySrcWidth + (x * 3);
			
								__m128i * src = (__m128i *)data;
								__m128i * dst = (__m128i *)dataDst;
			
								__m128i sa = _mm_loadu_si128((__m128i *)src);
								__m128i sb = _mm_loadu_si128((__m128i *)(src + 1));
								__m128i sc = _mm_loadu_si128((__m128i *)(src + 2));
			
								__m128i val = _mm_shuffle_epi8(sa, mask);
								_mm_storeu_si128(dst, val);
								val = _mm_shuffle_epi8(_mm_alignr_epi8(sb, sa, 12), mask);
								_mm_storeu_si128(dst + 1, val);
								val = _mm_shuffle_epi8(_mm_alignr_epi8(sc, sb, 8), mask);
								_mm_storeu_si128(dst + 2, val);
								val = _mm_shuffle_epi8(_mm_alignr_epi8(sc, sc, 4), mask);
								_mm_storeu_si128(dst + 3, val);
							
							}
							for (long x = width; x < head.biWidth; x++)
							{
								memcpy(buffer + yDstWidth + (x << 2), info.pImage + ySrcWidth + (x * 3), sizeofCopy);
							}
						}
			#else
			
			
						for (long y = 0; y < head.biHeight; y++)
						{
			
							for (long x = 0; x < head.biWidth; x++)
							{
								RGBQUAD color = BlindGetPixelColor(x, y);
								memcpy(buffer + (y * head.biWidth << 2) + (x << 2), &color, sizeof(uint8_t) << 2);
							}
						}
			#endif
			*/

			for (long y = 0; y < head.biHeight; y++)
			{
				for (long x = 0; x < head.biWidth; x++)
				{
					RGBQUAD color = BlindGetPixelColor(x, y);
					memcpy(buffer + (y * head.biWidth << 2) + (x << 2), &color, sizeof(uint8_t) << 2);
				}
			}
			//#endif
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * exports the image into a RGBA buffer, Useful for OpenGL applications.
 * \param hFile: file handle (CxMemFile or CxIOFile), with write access.
 * \param bFlipY: direction of Y axis. default = false.
 * \return true if everything is ok
 */
bool CxImage::Encode2RGBA(CxFile* hFile, bool bFlipY)
{
	if (EncodeSafeCheck(hFile)) return false;

	tbb::parallel_for(0, static_cast<int>(head.biHeight), 1, [=](int y1)
	{
		int32_t y = bFlipY ? head.biHeight - 1 - y1 : y1;
		for (int32_t x = 0; x < head.biWidth; x++)
		{
			RGBQUAD color = BlindGetPixelColor(x, y);
			hFile->PutC(color.rgbRed);
			hFile->PutC(color.rgbGreen);
			hFile->PutC(color.rgbBlue);
			hFile->PutC(color.rgbReserved);
		}
	});
	return true;
}

////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_ENCODE
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
// For UNICODE support: char -> char
/**
 * Reads from disk the image in a specific format.
 * - If decoding fails using the specified image format,
 * the function will try the automatic file format recognition.
 *
 * \param filename: file name
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 */
bool CxImage::Load(const char* filename, uint32_t imagetype)
//bool CxImage::Load(const char * filename, uint32_t imagetype)
{
	/*FILE* hFile;	//file handle to read the image
	if ((hFile=fopen(filename,"rb"))==NULL)  return false;
	bool bOK = Decode(hFile,imagetype);
	fclose(hFile);*/

	/* automatic file type recognition */
	bool bOK = false;
	if (GetTypeIndexFromId(imagetype))
	{
		FILE* hFile; //file handle to read the image

#ifdef WIN32
		if ((hFile = _tfopen(filename, _T("rb"))) == nullptr) return false; // For UNICODE support
#else
		if ((hFile=fopen(filename,"rb"))==NULL)  return false;
#endif

		bOK = Decode(hFile, imagetype);
		fclose(hFile);
		if (bOK) return bOK;
	}
	else
		return false;

	char szError[256];
	strcpy(szError, info.szLastError); //save the first error

	/*
	// if failed, try automatic recognition of the file...
	FILE* hFile;

#ifdef WIN32
	if ((hFile=_tfopen(filename,_T("rb")))==NULL)  return false;	// For UNICODE support
#else
	if ((hFile=fopen(filename,"rb"))==NULL)  return false;
#endif

	bOK = Decode(hFile,CXIMAGE_FORMAT_UNKNOWN);
	fclose(hFile);
	*/

	if (!bOK && imagetype > 0)
		strcpy(info.szLastError, szError); //restore the first error

	return bOK;
}

////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
//bool CxImage::Load(LPCWSTR filename, uint32_t imagetype)
//{
//	/*FILE* hFile;	//file handle to read the image
//	if ((hFile=_wfopen(filename, L"rb"))==NULL)  return false;
//	bool bOK = Decode(hFile,imagetype);
//	fclose(hFile);*/
//
//	/* automatic file type recognition */
//	bool bOK = false;
//	if ( GetTypeIndexFromId(imagetype) ){
//		FILE* hFile;	//file handle to read the image
//		if ((hFile=_wfopen(filename,L"rb"))==NULL)  return false;
//		bOK = Decode(hFile,imagetype);
//		fclose(hFile);
//		if (bOK) return bOK;
//	}
//
//	char szError[256];
//	strcpy(szError,info.szLastError); //save the first error
//
//	// if failed, try automatic recognition of the file...
//	FILE* hFile;	//file handle to read the image
//	if ((hFile=_wfopen(filename,L"rb"))==NULL)  return false;
//	bOK = Decode(hFile,CXIMAGE_FORMAT_UNKNOWN);
//	fclose(hFile);
//
//	if (!bOK && imagetype > 0) strcpy(info.szLastError,szError); //restore the first error
//
//	return bOK;
//}
////////////////////////////////////////////////////////////////////////////////
/**
 * Loads an image from the application resources.
 * \param hRes: the resource handle returned by FindResource().
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS.
 * \param hModule: NULL for internal resource, or external application/DLL hinstance returned by LoadLibray.
 * \return true if everything is ok
 */
bool CxImage::LoadResource(HRSRC hRes, uint32_t imagetype, HMODULE hModule)
{
	uint32_t rsize = SizeofResource(hModule, hRes);
	HGLOBAL hMem = ::LoadResource(hModule, hRes);
	if (hMem)
	{
		auto lpVoid = static_cast<char*>(LockResource(hMem));
		if (lpVoid)
		{
			// FILE* fTmp=tmpfile(); doesn't work with network
			/*char tmpPath[MAX_PATH] = {0};
			char tmpFile[MAX_PATH] = {0};
			GetTempPath(MAX_PATH,tmpPath);
			GetTempFileName(tmpPath,"IMG",0,tmpFile);
			FILE* fTmp=fopen(tmpFile,"w+b");
			if (fTmp){
				fwrite(lpVoid,rsize,1,fTmp);
				fseek(fTmp,0,SEEK_SET);
				bool bOK = Decode(fTmp,imagetype);
				fclose(fTmp);
				DeleteFile(tmpFile);
				return bOK;
			}*/

			CxMemFile fTmp((uint8_t*)lpVoid, rsize);
			return Decode(&fTmp, imagetype);
		}
	}
	else strcpy(info.szLastError, "Unable to load resource!");
	return false;
}
#endif //WIN32
////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor from file name, see Load()
 * \param filename: file name
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 */
// 
// > filename: file name
// > imagetype: specify the image format (CXIMAGE_FORMAT_BMP,...)
// For UNICODE support: char -> char
//CxImage::CxImage(const char * filename, uint32_t imagetype)
CxImage::CxImage(const std::wstring& ws, uint32_t imagetype)
{
	const std::string s(ws.begin(), ws.end());

	Startup(imagetype);
	Load(s.c_str(), imagetype);
}

////////////////////////////////////////////////////////////////////////////////
/**
* Constructor from file name, see Load()
* \param filename: file name
* \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
*/
// 
// > filename: file name
// > imagetype: specify the image format (CXIMAGE_FORMAT_BMP,...)
// For UNICODE support: char -> char
//CxImage::CxImage(const char * filename, uint32_t imagetype)
CxImage::CxImage(const std::wstring& ws, uint32_t imagetype, const bool& header)
{
	const std::string s(ws.begin(), ws.end());
	Startup(imagetype);
	if (header)
		info.nEscape = -1;
	Load(s.c_str(), imagetype);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor from file handle, see Decode()
 * \param stream: file handle, with read access.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 */
CxImage::CxImage(FILE* stream, uint32_t imagetype)
{
	Startup(imagetype);
	Decode(stream, imagetype);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor from CxFile object, see Decode()
 * \param stream: file handle (CxMemFile or CxIOFile), with read access.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 */
CxImage::CxImage(CxFile* stream, uint32_t imagetype)
{
	Startup(imagetype);
	Decode(stream, imagetype);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor from memory buffer, see Decode()
 * \param buffer: memory buffer
 * \param size: size of buffer
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 */
CxImage::CxImage(uint8_t* buffer, uint32_t size, uint32_t imagetype)
{
	Startup(imagetype);
	CxMemFile stream(buffer, size);
	Decode(&stream, imagetype);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Loads an image from memory buffer
 * \param buffer: memory buffer
 * \param size: size of buffer
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 */
bool CxImage::Decode(uint8_t* buffer, uint32_t size, uint32_t imagetype)
{
	CxMemFile file(buffer, size);
	return Decode(&file, imagetype);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Loads an image from file handle.
 * \param hFile: file handle, with read access.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 */
bool CxImage::Decode(FILE* hFile, uint32_t imagetype)
{
	CxIOFile file(hFile);
	return Decode(&file, imagetype);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Loads an image from CxFile object
 * \param hFile: file handle (CxMemFile or CxIOFile), with read access.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 * \sa ENUM_CXIMAGE_FORMATS
 */
bool CxImage::Decode(CxFile* hFile, uint32_t imagetype)
{
	if (hFile == nullptr)
	{
		strcpy(info.szLastError,CXIMAGE_ERR_NOFILE);
		return false;
	}

	uint32_t pos = hFile->Tell();

#if CXIMAGE_SUPPORT_BMP
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_BMP==imagetype){
		CxImageBMP *newima = new CxImageBMP;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_JPG
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_JPG==imagetype){
		CxImageJPG *newima = new CxImageJPG;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_ICO
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_ICO==imagetype){
		CxImageICO *newima = new CxImageICO;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			info.nNumFrames = newima->info.nNumFrames;
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_GIF
	if (CXIMAGE_FORMAT_UNKNOWN == imagetype || CXIMAGE_FORMAT_GIF == imagetype)
	{
		auto newima = new CxImageGIF;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile))
		{
			Transfer(*newima);
			delete newima;
			return true;
		}
		info.nNumFrames = newima->info.nNumFrames;
		strcpy(info.szLastError, newima->GetLastError());
		hFile->Seek(pos,SEEK_SET);
		delete newima;
		if (CXIMAGE_FORMAT_UNKNOWN != imagetype)
			return false;
	}
#endif
#if CXIMAGE_SUPPORT_PNG
	if (CXIMAGE_FORMAT_UNKNOWN == imagetype || CXIMAGE_FORMAT_PNG == imagetype)
	{
		auto newima = new CxImagePNG;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile))
		{
			Transfer(*newima);
			delete newima;
			return true;
		}
		strcpy(info.szLastError, newima->GetLastError());
		hFile->Seek(pos,SEEK_SET);
		delete newima;
		if (CXIMAGE_FORMAT_UNKNOWN != imagetype)
			return false;
	}
#endif
#if CXIMAGE_SUPPORT_TIF
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_TIF==imagetype){
		CxImageTIF *newima = new CxImageTIF;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			info.nNumFrames = newima->info.nNumFrames;
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_MNG
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_MNG==imagetype){
		CxImageMNG *newima = new CxImageMNG;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			info.nNumFrames = newima->info.nNumFrames;
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_TGA
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_TGA==imagetype){
		CxImageTGA *newima = new CxImageTGA;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_PCX
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_PCX==imagetype){
		CxImagePCX *newima = new CxImagePCX;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_WBMP
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_WBMP==imagetype){
		CxImageWBMP *newima = new CxImageWBMP;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_WMF && CXIMAGE_SUPPORT_WINDOWS
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_WMF==imagetype){
		CxImageWMF *newima = new CxImageWMF;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_JBG
	if (CXIMAGE_FORMAT_UNKNOWN == imagetype || CXIMAGE_FORMAT_JBG == imagetype)
	{
		auto newima = new CxImageJBG;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile))
		{
			Transfer(*newima);
			delete newima;
			return true;
		}
		strcpy(info.szLastError, newima->GetLastError());
		hFile->Seek(pos,SEEK_SET);
		delete newima;
		if (CXIMAGE_FORMAT_UNKNOWN != imagetype)
			return false;
	}
#endif
#if CXIMAGE_SUPPORT_JASPER
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype ||
#if	CXIMAGE_SUPPORT_JP2
	 CXIMAGE_FORMAT_JP2==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_JPC
	 CXIMAGE_FORMAT_JPC==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_PGX
	 CXIMAGE_FORMAT_PGX==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_PNM
	 CXIMAGE_FORMAT_PNM==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_RAS
	 CXIMAGE_FORMAT_RAS==imagetype || 
#endif
	 false ){
		CxImageJAS *newima = new CxImageJAS;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_SKA
	if (CXIMAGE_FORMAT_UNKNOWN == imagetype || CXIMAGE_FORMAT_SKA == imagetype)
	{
		auto newima = new CxImageSKA;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile))
		{
			Transfer(*newima);
			delete newima;
			return true;
		}
		strcpy(info.szLastError, newima->GetLastError());
		hFile->Seek(pos,SEEK_SET);
		delete newima;
		if (CXIMAGE_FORMAT_UNKNOWN != imagetype)
			return false;
	}
#endif
#if CXIMAGE_SUPPORT_RAW
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_RAW==imagetype){
		CxImageRAW *newima = new CxImageRAW;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_PSD
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_PSD==imagetype){
		CxImagePSD *newima = new CxImagePSD;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif

	strcpy(info.szLastError, "Decode: Unknown or wrong format");
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Loads an image from CxFile object
 * \param hFile: file handle (CxMemFile or CxIOFile), with read access.
 * \param imagetype: file format, default = 0 (CXIMAGE_FORMAT_UNKNOWN)
 * \return : if imagetype is not 0, the function returns true when imagetype
 *  matches the file image format. If imagetype is 0, the function returns true
 *  when the file image format is recognized as a supported format.
 *  If the returned value is true, use GetHeight(), GetWidth() or GetType()
 *  to retrieve the basic image information.
 * \sa ENUM_CXIMAGE_FORMATS
 */
bool CxImage::CheckFormat(CxFile* hFile, uint32_t imagetype)
{
	SetType(CXIMAGE_FORMAT_UNKNOWN);
	SetEscape(-1);

	if (!Decode(hFile, imagetype))
		return false;

	if (GetType() == CXIMAGE_FORMAT_UNKNOWN ||
		((imagetype != CXIMAGE_FORMAT_UNKNOWN) && (GetType() != imagetype)))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool CxImage::CheckFormat(uint8_t* buffer, uint32_t size, uint32_t imagetype)
{
	if (buffer == nullptr || size == NULL)
	{
		strcpy(info.szLastError, "invalid or empty buffer");
		return false;
	}
	CxMemFile file(buffer, size);
	return CheckFormat(&file, imagetype);
}

////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_EXIF
bool CxImage::GetExifThumbnail(const char* filename, const char* outname, int32_t type)
{
	switch (type)
	{
#if CXIMAGE_SUPPORT_RAW
	case CXIMAGE_FORMAT_RAW:
	{
		CxImageRAW image;
  		return image.GetExifThumbnail(filename, outname, type);
	}
#endif //CXIMAGE_SUPPORT_RAW
#if CXIMAGE_SUPPORT_JPG
	case CXIMAGE_FORMAT_JPG:
	{
  		CxImageJPG image;
  		return image.GetExifThumbnail(filename, outname, type);
	}
#endif //CXIMAGE_SUPPORT_JPG
	default:
		return false;
	}
}
#endif //CXIMAGE_SUPPORT_EXIF

////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
