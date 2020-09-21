#include "RegardsRaw.h"
#include "libraw/libraw.h"
#include <ximage.h>
#include <vector>
#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }


CRegardsRaw::CRegardsRaw()
{
}


CRegardsRaw::~CRegardsRaw()
{
}

// no error reporting, only params check
void write_ppm(libraw_processed_image_t *img, std::vector<uint8_t> *p)
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

	p->insert(p->end(), (uint8_t *)img->data, img->data + img->data_size);
}

CxImage * CRegardsRaw::GetPicture(const string & fileName)
{
	LibRaw RawProcessor;
	int  ret;//, output_thumbs = 0;
	CxImage * image = nullptr;

	if (RawProcessor.open_file(fileName.c_str()) == LIBRAW_SUCCESS)
	{
		if ((ret = RawProcessor.unpack()) == LIBRAW_SUCCESS)
		{
			ret = RawProcessor.dcraw_process();
			if (LIBRAW_SUCCESS == ret)
			{
				int width = 0;
				int height = 0;
				image = new CxImage();
				int raw_color, raw_bitsize;
				RawProcessor.get_mem_image_format(&width, &height, &raw_color, &raw_bitsize);
				int flip = RawProcessor.imgdata.sizes.flip;
				image->Create(width, height, raw_bitsize*raw_color);
				int iTaille = raw_color * (raw_bitsize / 8);
				int stride = ((iTaille * width + iTaille) & ~iTaille);
				RawProcessor.copy_mem_image(image->GetBits(), stride, 1);
				image->Flip();

			}
		}
		RawProcessor.recycle();
	}

	return image;
}

CxMemFile * CRegardsRaw::GetThumbnail(const string & fileName, int &outputFormat)
{
	//int i = 1;
	//int tempimg, row, col;
	CxMemFile * memPicture = nullptr;
	// step one: Open file
	LibRaw RawProcessor;
	int  ret;//, output_thumbs = 0;
	outputFormat = BITMAPOUTPUT;
	if (RawProcessor.open_file(fileName.c_str()) == LIBRAW_SUCCESS)
	{
		if (RawProcessor.unpack_thumb() == LIBRAW_SUCCESS)
		{
			libraw_processed_image_t *thumb = RawProcessor.dcraw_make_mem_thumb(&ret);
			if (thumb)
			{
				uint8_t * dataPt = nullptr;
				int size = 0;
				if (thumb->type == LIBRAW_IMAGE_JPEG)
				{
					dataPt = new uint8_t[thumb->data_size];
					memcpy(dataPt, thumb->data, thumb->data_size);
					outputFormat = JPEGOUTPUT;
					size = thumb->data_size;
					//memPicture = new CxMemFile(thumb->data,thumb->data_size);
				}
				else if (thumb->type == LIBRAW_IMAGE_BITMAP)
				{
					std::vector<uint8_t> data;
					outputFormat = BITMAPOUTPUT;
					write_ppm(thumb, &data);
					dataPt = new uint8_t[data.size()];
					size = data.size();
					memcpy(dataPt, &data[0], data.size());
				}
				memPicture = new CxMemFile(dataPt, size);
			}
			else
				outputFormat = NOTHUMBNAIL;
		}
		RawProcessor.recycle();
	}

	return memPicture;

}

#undef SWAP

void CRegardsRaw::GetDimensions(const string & fileName, int & width, int & height)
{
	// step one: Open file
	LibRaw RawProcessor;
	if (RawProcessor.open_file(fileName.c_str()) == LIBRAW_SUCCESS)
	{
#define S RawProcessor.imgdata.sizes
		width = S.iwidth;
		height = S.iheight;
		RawProcessor.recycle();
	}
}