#include <header.h>
#include <heifreader.h>
using namespace std;
using namespace HEIF;
#include "avif.h"
#include <RegardsBitmap.h>
#include <cstdint>
#include <avif/avif.h>
#include <iostream>
#include <fstream>
using namespace Regards::Picture;

CAvif::CAvif()
{
}

CAvif::~CAvif()
{
}

void LoadDataFromFile(const string& filename, avifRWData& raw)
{
	FILE* f = fopen(filename.c_str(), "rb");
	if (!f)
		return;
	fseek(f, 0, SEEK_END);
	uint32_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	avifRWDataRealloc(&raw, size);
	fread(raw.data, 1, size, f);
	fclose(f);
}

int CAvif::GetDelay(const string& filename)
{
	int delay = 0;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);
	// Decode it
	avifImage* decoded = avifImageCreateEmpty();
	avifDecoder* decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		delay = decoder->duration / decoder->imageCount;
		avifDecoderDestroy(decoder);
	}
	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
	return delay;
}

int CAvif::GetNbFrame(const string& filename)
{
	int nbFrame = 0;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);
	// Decode it
	avifImage* decoded = avifImageCreateEmpty();
	avifDecoder* decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		nbFrame = decoder->imageCount;
		avifDecoderDestroy(decoder);
	}
	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
	return nbFrame;
}


void CAvif::GetPictureDimension(const string& filename, int& width, int& height)
{
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);

	// Decode it
	avifImage* decoded = avifImageCreateEmpty();
	avifDecoder* decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifResult decodeResult = avifDecoderRead(decoder, decoded, (avifROData*)&raw);

		if (decodeResult == AVIF_RESULT_OK)
		{
			width = decoded->width;
			height = decoded->height;
		}

		avifDecoderDestroy(decoder);
	}


	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
}


CRegardsBitmap* CAvif::GetThumbnailPicture(const string& filename)
{
	CRegardsBitmap* picture = nullptr;
	auto* reader = Reader::Create();
	uint64_t itemSize = 1024 * 1024;
	auto itemData = new uint8_t[itemSize];
	Array<ImageId> itemIds;


	if (reader->initialize(filename.c_str()) == ErrorCode::OK)
	{
		FileInformation info;
		reader->getFileInformation(info);

		// Find the item ID
		ImageId itemId;
		reader->getPrimaryItem(itemId);

		// Thumbnail references ('thmb') are from the thumbnail image to the master image
		Array<ImageId> thumbIds;
		reader->getReferencedToItemListByType(itemId, "thmb", thumbIds);
		if (thumbIds.size > 0)
		{
			const ImageId thumbId = *std::min_element(thumbIds.begin(), thumbIds.end(), [&](ImageId a, ImageId b)
			{
				uint32_t widthA, widthB;
				reader->getWidth(a, widthA);
				reader->getWidth(b, widthB);
				return (widthA < widthB);
			});
			//imageMap[masterId] = thumbId;

			if (reader->getItemDataWithDecoderParameters(thumbId.get(), itemData, itemSize) == ErrorCode::OK)
			{
				avifDecoder* decoder = avifDecoderCreate();
				avifRWData raw = AVIF_DATA_EMPTY;
				avifRWDataRealloc(&raw, itemSize);
				memcpy(&raw, itemData, itemSize);
				// Decode it
				avifImage* decoded = avifImageCreateEmpty();
				if (decoder != nullptr)
				{
					avifResult decodeResult = avifDecoderRead(decoder, decoded, (avifROData*)&raw);

					if (decodeResult == AVIF_RESULT_OK)
					{
						avifRGBImage dstRGB;
						avifRGBImageSetDefaults(&dstRGB, decoded);
						dstRGB.format = AVIF_RGB_FORMAT_BGRA; // See choices in avif.h
						dstRGB.depth = 8; // [8, 10, 12, 16]; Does not need to match image->depth.
						avifRGBImageAllocatePixels(&dstRGB);

						if (avifImageYUVToRGB(decoded, &dstRGB) == AVIF_RESULT_OK)
						{
							int image_width = dstRGB.width;
							int image_height = dstRGB.height;
							picture = new CRegardsBitmap(image_width, image_height);
							uint8_t* dataOut = picture->GetPtBitmap();
							memcpy(dataOut, dstRGB.pixels, picture->GetBitmapSize());
							picture->VertFlipBuf();
						}
						avifRGBImageFreePixels(&dstRGB);
					}
					avifDecoderDestroy(decoder);
				}
				avifImageDestroy(decoded);
				avifRWDataFree(&raw);
				/*
				picture = DecodeFrame(itemData, itemSize);
				*/
				if (picture != nullptr)
				{
					picture->SetFilename(filename);
				}
			}
		}
	}
	else
	{
		cout << "Can't find input file: " << filename << ". "
			<< "Please download it from https://github.com/nokiatech/heif_conformance "
			<< "and place it in same directory with the executable." << endl;
	}

	delete[] itemData;
	Reader::Destroy(reader);

	return picture;
}

CRegardsBitmap* CAvif::GetPicture(const string& filename, int& delay, const int& numPicture)
{
	CRegardsBitmap* out = nullptr;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);

	// Decode it
	avifDecoder* decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		int frameIndex = 0;
		delay = decoder->duration / decoder->imageCount; // Duration of entire sequence (seconds)
		for (;;)
		{
			avifResult nextImageResult = avifDecoderNextImage(decoder);
			if (nextImageResult == AVIF_RESULT_NO_IMAGES_REMAINING)
			{
				// No more images, bail out. Verify that you got the expected amount of images decoded.
				break;
			}
			if (nextImageResult != AVIF_RESULT_OK)
			{
				printf("ERROR: Failed to decode all frames: %s\n", avifResultToString(nextImageResult));
				break;
			}
			if (nextImageResult == AVIF_RESULT_OK && numPicture == frameIndex)
			{
				avifRGBImage dstRGB;
				avifRGBImageSetDefaults(&dstRGB, decoder->image);
				dstRGB.format = AVIF_RGB_FORMAT_BGRA; // See choices in avif.h
				dstRGB.depth = 8; // [8, 10, 12, 16]; Does not need to match image->depth.
				avifRGBImageAllocatePixels(&dstRGB);

				if (avifImageYUVToRGB(decoder->image, &dstRGB) == AVIF_RESULT_OK)
				{
					int image_width = dstRGB.width;
					int image_height = dstRGB.height;
					out = new CRegardsBitmap(image_width, image_height);
					uint8_t* dataOut = out->GetPtBitmap();
					memcpy(dataOut, dstRGB.pixels, out->GetBitmapSize());
					out->VertFlipBuf();
				}
				avifRGBImageFreePixels(&dstRGB);
			}
			frameIndex++;
		}
		avifDecoderDestroy(decoder);
	}
	avifRWDataFree(&raw);
	return out;
}




void CAvif::SavePicture(const string& filename, CRegardsBitmap* source, uint8_t* data, const long& size, const int& compression, const bool& hasExif)
{
	if (source != nullptr)
	{
		int width = source->GetBitmapWidth();
		int height = source->GetBitmapHeight();
		int depth = 8;
		avifPixelFormat format = AVIF_PIXEL_FORMAT_YUV420;
		avifImage* image = avifImageCreate(width, height, depth, format);
		if (image != nullptr)
		{
			if(hasExif)
				avifImageSetMetadataExif(image, data, size);
			
			// (Semi-)optional: Describe the color profile, YUV<->RGB conversion, and range.
			// These default to "unspecified" and full range. You should at least set the
			// matrixCoefficients to indicate how you would like YUV<->RGB conversion to be done.
			image->colorPrimaries = AVIF_COLOR_PRIMARIES_BT709;
			image->transferCharacteristics = AVIF_TRANSFER_CHARACTERISTICS_SRGB;
			image->matrixCoefficients = AVIF_MATRIX_COEFFICIENTS_BT709;
			image->yuvRange = AVIF_RANGE_FULL;
			source->VertFlipBuf();
			// Option 2: Convert from interleaved RGB(A)/BGR(A) using a libavif-allocated buffer.
			avifRGBImage rgb;
			avifRGBImageSetDefaults(&rgb, image);
			rgb.depth = 8; // [8, 10, 12, 16]; Does not need to match image->depth.
			rgb.format = AVIF_RGB_FORMAT_BGRA; // See choices in avif.h
			rgb.pixels = source->GetPtBitmap(); // Point at your RGB(A)/BGR(A) pixels here
			rgb.rowBytes = source->GetWidthSize();
			//memcpy(&rgb.pixels, source->GetPtBitmap(), rgb.rowBytes);


			avifImageRGBToYUV(image, &rgb); // if alpha is present, it will also be copied/converted
			avifRGBImageFreePixels(&rgb);

			/*
			// Optional: Set Exif and/or XMP metadata
			uint8_t * exif = ...;  // raw Exif payload
			size_t exifSize = ...; // Length of raw Exif payload
			avifImageSetMetadataExif(image, exif, exifSize);
			*/

			avifRWData output = AVIF_DATA_EMPTY;
			avifEncoder* encoder = avifEncoderCreate();
			encoder->maxThreads = 8; // Choose max encoder threads, 1 to disable multithreading
			encoder->minQuantizer = compression;
			encoder->maxQuantizer = compression;
			avifResult encodeResult = avifEncoderWrite(encoder, image, &output);
			if (encodeResult == AVIF_RESULT_OK)
			{
				ofstream file(filename, std::ofstream::binary);
				file.write((char*)output.data, output.size);
				file.close();
			}
			else
			{
				printf("ERROR: Failed to encode: %s\n", avifResultToString(encodeResult));
			}

			avifImageDestroy(image);
			avifRWDataFree(&output);
			avifEncoderDestroy(encoder);
		}
	}
}


vector<CRegardsBitmap*> CAvif::GetAllPicture(const string& filename, int& delay)
{
	vector<CRegardsBitmap*> listPicture;

	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);

	// Decode it
	avifDecoder* decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		//int frameIndex = 0;
		//avifBool firstImage = AVIF_TRUE;
		delay = decoder->duration / decoder->imageCount; // Duration of entire sequence (seconds)
		for (;;)
		{
			avifResult nextImageResult = avifDecoderNextImage(decoder);
			if (nextImageResult == AVIF_RESULT_NO_IMAGES_REMAINING)
			{
				// No more images, bail out. Verify that you got the expected amount of images decoded.
				break;
			}
			if (nextImageResult != AVIF_RESULT_OK)
			{
				printf("ERROR: Failed to decode all frames: %s\n", avifResultToString(nextImageResult));
				break;
			}
			if (nextImageResult == AVIF_RESULT_OK)
			{
				avifRGBImage dstRGB;
				avifRGBImageSetDefaults(&dstRGB, decoder->image);
				dstRGB.format = AVIF_RGB_FORMAT_BGRA; // See choices in avif.h
				dstRGB.depth = 8; // [8, 10, 12, 16]; Does not need to match image->depth.
				avifRGBImageAllocatePixels(&dstRGB);

				if (avifImageYUVToRGB(decoder->image, &dstRGB) == AVIF_RESULT_OK)
				{
					int image_width = dstRGB.width;
					int image_height = dstRGB.height;
					auto out = new CRegardsBitmap(image_width, image_height);
					uint8_t* dataOut = out->GetPtBitmap();
					memcpy(dataOut, dstRGB.pixels, out->GetBitmapSize());
					out->VertFlipBuf();
					listPicture.push_back(out);
				}
				avifRGBImageFreePixels(&dstRGB);
			}
		}
		avifDecoderDestroy(decoder);
	}
	avifRWDataFree(&raw);
	return listPicture;
}

CRegardsBitmap* CAvif::GetPicture(const string& filename)
{
	CRegardsBitmap* out = nullptr;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);

	// Decode it
	avifImage* decoded = avifImageCreateEmpty();
	avifDecoder* decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifResult decodeResult = avifDecoderRead(decoder, decoded, (avifROData*)&raw);

		if (decodeResult == AVIF_RESULT_OK)
		{
			avifRGBImage dstRGB;
			avifRGBImageSetDefaults(&dstRGB, decoded);
			dstRGB.format = AVIF_RGB_FORMAT_BGRA; // See choices in avif.h
			dstRGB.depth = 8; // [8, 10, 12, 16]; Does not need to match image->depth.
			avifRGBImageAllocatePixels(&dstRGB);

			if (avifImageYUVToRGB(decoded, &dstRGB) == AVIF_RESULT_OK)
			{
				int image_width = dstRGB.width;
				int image_height = dstRGB.height;
				out = new CRegardsBitmap(image_width, image_height);
				uint8_t* dataOut = out->GetPtBitmap();
				memcpy(dataOut, dstRGB.pixels, out->GetBitmapSize());
				out->VertFlipBuf();
			}
			avifRGBImageFreePixels(&dstRGB);
		}
		avifDecoderDestroy(decoder);
	}
	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
	return out;
}

// static
bool CAvif::HasExifMetaData(const string& filename)
{
	bool exifData = false;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);
	avifImage* decoded = avifImageCreateEmpty();
	avifDecoder* decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifDecoderRead(decoder, decoded, (avifROData*)&raw);
		if (decoded->exif.size > 0)
			exifData = true;
		avifDecoderDestroy(decoder);
	}
	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
	return exifData;
}


/* raw JPEG image data */
static const unsigned char image_jpg[] = {
	0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46, 0x49, 0x46, 0x00, 0x01,
	0x01, 0x01, 0x00, 0x48, 0x00, 0x48, 0x00, 0x00, 0xff, 0xdb, 0x00, 0x43,
	0x00, 0x14, 0x0e, 0x0f, 0x12, 0x0f, 0x0d, 0x14, 0x12, 0x10, 0x12, 0x17,
	0x15, 0x14, 0x18, 0x1e, 0x32, 0x21, 0x1e, 0x1c, 0x1c, 0x1e, 0x3d, 0x2c,
	0x2e, 0x24, 0x32, 0x49, 0x40, 0x4c, 0x4b, 0x47, 0x40, 0x46, 0x45, 0x50,
	0x5a, 0x73, 0x62, 0x50, 0x55, 0x6d, 0x56, 0x45, 0x46, 0x64, 0x88, 0x65,
	0x6d, 0x77, 0x7b, 0x81, 0x82, 0x81, 0x4e, 0x60, 0x8d, 0x97, 0x8c, 0x7d,
	0x96, 0x73, 0x7e, 0x81, 0x7c, 0xff, 0xc0, 0x00, 0x0b, 0x08, 0x00, 0x40,
	0x00, 0x40, 0x01, 0x01, 0x11, 0x00, 0xff, 0xc4, 0x00, 0x1b, 0x00, 0x00,
	0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x05, 0x06, 0x04, 0x03, 0x07, 0x02, 0x01, 0xff,
	0xc4, 0x00, 0x2f, 0x10, 0x00, 0x01, 0x03, 0x03, 0x02, 0x05, 0x03, 0x03,
	0x03, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x11,
	0x00, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x81, 0x61, 0x71,
	0x91, 0x13, 0x32, 0xa1, 0x14, 0x22, 0xc1, 0x15, 0x23, 0x52, 0xd1, 0xf0,
	0xff, 0xda, 0x00, 0x08, 0x01, 0x01, 0x00, 0x00, 0x3f, 0x00, 0xb3, 0xa2,
	0x8a, 0x28, 0xa2, 0x8a, 0x28, 0xa2, 0x97, 0x64, 0x72, 0xd6, 0x58, 0xd4,
	0x8f, 0xd5, 0x3d, 0xca, 0xa5, 0x7d, 0xa8, 0x4e, 0xaa, 0x3e, 0xb0, 0x3a,
	0x7a, 0x9d, 0x2b, 0x2d, 0x87, 0x13, 0xe3, 0xaf, 0x9f, 0x0c, 0xb6, 0xb5,
	0xb6, 0xe2, 0xb4, 0x48, 0x71, 0x30, 0x14, 0x7b, 0x02, 0x09, 0x14, 0xee,
	0x94, 0x64, 0x38, 0x87, 0x1f, 0x8d, 0x5f, 0xd3, 0x7d, 0xe2, 0xa7, 0x46,
	0xe8, 0x6c, 0x49, 0x1e, 0xfd, 0x07, 0xb1, 0x33, 0x5f, 0xb8, 0xdc, 0xf5,
	0x96, 0x51, 0x7c, 0x96, 0xee, 0x14, 0xba, 0x04, 0xf2, 0x2c, 0x42, 0xa3,
	0xd3, 0x70, 0x7c, 0x1a, 0x6d, 0x45, 0x2c, 0xcd, 0xe5, 0x11, 0x89, 0xb1,
	0x5b, 0xc4, 0x02, 0xe1, 0xd1, 0xb4, 0xcf, 0xdc, 0xa3, 0xfc, 0x0d, 0xcd,
	0x41, 0xde, 0xe3, 0xb2, 0x2e, 0xda, 0x1c, 0xbd, 0xe2, 0x4a, 0x90, 0xe9,
	0x04, 0x95, 0x2b, 0xf7, 0x41, 0xd0, 0x18, 0xe8, 0x36, 0x03, 0xc6, 0x91,
	0x4a, 0x81, 0x20, 0x82, 0x24, 0x10, 0x66, 0x45, 0x5a, 0xdd, 0xf1, 0x32,
	0xff, 0x00, 0xa2, 0x5a, 0x8b, 0x62, 0x57, 0x7d, 0x70, 0x80, 0x93, 0x1a,
	0x94, 0x10, 0x79, 0x4a,
};
/* length of data in image_jpg */
static const unsigned int image_jpg_len = sizeof(image_jpg);

/* dimensions of image */
//static const unsigned int image_jpg_x = 64;
//static const unsigned int image_jpg_y = 64;

/* start of JPEG image data section */
static const unsigned int image_data_offset = 20;
#define image_data_len (image_jpg_len - image_data_offset)

// static
void CAvif::GetMetadata(const string& filename, uint8_t*& data, unsigned int& size)
{
	//bool exifData = false;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);
	avifImage* decoded = avifImageCreateEmpty();
	avifDecoder* decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifDecoderRead(decoder, decoded, (avifROData*)&raw);
		if (decoded->exif.size > 0)
		{
			if (size > 0)
			{
				//memcpy(data, decoded->exif.data, size);
				/* raw EXIF header data */
				static const unsigned char exif_header[] = {
					0xff, 0xd8, 0xff, 0xe1
				};
				/* length of data in exif_header */
				static const unsigned int exif_header_len = sizeof(exif_header);

				char value = 0;
				int pos = 0;
				memcpy(data, exif_header, exif_header_len);
				pos += exif_header_len;
				value = ((decoded->exif.size + 2) >> 8);
				memcpy(data + pos, &value, 1);
				pos++;
				value = (decoded->exif.size + 2) & 0xff;
				memcpy(data + pos, &value, 1);
				pos++;
				memcpy(data + pos, decoded->exif.data, decoded->exif.size);
				pos += decoded->exif.size;
				memcpy(data + pos, image_jpg + image_data_offset, image_data_len);

			}
			else
				size = decoded->exif.size + 512;
		}
		avifDecoderDestroy(decoder);
	}
	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
}
