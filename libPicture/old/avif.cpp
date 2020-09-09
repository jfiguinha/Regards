#include <header.h>
#include "avif.h"
#include <RegardsBitmap.h>
#include <picture_id.h>
#include <avif/avif.h>
#include <heifreader.h>

CAvif::CAvif()
{

}

CAvif::~CAvif()
{

}

void LoadDataFromFile(const string &filename, avifRWData & raw)
{
	FILE * f = fopen(filename.c_str(), "rb");
	if (!f)
		return;
	fseek(f, 0, SEEK_END);
	uint32_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	avifRWDataRealloc(&raw, size);
	fread(raw.data, 1, size, f);
	fclose(f);
}

int CAvif::GetDelay(const string &filename)
{
	int delay = 0;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);
	// Decode it
	avifImage * decoded = avifImageCreateEmpty();
	avifDecoder * decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		delay = decoder->duration / decoder->imageCount;
		avifDecoderDestroy(decoder);
	}
	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
	return delay;
}

int CAvif::GetNbFrame(const string &filename)
{
	int nbFrame = 0;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);
	// Decode it
	avifImage * decoded = avifImageCreateEmpty();
	avifDecoder * decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		nbFrame = decoder->imageCount;
		avifDecoderDestroy(decoder);
	}
	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
	return nbFrame;
}


void CAvif::GetPictureDimension(const string &filename, int &width, int &height)
{
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);

	// Decode it
	avifImage * decoded = avifImageCreateEmpty();
	avifDecoder * decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifResult decodeResult = avifDecoderRead(decoder, decoded, (avifROData *)&raw);

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

CRegardsBitmap * CAvif::GetThumbnailPicture(const string &filename)
{
	CRegardsBitmap * picture = nullptr;
	auto* reader = Reader::Create();
	uint64_t itemSize = 1024 * 1024;
	uint8_t* itemData = new uint8_t[itemSize];
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
			const ImageId thumbId = *std::min_element(thumbIds.begin(), thumbIds.end(), [&](ImageId a, ImageId b) {
				uint32_t widthA, widthB;
				reader->getWidth(a, widthA);
				reader->getWidth(b, widthB);
				return (widthA < widthB);
			});
			//imageMap[masterId] = thumbId;

			if (reader->getItemDataWithDecoderParameters(thumbId.get(), itemData, itemSize) == ErrorCode::OK)
			{
				avifDecoder * decoder = avifDecoderCreate();
				avifRWData raw = AVIF_DATA_EMPTY;
				avifRWDataRealloc(&raw, itemSize);
				memcpy(&raw, itemData, itemSize);
				// Decode it
				avifImage * decoded = avifImageCreateEmpty();
				avifDecoder * decoder = avifDecoderCreate();
				if (decoder != nullptr)
				{
					avifResult decodeResult = avifDecoderRead(decoder, decoded, (avifROData *)&raw);

					if (decodeResult == AVIF_RESULT_OK)
					{
						avifRGBImage dstRGB;
						avifRGBImageSetDefaults(&dstRGB, decoded);
						dstRGB.format = AVIF_RGB_FORMAT_BGRA;                 // See choices in avif.h
						dstRGB.depth = 8;                  // [8, 10, 12, 16]; Does not need to match image->depth.
						avifRGBImageAllocatePixels(&dstRGB);

						if (avifImageYUVToRGB(decoded, &dstRGB) == AVIF_RESULT_OK)
						{
							int image_width = dstRGB.width;
							int image_height = dstRGB.height;
							picture = new CRegardsBitmap(image_width, image_height);
							uint8_t * dataOut = picture->GetPtBitmap();
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

CRegardsBitmap * CAvif::GetPicture(const string &filename, int &delay, const int &numPicture)
{
	CRegardsBitmap * out = nullptr;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);

	// Decode it
	avifDecoder * decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifResult decodeResult = avifDecoderParse(decoder, (avifROData *)&raw);
		if (decodeResult == AVIF_RESULT_OK)
		{
			int frameIndex = 0;
			delay = decoder->duration / decoder->imageCount;   // Duration of entire sequence (seconds)
			for (;;)
			{
				avifResult nextImageResult = avifDecoderNextImage(decoder);
				if (nextImageResult == AVIF_RESULT_NO_IMAGES_REMAINING) {
					// No more images, bail out. Verify that you got the expected amount of images decoded.
					break;
				}
				else if (nextImageResult != AVIF_RESULT_OK) {
					printf("ERROR: Failed to decode all frames: %s\n", avifResultToString(nextImageResult));
					break;
				}
				else if (nextImageResult == AVIF_RESULT_OK && numPicture == frameIndex)
				{

					avifRGBImage dstRGB;
					avifRGBImageSetDefaults(&dstRGB, decoder->image);
					dstRGB.format = AVIF_RGB_FORMAT_BGRA;                 // See choices in avif.h
					dstRGB.depth = 8;                  // [8, 10, 12, 16]; Does not need to match image->depth.
					avifRGBImageAllocatePixels(&dstRGB);

					if (avifImageYUVToRGB(decoder->image, &dstRGB) == AVIF_RESULT_OK)
					{
						int image_width = dstRGB.width;
						int image_height = dstRGB.height;
						out = new CRegardsBitmap(image_width, image_height);
						uint8_t * dataOut = out->GetPtBitmap();
						memcpy(dataOut, dstRGB.pixels, out->GetBitmapSize());
						out->VertFlipBuf();
					}
					avifRGBImageFreePixels(&dstRGB);
				}
				frameIndex++;
			}
		}
		avifDecoderDestroy(decoder);
	}
	avifRWDataFree(&raw);
	return out;
}

vector<CRegardsBitmap *> CAvif::GetAllPicture(const string &filename, int &delay)
{
	vector<CRegardsBitmap *> listPicture;
	
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);

	// Decode it
	avifDecoder * decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifResult decodeResult = avifDecoderParse(decoder, (avifROData *)&raw);
		if (decodeResult == AVIF_RESULT_OK) 
		{
			int frameIndex = 0;
			avifBool firstImage = AVIF_TRUE;
			delay = decoder->duration / decoder->imageCount;   // Duration of entire sequence (seconds)
			for (;;)
			{
				avifResult nextImageResult = avifDecoderNextImage(decoder);
				if (nextImageResult == AVIF_RESULT_NO_IMAGES_REMAINING) {
					// No more images, bail out. Verify that you got the expected amount of images decoded.
					break;
				}
				else if (nextImageResult != AVIF_RESULT_OK) {
					printf("ERROR: Failed to decode all frames: %s\n", avifResultToString(nextImageResult));
					break;
				}
				else if (nextImageResult == AVIF_RESULT_OK)
				{

					avifRGBImage dstRGB;
					avifRGBImageSetDefaults(&dstRGB, decoder->image);
					dstRGB.format = AVIF_RGB_FORMAT_BGRA;                 // See choices in avif.h
					dstRGB.depth = 8;                  // [8, 10, 12, 16]; Does not need to match image->depth.
					avifRGBImageAllocatePixels(&dstRGB);

					if (avifImageYUVToRGB(decoder->image, &dstRGB) == AVIF_RESULT_OK)
					{
						int image_width = dstRGB.width;
						int image_height = dstRGB.height;
						CRegardsBitmap * out = new CRegardsBitmap(image_width, image_height);
						uint8_t * dataOut = out->GetPtBitmap();
						memcpy(dataOut, dstRGB.pixels, out->GetBitmapSize());
						out->VertFlipBuf();
						listPicture.push_back(out);
					}
					avifRGBImageFreePixels(&dstRGB);
				}
		
			}
		}
		avifDecoderDestroy(decoder);
	}
	avifRWDataFree(&raw);
	return listPicture;
}

CRegardsBitmap * CAvif::GetPicture(const string &filename)
{

	CRegardsBitmap * out = nullptr;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);

	// Decode it
	avifImage * decoded = avifImageCreateEmpty();
	avifDecoder * decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifResult decodeResult = avifDecoderRead(decoder, decoded, (avifROData *)&raw);

		if (decodeResult == AVIF_RESULT_OK)
		{
			avifRGBImage dstRGB;
			avifRGBImageSetDefaults(&dstRGB, decoded);
			dstRGB.format = AVIF_RGB_FORMAT_BGRA;                 // See choices in avif.h
			dstRGB.depth = 8;                  // [8, 10, 12, 16]; Does not need to match image->depth.
			avifRGBImageAllocatePixels(&dstRGB);

			if (avifImageYUVToRGB(decoded, &dstRGB) == AVIF_RESULT_OK)
			{
				int image_width = dstRGB.width;
				int image_height = dstRGB.height;
				out = new CRegardsBitmap(image_width, image_height);
				uint8_t * dataOut = out->GetPtBitmap();
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
bool CAvif::HasExifMetaData(const string &filename)
{
	bool exifData = false;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);
	avifImage * decoded = avifImageCreateEmpty();
	avifDecoder * decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifResult decodeResult = avifDecoderRead(decoder, decoded, (avifROData *)&raw);
		if (decoded->exif.size > 0)
			exifData = true;
		avifDecoderDestroy(decoder);
	}
	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
	return exifData;

}

// static
void CAvif::GetMetadata(const string &filename, uint8_t * & data, long & size)
{
	bool exifData = false;
	avifRWData raw = AVIF_DATA_EMPTY;
	LoadDataFromFile(filename, raw);
	avifImage * decoded = avifImageCreateEmpty();
	avifDecoder * decoder = avifDecoderCreate();
	if (decoder != nullptr)
	{
		avifResult decodeResult = avifDecoderRead(decoder, decoded, (avifROData *)&raw);
		if (decoded->exif.size > 0)
		{
			if (size > 0)
				memcpy(data, decoded->exif.data, size);
			else
				size = decoded->exif.size;
		}
		avifDecoderDestroy(decoder);
	}
	avifImageDestroy(decoded);
	avifRWDataFree(&raw);
}