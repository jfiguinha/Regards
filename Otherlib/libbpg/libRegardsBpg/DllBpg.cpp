// DllFormatBpg.cpp : définit les fonctions exportées pour l'application DLL.
//
#include "DllBpg.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>

extern "C" {
#include <libbpg.h>
#include "bpgdec.h"
#include "bpgenc.h"
}


DLLFORMATBPG_API int BPG_GetDimensions(uint8_t* buf, size_t buf_len, int& width, int& height)
{
	printf("BPG_GetDimensions \n");
	int returnValue = 0;
	BPGDecoderContext* img;
	BPGImageInfo img_info_s;
	if (buf_len > 0 && buf != nullptr)
	{
		int localWidth = 0;
		int localHeight = 0;
		printf("bpg_decoder_open \n");
		img = bpg_decoder_open();
		printf("bpg_decoder_decodeheader \n");
		if (bpg_decoder_decodeheader(img, buf, buf_len, &localWidth, &localHeight) <= 0)
		{
			printf("bpg_decoder_decodeheader Error \n");
			//bpg_decoder_get_info(img, &img_info_s);
			width = 0;
			height = 0;
			returnValue = -1;
		}
		else
		{
			printf("bpg_decoder_decodeheader OK %d %d \n", localWidth, localHeight);
			width = localWidth;
			height = localHeight;
		}
		bpg_decoder_close(img);
	}
	return returnValue;
}


//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
DLLFORMATBPG_API int BPG_GetPictureRGBA(uint8_t* buf, size_t buf_len, uint8_t* data, size_t data_len, int& width,
                                        int& height, bool flip)
{
	BPGDecoderContext* img;
	BPGImageInfo img_info_s;
	int returnValue = 0;
	if (buf_len > 0 && buf != nullptr)
	{
		img = bpg_decoder_open();

		if (bpg_decoder_decode(img, buf, buf_len) >= 0)
		{
			bpg_decoder_get_info(img, &img_info_s);

			width = img_info_s.width;
			height = img_info_s.height;
			int pictureSize = width * height * 4;
			if (data != nullptr && pictureSize == data_len)
			{
				BPGDecoderOutputFormat out_fmt;
				int strideLine = 4 * width;

				//if (img_info_s.has_alpha)
				out_fmt = BPG_OUTPUT_FORMAT_RGBA32;
				/*
			else
			{
				strideLine = ((((width * 24) + 31) & ~31) >> 3);
				out_fmt = BPG_OUTPUT_FORMAT_RGB24;
			}*/
				bpg_decoder_start(img, out_fmt);

				//int bpp = (3 + img_info_s.has_alpha) * (img_info_s.bit_depth / 8);

				//Calcul stride

				auto rgb_line = new uint8_t[strideLine];


				for (auto y = 0; y < height; y++)
				{
					int returnValue = bpg_decoder_get_line(img, rgb_line);
					if (!flip)
					{
						memcpy(data + (strideLine * y), rgb_line, strideLine);
					}
					else
					{
#pragma omp parallel
						for (auto x = 0; x < width; x++)
						{
							int position = (((flip ? height - y - 1 : y)) * 4 * width) + x * 4;
							memcpy(data + position, rgb_line + (x * 4), 4 * sizeof(uint8_t));
						}
					}
				}

				delete[] rgb_line;
			}
			else
				returnValue = -1;
		}
		else
			returnValue = -1;
		bpg_decoder_close(img);
	}
	else
		returnValue = -1;
	return returnValue;
}


//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
DLLFORMATBPG_API int BPG_GetPictureBGRA(uint8_t* buf, size_t buf_len, uint8_t* data, size_t data_len, int& width,
                                        int& height, bool flip)
{
	BPGDecoderContext* img;
	BPGImageInfo img_info_s;
	int returnValue = 0;
	printf("BPG_GetPictureBGRA \n");
	if (buf_len > 0 && buf != nullptr)
	{
		printf("bpg_decoder_open \n");
		img = bpg_decoder_open();

		printf("bpg_decoder_decode \n");
		if (bpg_decoder_decode(img, buf, buf_len) >= 0)
		{
			printf("bpg_decoder_get_info \n");
			bpg_decoder_get_info(img, &img_info_s);

			width = img_info_s.width;
			height = img_info_s.height;

			printf("bpg_decoder_get_info %d %d \n", width, height);
			int pictureSize = width * height * 4;
			if (data != nullptr && pictureSize == data_len)
			{
				BPGDecoderOutputFormat out_fmt;
				int strideLine = 4 * width;
				out_fmt = BPG_OUTPUT_FORMAT_RGBA32;
				bpg_decoder_start(img, out_fmt);

				//int bpp = (3 + img_info_s.has_alpha) * (img_info_s.bit_depth / 8);

				//Calcul stride

				auto rgb_line = new uint8_t[strideLine];


				for (auto y = 0; y < height; y++)
				{
					int returnValue = bpg_decoder_get_line(img, rgb_line);

#pragma omp parallel
					for (auto x = 0; x < width; x++)
					{
						int position = (((flip ? height - y - 1 : y)) * 4 * width) + x * 4;
						data[position + 2] = rgb_line[x * 4];
						data[position + 1] = rgb_line[x * 4 + 1];
						data[position] = rgb_line[x * 4 + 2];
						data[position + 3] = rgb_line[x * 4 + 3];
					}
				}

				delete[] rgb_line;
			}
			else
				returnValue = -1;
		}
		else
			returnValue = -1;
		bpg_decoder_close(img);
	}
	else
		returnValue = -1;
	return returnValue;
}

DLLFORMATBPG_API int BPG_SavePNGPicture(uint8_t* buf, size_t buf_len, int compress_level, int lossless_mode,
                                        int bit_depth, const char* filename)
{
	return SavePNGPicture(buf, buf_len, compress_level, lossless_mode, bit_depth, filename);
}

DLLFORMATBPG_API uint8_t* BPG_ReadToPNGPicture(uint8_t* buf, size_t buf_len, size_t* buf_out, int bit_depth,
                                               int* returnValue)
{
	return ReadToPNGPicture(buf, buf_len, buf_out, bit_depth, returnValue);
}
