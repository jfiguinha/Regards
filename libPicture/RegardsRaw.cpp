#include <header.h>
#include <FreeImage.h>
#include "RegardsRaw.h"
#include "libraw/libraw.h"
#include <ximage.h>
#include <vector>
#include <ConvertUtility.h>
#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }
using namespace Regards::Picture;


void write_ppm(
    const libraw_processed_image_t* image,
    std::vector<uint8_t>& output)
{
    if (image == nullptr)
        return;

    // write_ppm only supports bitmap RGB images.
    if (image->type != LIBRAW_IMAGE_BITMAP ||
        image->colors != 3 ||
        image->data == nullptr ||
        image->data_size == 0)
    {
        return;
    }

    // PPM P6 header.
    char header[64];

    const int headerSize = std::snprintf(
        header,
        sizeof(header),
        "P6\n%d %d\n%d\n",
        image->width,
        image->height,
        (1 << image->bits) - 1);

    if (headerSize <= 0 ||
        static_cast<size_t>(headerSize) >= sizeof(header))
    {
        return;
    }

    output.reserve(
        static_cast<size_t>(headerSize) +
        image->data_size);

    output.insert(
        output.end(),
        header,
        header + headerSize);

    // LibRaw stores 16-bit pixel data in host byte order.
    // PPM expects the 16-bit values in network byte order.
    if (image->bits == 16 &&
        htons(0x55AA) != 0x55AA)
    {
        output.reserve(
            output.size() + image->data_size);

        for (size_t i = 0; i + 1 < image->data_size; i += 2)
        {
            output.push_back(image->data[i + 1]);
            output.push_back(image->data[i]);
        }

        // Handle an unexpected odd byte count safely.
        if ((image->data_size & 1U) != 0)
            output.push_back(image->data[image->data_size - 1]);
    }
    else
    {
        output.insert(
            output.end(),
            image->data,
            image->data + image->data_size);
    }
}

std::vector<uint8_t> CRegardsRaw::GetThumbnail(const string& fileName, int& outputFormat)
{
	std::vector<uint8_t> data;
    auto rawProcessor = std::make_unique<LibRaw>();
	int ret; //, output_thumbs = 0;
	outputFormat = BITMAPOUTPUT;
	if (rawProcessor->open_file(CConvertUtility::ConvertToStdString(fileName).c_str()) == LIBRAW_SUCCESS)
	{
		if (rawProcessor->unpack_thumb() == LIBRAW_SUCCESS)
		{
			libraw_processed_image_t* thumb = rawProcessor->dcraw_make_mem_thumb(&ret);
			if (thumb)
			{
				if (thumb->type == LIBRAW_IMAGE_JPEG)
				{
					data.resize(thumb->data_size);
					memcpy(&data[0], thumb->data, thumb->data_size);
					outputFormat = JPEGOUTPUT;
				}
				else if (thumb->type == LIBRAW_IMAGE_BITMAP)
				{
					outputFormat = BITMAPOUTPUT;
					write_ppm(thumb, data);
				}
                rawProcessor->dcraw_clear_mem(thumb);
			}
			else
				outputFormat = NOTHUMBNAIL;

           
		}
	}

	return data;
}

#undef SWAP

void CRegardsRaw::GetDimensions(const string& fileName, int& width, int& height)
{
	// step one: Open file
    auto rawProcessor = std::make_unique<LibRaw>();
	if (rawProcessor->open_file(fileName.c_str()) == LIBRAW_SUCCESS)
	{
		width = rawProcessor->imgdata.sizes.iwidth;
		height = rawProcessor->imgdata.sizes.iheight;
	}

}

int CRegardsRaw::GetOrientation(const string& fileName)
{
	// step one: Open file
	int flip = 0;
    auto rawProcessor = std::make_unique<LibRaw>();
	if (rawProcessor->open_file(fileName.c_str()) == LIBRAW_SUCCESS)
	{
		flip = rawProcessor->imgdata.sizes.flip;
	}

	return flip;
}
