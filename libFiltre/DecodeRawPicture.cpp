#include <header.h>
#include "DecodeRawPicture.h"
#include <libraw/libraw.h>
#include <DecodeRawParameter.h>
#include <ImageLoadingFormat.h>
#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }
#include <ximage.h>
using namespace Regards::Filter;

CDecodeRawPicture::CDecodeRawPicture(const string& fileName)
{
	rawProcessor = new LibRaw();
	result = rawProcessor->open_file(fileName.c_str());
	if(result == LIBRAW_SUCCESS)
	{
		// step two: positioning libraw_internal_data.unpacker_data.data_offset
		result = rawProcessor->unpack();
	}

}


CDecodeRawPicture::~CDecodeRawPicture()
{
	if(rawProcessor != nullptr)
	{
		//rawProcessor->recycle();
		delete rawProcessor;
	}
}


CImageLoadingFormat * CDecodeRawPicture::DecodePicture(CDecodeRawParameter * decodeRawParameter)
{

	rawProcessor->imgdata.params.bright = decodeRawParameter->bright;
	rawProcessor->imgdata.params.highlight = decodeRawParameter->highlight;
	rawProcessor->imgdata.params.threshold = decodeRawParameter->threshold;
	rawProcessor->imgdata.params.use_auto_wb = decodeRawParameter->use_auto_wb;
	rawProcessor->imgdata.params.use_camera_wb = decodeRawParameter->use_camera_wb;
	rawProcessor->imgdata.params.use_rawspeed = 1;
	
	try
	{
		result = rawProcessor->dcraw_process();
	}
	catch (...)
	{

	}

	int width = 0;
	int height = 0;


	CImageLoadingFormat * imageLoadingFormat = nullptr;
	CxImage * image = new CxImage();
	if(result == 0)
	{
		imageLoadingFormat = new CImageLoadingFormat();
		int raw_color, raw_bitsize;
		rawProcessor->get_mem_image_format(&width, &height, &raw_color, &raw_bitsize);
		image->Create(width,height,raw_bitsize*raw_color);

		int iTaille = raw_color * (raw_bitsize/8);
		int stride = ((iTaille * width + iTaille) & ~iTaille);
		//rawProcessor->copy_mem_image(image->GetBits(), width * raw_color * (raw_bitsize/8), 1);
		rawProcessor->copy_mem_image(image->GetBits(), stride, 1);
		image->Flip();
		imageLoadingFormat->SetPicture(image);
	}

	return imageLoadingFormat;
}