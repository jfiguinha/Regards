#include <header.h>
#include "raw.h"
#include "RegardsRaw.h"
#include <ximage.h>
#include <ImageLoadingFormat.h>
#include <libraw/libraw.h>
#ifdef TURBOJPEG
#include <turbojpeg.h>
#endif
#include <ConvertUtility.h>
#include <RegardsBitmap.h>
using namespace Regards::Picture;

CImageLoadingFormat* CRaw::GetThumbnail(const wxString& fileName, const bool& thumbnail, bool& isFromExif)
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	CImageLoadingFormat* picture;
	int type = 0;
	CxMemFile* memFile = CRegardsRaw::GetThumbnail(CConvertUtility::ConvertToStdString(fileName), type);
	if (memFile != nullptr)
	{
		if (type == JPEGOUTPUT)
		{
			isFromExif = true;
			picture = new CImageLoadingFormat();
#ifdef TURBOJPEG

			long unsigned int _jpegSize; //!< _jpegSize from above
			//unsigned char* _compressedImage; //!< _compressedImage from above
			CRegardsBitmap* image;
			int jpegSubsamp, width, height;
			uint8_t* _compressedImage = memFile->GetBuffer(false);
			_jpegSize = memFile->Size();

			tjhandle _jpegDecompressor = tjInitDecompress();

			tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);

			image = new CRegardsBitmap(width, height);

			//if (thumbnail)
			//	tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, image->GetPtBitmap(), width, 0, height, TJPF_RGBX, TJFLAG_FASTDCT);
			//else
			tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, image->GetPtBitmap(), width, 0, height,
			              TJPF_BGRX, TJFLAG_FASTDCT | TJFLAG_BOTTOMUP);

			tjDestroy(_jpegDecompressor);

			picture->SetPicture(image);
			picture->SetFilename(fileName);
#else
			CxImage * image = new CxImage(memFile, CxImage::GetTypeIdFromName("jpg"));
			picture->SetPicture(image);
			picture->SetFilename(fileName);
#endif
		}
		else
		{
			picture = new CImageLoadingFormat();
			auto image = new CxImage(memFile, CxImage::GetTypeIdFromName("ppm"));
			picture->SetPicture(image);
			picture->SetFilename(fileName);
		}

		delete memFile;
	}
	else
	{
		picture = new CImageLoadingFormat();
		LoadPicture(fileName, picture);
		picture->SetFilename(fileName);
		if (thumbnail)
		{
			picture->ConvertToBGR(true);
		}
	}


	return picture;
}

bool CRaw::LoadPicture(const wxString& fileName, CImageLoadingFormat * imageLoadingFormat)
{
	int result;
	LibRaw* rawProcessor = new LibRaw();
	result = rawProcessor->open_file(fileName.mb_str());
	if (result == LIBRAW_SUCCESS)
	{
		// step two: positioning libraw_internal_data.unpacker_data.data_offset
		result = rawProcessor->unpack();
	}

	rawProcessor->imgdata.params.use_rawspeed = 1;
	rawProcessor->imgdata.params.use_camera_wb = 1;

	try
	{
		result = rawProcessor->dcraw_process();
	}
	catch (...)
	{

	}

	int width = 0;
	int height = 0;
	bool isOk = false;
	CxImage* image = new CxImage();
	if (result == 0)
	{
		int raw_color, raw_bitsize;
		rawProcessor->get_mem_image_format(&width, &height, &raw_color, &raw_bitsize);
		image->Create(width, height, raw_bitsize * raw_color);

		int iTaille = raw_color * (raw_bitsize / 8);
		int stride = ((iTaille * width + iTaille) & ~iTaille);
		rawProcessor->copy_mem_image(image->GetBits(), stride, 1);
		image->Flip();
		imageLoadingFormat->SetPicture(image);
		isOk = true;
	}

	if (rawProcessor != nullptr)
	{
		rawProcessor->recycle();
		delete rawProcessor;
	}

	delete image;
	return isOk;
}


void CRaw::GetDimensions(const wxString& fileName, int& width, int& height)
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	CRegardsRaw::GetDimensions(CConvertUtility::ConvertToStdString(fileName), width, height);
}
