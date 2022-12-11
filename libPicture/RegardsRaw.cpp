#include <header.h>
#include <freeimage.h>
#include "RegardsRaw.h"
#include "libraw/libraw.h"
#include <ximage.h>
#include <vector>
#include <ConvertUtility.h>
#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }
using namespace Regards::Picture;

CRegardsRaw::CRegardsRaw()
{
}


CRegardsRaw::~CRegardsRaw()
{
}

// no error reporting, only params check
void write_ppm(libraw_processed_image_t* img, std::vector<uint8_t>* p)
{
	if (!img)
		return;
	// type SHOULD be LIBRAW_IMAGE_BITMAP, but we'll check
	if (img->type != LIBRAW_IMAGE_BITMAP)
		return;
	// only 3-color images supported...
	if (img->colors != 3)
		return;


	char buffer[512];
	int size = sprintf(buffer, "P6\n%d %d\n%d\n", img->width, img->height, (1 << img->bits) - 1);
	p->insert(p->end(), buffer, buffer + size);

	/*
	 NOTE:
	 data in img->data is not converted to network byte order.
	 So, we should swap values on some architectures for dcraw compatibility
	 (unfortunately, xv cannot display 16-bit PPMs with network byte order data
	 */
#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }
	if (img->bits == 16 && htons(0x55aa) != 0x55aa)
		for (unsigned i = 0; i < img->data_size; i += 2)
			SWAP(img->data[i], img->data[i + 1]);
#undef SWAP

	p->insert(p->end(), img->data, img->data + img->data_size);
}



DataStorage* CRegardsRaw::GetThumbnail(const string& fileName, int& outputFormat)
{
	DataStorage * memPicture = new DataStorage();
	auto RawProcessor = new LibRaw;
	int ret; //, output_thumbs = 0;
	outputFormat = BITMAPOUTPUT;
	if (RawProcessor->open_file(CConvertUtility::ConvertToUTF8(fileName)) == LIBRAW_SUCCESS)
	{
		if (RawProcessor->unpack_thumb() == LIBRAW_SUCCESS)
		{
			libraw_processed_image_t* thumb = RawProcessor->dcraw_make_mem_thumb(&ret);
			if (thumb)
			{
				if (thumb->type == LIBRAW_IMAGE_JPEG)
				{
					memPicture->dataPt = new uint8_t[thumb->data_size];
					memcpy(memPicture->dataPt, thumb->data, thumb->data_size);
					outputFormat = JPEGOUTPUT;
					memPicture->size = thumb->data_size;
				}
				else if (thumb->type == LIBRAW_IMAGE_BITMAP)
				{
					std::vector<uint8_t> data;
					outputFormat = BITMAPOUTPUT;
					write_ppm(thumb, &data);
					memPicture->dataPt = new uint8_t[data.size()];
					memPicture->size = data.size();
					memcpy(memPicture->dataPt, &data[0], data.size());
				}
			}
			else
				outputFormat = NOTHUMBNAIL;
		}
	}
	delete RawProcessor;
	return memPicture;
}

#undef SWAP

void CRegardsRaw::GetDimensions(const string& fileName, int& width, int& height)
{
	// step one: Open file
	auto RawProcessor = new LibRaw;
	if (RawProcessor->open_file(fileName.c_str()) == LIBRAW_SUCCESS)
	{
		width = RawProcessor->imgdata.sizes.iwidth;
		height = RawProcessor->imgdata.sizes.iheight;
		//RawProcessor.recycle();
	}
	delete RawProcessor;
}
