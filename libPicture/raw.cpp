#include <header.h>
#include "raw.h"
#include "RegardsRaw.h"
#include <FreeImage.h>
#include <ximage.h>
#include <ImageLoadingFormat.h>
#include <libraw/libraw.h>
#include <ConvertUtility.h>
using namespace Regards::Picture;
using namespace Regards::Picture;

CImageLoadingFormat* CRaw::GetThumbnail(const wxString& fileName, const bool& thumbnail, bool& isFromExif)
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	CImageLoadingFormat* picture = new CImageLoadingFormat();
	int type = 0;
    bool isOk = false;
	int orientation = CRegardsRaw::GetOrientation(CConvertUtility::ConvertToStdString(fileName).c_str());
	std::vector<uint8_t> memFile = CRegardsRaw::GetThumbnail(CConvertUtility::ConvertToStdString(fileName).c_str(), type);
	if (!memFile.empty())
	{
		if (type == JPEGOUTPUT)
		{
			wxMemoryInputStream cxMemFile(&memFile[0], memFile.size());
			isFromExif = true;
			
			wxImage jpegImage;
			jpegImage.LoadFile(cxMemFile, wxBITMAP_TYPE_ANY);
			picture->SetPicture(jpegImage);
			picture->SetFilename(fileName);
		}
		else if(type == BITMAPOUTPUT)
		{
			cv::Mat rawData(1, memFile.size(), CV_8UC1, &memFile[0]);
            try
            {
			    cv::Mat matPicture = imdecode(rawData, cv::IMREAD_COLOR);
                if (!matPicture.empty())
			    {
				    picture->SetPicture(matPicture, 0, fileName);
                    picture->SetFilename(fileName);
			    }
            }
            catch (const cv::Exception&)
            {

            }
		}
	}


	if(!picture->IsOk())
	{
		LoadPicture(fileName, picture);
		picture->SetFilename(fileName);
	}

	picture->RotateExif_withoutflip(orientation);
	return picture;
}

bool CRaw::LoadPicture(
    const wxString& fileName,
    CImageLoadingFormat* imageLoadingFormat)
{
    if (imageLoadingFormat == nullptr)
        return false;

    auto rawProcessor = std::make_unique<LibRaw>();

    // Open RAW file.
    int result = rawProcessor->open_file(
        fileName.mb_str().data());

    if (result != LIBRAW_SUCCESS)
        return false;

    // Unpack RAW data.
    result = rawProcessor->unpack();

    if (result != LIBRAW_SUCCESS)
        return false;

    auto& params = rawProcessor->imgdata.params;

    // Default RAW rendering.
    params.use_camera_wb = 1;

    /*
     * Fast decoding.
     *
     * half_size = 1 reduces the output resolution by 2 in
     * both dimensions and considerably reduces the amount
     * of work performed by dcraw_process().
     *
     * Keep it disabled if LoadPicture() must return the
     * full-resolution RAW image.
     */
     // params.half_size = 1;

    try
    {
        result = rawProcessor->dcraw_process();
    }
    catch (...)
    {
        return false;
    }

    if (result != LIBRAW_SUCCESS)
        return false;

    int width = 0;
    int height = 0;
    int rawColor = 0;
    int rawBitsize = 0;

    rawProcessor->get_mem_image_format(
        &width,
        &height,
        &rawColor,
        &rawBitsize);

    if (width <= 0 ||
        height <= 0 ||
        rawColor <= 0 ||
        rawBitsize <= 0)
    {
        return false;
    }

    const int bytesPerPixel =
        rawColor * (rawBitsize / 8);

    if (bytesPerPixel <= 0)
        return false;

    const int stride =
        ((bytesPerPixel * width + bytesPerPixel) &
            ~bytesPerPixel);

    CxImage image;

    if (!image.Create(
        width,
        height,
        rawBitsize * rawColor))
    {
        return false;
    }

    result = rawProcessor->copy_mem_image(
        image.GetBits(),
        stride,
        1);

    if (result != LIBRAW_SUCCESS)
        return false;

    imageLoadingFormat->SetPicture(image);
    imageLoadingFormat->Flip();

    return true;
}




void CRaw::GetDimensions(const wxString& fileName, int& width, int& height)
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	CRegardsRaw::GetDimensions(CConvertUtility::ConvertToStdString(fileName).c_str(), width, height);
}
