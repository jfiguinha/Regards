#include <header.h>
#include "avif_heif.h"
#include <libheif/heif.h>
#include <RegardsBitmap.h>
#include <picture_id.h>
static const char kMetadataTypeExif[] = "Exif";

CHeifAvif::CHeifAvif()
{
}

CHeifAvif::~CHeifAvif()
{
}

int CHeifAvif::GetNbFrame(const string& filename)
{
	int num_images = 0;
	heif_context* ctx = heif_context_alloc();
	if (ctx)
	{
		heif_context_read_from_file(ctx, filename.c_str(), nullptr);

		num_images = heif_context_get_number_of_top_level_images(ctx);
		if (num_images == 0)
		{
			fprintf(stderr, "File doesn't contain any images\n");
			return num_images;
		}

		heif_context_free(ctx);
	}
	return num_images;
}

CRegardsBitmap* CHeifAvif::GetThumbnailPicture(const string& filename)
{
	CRegardsBitmap* outputBitmap = nullptr;
	
	heif_context* ctx = heif_context_alloc();
	if (ctx)
	{
		heif_context_read_from_file(ctx, filename.c_str(), nullptr);

		// get a handle to the primary image
		heif_image_handle* handle;
		heif_context_get_primary_image_handle(ctx, &handle);
		if (handle)
		{
			// --- decode the image (or its thumbnail)
			heif_decoding_options* decode_options = heif_decoding_options_alloc();
			decode_options->convert_hdr_to_8bit = true;

			//int bit_depth = 8;

			//struct heif_image* image = NULL;
			heif_image* img;
			heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr);
			if (img)
			{
				int stride;
				const uint8_t* data = heif_image_get_plane_readonly(img, heif_channel_interleaved, &stride);

				int image_width = heif_image_handle_get_width(handle);
				int image_height = heif_image_handle_get_height(handle);

				int iPos = 0;

				outputBitmap = new CRegardsBitmap(image_width, image_height);
				uint8_t* dataOut = outputBitmap->GetPtBitmap();
				for (int y = 0; y < image_height; y++)
					for (int x = 0; x < image_width; x++)
					{
						int pos = outputBitmap->GetPosition(x, image_height - y - 1);
						dataOut[pos + 2] = data[iPos++];
						dataOut[pos + 1] = data[iPos++];
						dataOut[pos] = data[iPos++];
					}
				//outputBitmap->SetBitmap((uint8_t *)data, image_width, image_height);
				heif_image_release(img);
			}
		}

		heif_context_free(ctx);
	}
	return outputBitmap;
}

void CHeifAvif::GetPictureDimension(const string& filename, int& width, int& height)
{
	heif_context* ctx = heif_context_alloc();
	if (ctx)
	{
		heif_context_read_from_file(ctx, filename.c_str(), nullptr);

		// get a handle to the primary image
		heif_image_handle* handle;
		heif_context_get_primary_image_handle(ctx, &handle);
		if (handle)
		{
			// decode the image and convert colorspace to RGB, saved as 24bit interleaved
			heif_image* img;
			heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr);
			if (img)
			{
				width = heif_image_handle_get_width(handle);
				height = heif_image_handle_get_height(handle);
				heif_image_release(img);
			}
		}

		heif_context_free(ctx);
	}
}

CRegardsBitmap* CHeifAvif::GetPicture(const string& filename)
{
	CRegardsBitmap* outputBitmap = nullptr;

	heif_context* ctx = heif_context_alloc();
	if (ctx)
	{
		heif_context_read_from_file(ctx, filename.c_str(), nullptr);

		// get a handle to the primary image
		heif_image_handle* handle;
		heif_context_get_primary_image_handle(ctx, &handle);
		if (handle)
		{
			// decode the image and convert colorspace to RGB, saved as 24bit interleaved
			heif_image* img;
			heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr);
			if (img)
			{
				int stride;
				const uint8_t* data = heif_image_get_plane_readonly(img, heif_channel_interleaved, &stride);

				int image_width = heif_image_handle_get_width(handle);
				int image_height = heif_image_handle_get_height(handle);

				int iPos = 0;

				outputBitmap = new CRegardsBitmap(image_width, image_height);
				uint8_t* dataOut = outputBitmap->GetPtBitmap();
				for (int y = 0; y < image_height; y++)
					for (int x = 0; x < image_width; x++)
					{
						int pos = outputBitmap->GetPosition(x, image_height - y - 1);
						dataOut[pos + 2] = data[iPos++];
						dataOut[pos + 1] = data[iPos++];
						dataOut[pos] = data[iPos++];
					}
				//outputBitmap->SetBitmap((uint8_t *)data, image_width, image_height);
				heif_image_release(img);
			}
		}

		heif_context_free(ctx);
	}
	return outputBitmap;
}

void CHeifAvif::SavePicture(const string& filenameOut, const int& type, CRegardsBitmap* source, const int& compression)
{
	if (source)
	{
		heif_context* ctx = heif_context_alloc();
		if (ctx)
		{
			// get the default encoder
			heif_encoder* encoder;
			if (type == AVIF)
				heif_context_get_encoder_for_format(ctx, heif_compression_AV1, &encoder);
			else if (type == HEIC)
				heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder);
			else
			{
				heif_context_free(ctx);
				return;
			}
			// set the encoder parameters
			heif_encoder_set_lossy_quality(encoder, compression);

			// encode the image
			heif_image* image; // code to fill in the image omitted in this example

			heif_error err = heif_image_create(source->GetBitmapWidth(), source->GetBitmapHeight(),
			                                   heif_colorspace_RGB,
			                                   heif_chroma_interleaved_RGBA,
			                                   &image);
			(void)err;

			heif_image_add_plane(image, heif_channel_interleaved, source->GetBitmapWidth(), source->GetBitmapHeight(),
			                     32);

			int stride;
			uint8_t* p = heif_image_get_plane(image, heif_channel_interleaved, &stride);
			uint8_t* data = source->GetPtBitmap();
			source->ConvertToBgr();
			//source->HorzFlipBuf();
			for (uint32_t y = 0; y < source->GetBitmapHeight(); y++)
			{
				const int position = source->GetPosition(0, source->GetBitmapHeight() - y - 1);
				memcpy(p + y * stride, data + position, source->GetBitmapWidth() * 4);
			}

			heif_context_encode_image(ctx, image, encoder, nullptr, nullptr);

			heif_encoder_release(encoder);

			heif_context_write_to_file(ctx, filenameOut.c_str());

			if (image)
			{
				heif_image_release(image);
			}

			heif_context_free(ctx);
		}
	}
}

// static
bool CHeifAvif::HasExifMetaData(const string& filename)
{
	int count = 0;
	heif_context* ctx = heif_context_alloc();
	if (ctx)
	{
		heif_context_read_from_file(ctx, filename.c_str(), nullptr);

		// get a handle to the primary image
		heif_image_handle* handle;
		heif_context_get_primary_image_handle(ctx, &handle);
		if (handle)
		{
			heif_item_id metadata_id;
			count = heif_image_handle_get_list_of_metadata_block_IDs(handle, kMetadataTypeExif,
			                                                         &metadata_id, 1);
		}
		heif_context_free(ctx);
	}
	return count > 0;
}

// static
void CHeifAvif::GetMetadata(const string& filename, uint8_t* & data, long& size)
{
	//int count = 0;
	heif_context* ctx = heif_context_alloc();
	if (ctx)
	{
		heif_context_read_from_file(ctx, filename.c_str(), nullptr);

		// get a handle to the primary image
		heif_image_handle* handle;
		heif_context_get_primary_image_handle(ctx, &handle);
		if (handle)
		{
			heif_item_id metadata_id;
			int count = heif_image_handle_get_list_of_metadata_block_IDs(handle, kMetadataTypeExif,
			                                                             &metadata_id, 1);

			if (count > 0)
			{
				const size_t datasize = heif_image_handle_get_metadata_size(handle, metadata_id);
				if (size > 0)
				{
					if (data)
					{
						heif_image_handle_get_metadata(handle, metadata_id, data);
					}
				}
				else
					size = datasize;
			}
		}
	}
}
