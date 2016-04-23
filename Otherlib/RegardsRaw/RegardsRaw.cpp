#include "RegardsRaw.h"
#include <libraw.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
using namespace std;
#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }

#define EXPORT __attribute__((visibility("default")))

CRegardsRaw::CRegardsRaw()
{
}


CRegardsRaw::~CRegardsRaw()
{
}

// no error reporting, only params check
void write_ppm(libraw_processed_image_t *img, std::vector<uint8_t> *p)
{

    
    if(!img)
        return;
    // type SHOULD be LIBRAW_IMAGE_BITMAP, but we'll check
    if(img->type != LIBRAW_IMAGE_BITMAP)
        return;
    // only 3-color images supported...
    if(img->colors != 3)
        return;

    
    char buffer[512];
    int size = sprintf (buffer, "P6\n%d %d\n%d\n", img->width, img->height, (1 << img->bits)-1);
    p->insert(p->end(), buffer, buffer + size);

    /*
     NOTE:
     data in img->data is not converted to network byte order.
     So, we should swap values on some architectures for dcraw compatibility
     (unfortunately, xv cannot display 16-bit PPMs with network byte order data
     */
#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }
    if (img->bits == 16 && htons(0x55aa) != 0x55aa)
        for(unsigned i=0; i< img->data_size; i+=2)
            SWAP(img->data[i],img->data[i+1]);
#undef SWAP
    
    p->insert(p->end(), (uint8_t *)img->data, img->data + img->data_size);
}


std::vector<uint8_t> CRegardsRaw::GetThumbnail(const string & fileName, int &outputFormat)
{
    //int i = 1;
	//int tempimg, row, col;
	std::vector<uint8_t> memPicture;
	// step one: Open file
	LibRaw RawProcessor;
	int  ret;//, output_thumbs = 0;

	if (RawProcessor.open_file(fileName.c_str()) == LIBRAW_SUCCESS)
	{
			if (RawProcessor.unpack_thumb() == LIBRAW_SUCCESS)
			{
				libraw_processed_image_t *thumb = RawProcessor.dcraw_make_mem_thumb(&ret);
				if (thumb)
				{
					if (thumb->type == LIBRAW_IMAGE_JPEG)
					{
                        outputFormat = JPEGOUTPUT;
                        memPicture.insert(memPicture.end(), (uint8_t *)thumb->data, thumb->data + thumb->data_size);

					}
                    else if (thumb->type == LIBRAW_IMAGE_BITMAP)
                    {
                        outputFormat = BITMAPOUTPUT;
                        write_ppm(thumb, &memPicture);
                    }
				}
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
		// step two: positioning libraw_internal_data.unpacker_data.data_offset
		if (RawProcessor.unpack() != LIBRAW_SUCCESS)
		{
			#define S RawProcessor.imgdata.sizes
			width = S.iwidth;
			height = S.iheight;
		}
	}

}

std::vector<uint8_t> CRegardsRaw::GetPicture(const string & fileName, int & width, int & height)
{
    std::vector<uint8_t> data;
	// step one: Open file
	LibRaw RawProcessor;

    //RawProcessor.imgdata.params.use_rawspeed = 1;
	if (RawProcessor.open_file(fileName.c_str()) == LIBRAW_SUCCESS)
	{
		// step two: positioning libraw_internal_data.unpacker_data.data_offset
		if (RawProcessor.unpack() == LIBRAW_SUCCESS)
		{
			RawProcessor.dcraw_process();
			int raw_color, raw_bitsize;
			RawProcessor.get_mem_image_format(&width, &height, &raw_color, &raw_bitsize);
			unsigned char *buffer = new unsigned char[height * width * 3];
			RawProcessor.copy_mem_image(buffer, width * 3, 0);



	#pragma omp parallel for
			for (int y = height - 1; y >= 0; y--)
			{
				for (int x = 0; x < width; x++)
				{
					int positionIn = width * y * 3 + x * 3;
					data.push_back(buffer[positionIn + 2]);
					data.push_back(buffer[positionIn + 1]);
					data.push_back(buffer[positionIn]);
					data.push_back(0);
				}
			}
			// mode coding, not finished yet...
			//image->VertFlipBuf();
			RawProcessor.recycle();

			delete[] buffer;
		}
	}
	
	return data;

}