#include <header.h>
#include "raw.h"
#include "RegardsRaw.h"
#include <FreeImage.h>
#include <ximage.h>
#include <ImageLoadingFormat.h>
#include <libraw/libraw.h>
#ifdef TURBOJPEG
#include <turbojpeg.h>
#endif
#include <ConvertUtility.h>
#include "PictureMetadataExiv.h"
using namespace Regards::Picture;
using namespace Regards::exiv2;
CImageLoadingFormat* CRaw::GetThumbnail(const wxString& fileName, const bool& thumbnail, bool& isFromExif)
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	CImageLoadingFormat* picture;
	int type = 0;
	int orientation = CRegardsRaw::GetOrientation(CConvertUtility::ConvertToStdString(fileName));
	DataStorage* memFile = CRegardsRaw::GetThumbnail(CConvertUtility::ConvertToStdString(fileName), type);
	if (memFile != nullptr)
	{
		if (type == JPEGOUTPUT)
		{
			wxMemoryInputStream cxMemFile(memFile->dataPt, memFile->size);
			isFromExif = true;
			picture = new CImageLoadingFormat();
			wxImage jpegImage;
			jpegImage.LoadFile(cxMemFile, wxBITMAP_TYPE_JPEG);
			picture->SetPicture(jpegImage);
			picture->SetFilename(fileName);
			//CPictureMetadataExiv metadata(fileName);
			//orientation = metadata.GetOrientation();
			

		}
		else
		{
			picture = new CImageLoadingFormat();
			cv::Mat rawData(1, memFile->size, CV_8UC1, (void*)memFile->dataPt);
			cv::Mat matPicture = cv::imdecode(rawData, cv::IMREAD_COLOR);
			//cv::flip(matPicture, matPicture, 0);
			picture->SetPicture(matPicture, 0, fileName);
			picture->SetFilename(fileName);
		}
		delete memFile;
	}
	else
	{
		picture = new CImageLoadingFormat();
		LoadPicture(fileName, picture);
		picture->SetFilename(fileName);
	}

	picture->RotateExif_withoutflip(orientation);
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

	//rawProcessor->imgdata.params.use_rawspeed = 1;
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
		//image->Flip();
		imageLoadingFormat->SetPicture(image);
		imageLoadingFormat->Flip();
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
