#pragma once
#include <stdint.h>
#include <dlib/pixel.h>

void copy_to_dlib_image(uint8_t *src, array2d<rgb_alpha_pixel>& dst, unsigned width, unsigned height)
{
	for (unsigned int i = 0, j = 0; i < width * height; ++i) {
		rgb_alpha_pixel pixel;

		pixel.red = static_cast<unsigned char> (src[j++]);
		pixel.green = static_cast<unsigned char> (src[j++]);
		pixel.blue = static_cast<unsigned char> (src[j++]);
		pixel.alpha = static_cast<unsigned char> (src[j++]);

		unsigned r = std::floor(i / width);
		unsigned c = i % width;

		dst[r][c] = pixel;
	}
}

void copy_to_dlib_24bit_image(uint8_t *src, matrix<rgb_pixel>& dst, unsigned width, unsigned height)
{
	dst.set_size(height, width);

	for (unsigned int i = 0, j = (width * height * 4) - 4; i < width * height; ++i, j -= 4) {
		rgb_pixel pixel;

		pixel.red = static_cast<unsigned char> (src[j + 2]);
		pixel.green = static_cast<unsigned char> (src[j + 1]);
		pixel.blue = static_cast<unsigned char> (src[j]);
		//pixel.alpha = static_cast<unsigned char> ((src[i] >> 24) & 0xFF);

		unsigned r = std::floor(i / width);
		unsigned c = i % width;

		dst(r,c) = pixel;
	}

}

void copy_to_dlib_24bit_image(uint8_t *src, array2d<rgb_pixel>& dst, unsigned width, unsigned height)
{
	dst.set_size(height, width);

	for (unsigned int i = 0, j = (width * height * 4) - 4; i < width * height; ++i, j -= 4) {
		rgb_pixel pixel;

		pixel.red = static_cast<unsigned char> (src[j + 2]);
		pixel.green = static_cast<unsigned char> (src[j + 1]);
		pixel.blue = static_cast<unsigned char> (src[j]);
		//pixel.alpha = static_cast<unsigned char> ((src[i] >> 24) & 0xFF);

		unsigned r = std::floor(i / width);
		unsigned c = i % width;

		dst[r][c] = pixel;
	}

}


void copy_to_dlib_grayscale_image(uint8_t *src, array2d<unsigned char>& dst, unsigned width, unsigned height)
{

    dst.set_size(height, width);

	for (unsigned int i = 0, j = (width * height * 4) - 4; i < width * height; ++i, j -= 4) {
		unsigned char pixel = ((src[j] * 0.299) + (src[j + 1] * 0.587) + (src[j + 2] * 0.114));
		unsigned r = std::floor(i / width);
		unsigned c = i % width;
		dst[r][c] = pixel;
	}

}



void copy_from_dlib_image(matrix<rgb_alpha_pixel> src, uint8_t *dst, unsigned width, unsigned height) {
	unsigned int image_size = width * height;
	for (unsigned int  i = 0, j = (width * height * 4) - 4; i < image_size; ++i, j -= 4) {

		//unsigned r = std::floor(i / width);
		//unsigned c = i % width;
		rgb_alpha_pixel& p = src(i);

		dst[j] = p.blue;
		dst[j + 1] = p.green;
		dst[j + 2] = p.red;
	}
}
