#include "LoadPicture.h"
#include "utils.h"

array2d<rgb_pixel> CLoadPicture::LoadPictureFromJpegBuffer(const unsigned char * data, const int &size)
{
	array2d<rgb_pixel> img;
	load_jpeg(img, data, size);
	return img;
}

array2d<rgb_pixel> CLoadPicture::LoadPicture(const char * filename)
{
	array2d<rgb_pixel> img;
	load_image(img, filename);
	return img;
}

array2d<rgb_pixel> CLoadPicture::LoadPicture(uint8_t * data, const int &width, const int &height)
{
	array2d<rgb_pixel> img;
	copy_to_dlib_24bit_image(data, img, width, height);
	return img;
}
