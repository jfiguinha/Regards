// ReSharper disable All
#include <header.h>
#include "Heic.h"
#ifdef LIBHEIC
#include <cstdint>
#include <vector>
#include <libheif/heif.h>

#include "imageinfo.hpp"


using namespace std;
using namespace Regards::Picture;


static const char kMetadataTypeExif[] = "Exif";

namespace
{
	// Decode un heif_image_handle vers un cv::Mat BGRA.
	// Retourne false en cas d'echec (handle invalide, decode impossible, etc).
	bool DecodeHandleToMat(heif_image_handle* handle, cv::Mat& outPicture)
	{
		if (handle == nullptr)
			return false;

		int width = heif_image_handle_get_width(handle);
		int height = heif_image_handle_get_height(handle);

		if (width <= 0 || height <= 0)
			return false;

		heif_image* img = nullptr;
		heif_error err = heif_decode_image(handle, &img, heif_colorspace_RGB,
			heif_chroma_interleaved_RGB, nullptr);
		if (err.code != heif_error_Ok || img == nullptr)
		{
			std::cerr << "heif_decode_image: " << err.message << "\n";
			return false;
		}

		int stride = 0;
		const uint8_t* data = heif_image_get_plane_readonly(img, heif_channel_interleaved, &stride);
		if (data == nullptr)
		{
			heif_image_release(img);
			return false;
		}

		outPicture.create(height, width, CV_8UC3);

		size_t len = outPicture.total() * outPicture.elemSize();
		if (len == static_cast<size_t>(stride) * height)
		{
			memcpy(outPicture.data, data, static_cast<size_t>(stride) * height);
		}
		else
		{
#pragma omp parallel for
			for (int y = 0; y < height; y++)
			{
				memcpy(outPicture.data + static_cast<size_t>(y) * width * 3,
					data + static_cast<size_t>(y) * stride,
					static_cast<size_t>(width) * 3);
			}
		}

		cv::cvtColor(outPicture, outPicture, cv::COLOR_BGR2RGBA);

		heif_image_release(img);
		return true;
	}

	// Recupere le bloc de metadonnees Exif (libheif) attache a un handle, s'il existe.
	// Retourne true si des donnees Exif ont ete trouvees et copiees dans buffer.
	bool GetHandleMetadataExif(heif_image_handle* handle, std::vector<uint8_t>& buffer)
	{
		heif_item_id metadata_id;
		int count = heif_image_handle_get_list_of_metadata_block_IDs(handle, kMetadataTypeExif,
			&metadata_id, 1);

		for (int i = 0; i < count; i++)
		{
			size_t datasize = heif_image_handle_get_metadata_size(handle, metadata_id);
			if (datasize == 0)
				continue;

			buffer.resize(datasize);
			heif_error error = heif_image_handle_get_metadata(handle, metadata_id, buffer.data());
			if (error.code != heif_error_Ok)
			{
				buffer.clear();
				continue;
			}

			return true;
		}

		return false;
	}
}

vector<cv::Mat> CHeic::GetAllPicture(const char* filename, int& delay)
{
	vector<cv::Mat> listPicture;

	heif_context* ctx = heif_context_alloc();
	if (ctx == nullptr)
		return listPicture;

	heif_error err = heif_context_read_from_file(ctx, filename, nullptr);
	if (err.code != heif_error_Ok)
	{
		std::cerr << err.message << "\n";
		heif_context_free(ctx);
		return listPicture;
	}

	int numImages = heif_context_get_number_of_top_level_images(ctx);
	if (numImages <= 0)
	{
		heif_context_free(ctx);
		return listPicture;
	}

	std::vector<heif_item_id> IDs(numImages);
	int nbReturnImage = heif_context_get_list_of_top_level_image_IDs(ctx, IDs.data(), numImages);
	if (nbReturnImage == 0)
	{
		heif_context_free(ctx);
		return listPicture;
	}

	for (int i = 0; i < numImages; i++)
	{
		heif_image_handle* handle = nullptr;
		heif_error herr = heif_context_get_image_handle(ctx, IDs[i], &handle);
		if (herr.code != heif_error_Ok)
		{
			std::cerr << herr.message << "\n";
			// une image en erreur ne doit pas empecher la lecture des suivantes
			continue;
		}

		cv::Mat picture;
		if (DecodeHandleToMat(handle, picture))
		{
			listPicture.push_back(picture);
		}

		heif_image_handle_release(handle);
	}

	heif_context_free(ctx);

	return listPicture;
}

/*
#define HEIC 26
#define AVIF 35
*/

void CHeic::SavePicture(const char* filenameOut, const int& format, cv::Mat& source, std::vector<uint8_t> & exifBuffer,
	const int& compression, const bool& hasExif)
{
	if (source.empty())
		return;

	heif_context* ctx = heif_context_alloc();
	if (ctx == nullptr)
		return;

	// get the encoder for the requested format
	heif_encoder* encoder = nullptr;
	heif_error err{};

	switch (format)
	{
	case 26: // HEIC
		err = heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder);
		break;
	case 35: // AVIF
		err = heif_context_get_encoder_for_format(ctx, heif_compression_AV1, &encoder);
		break;
	default:
		err = heif_context_get_encoder_for_format(ctx, heif_compression_undefined, &encoder);
		break;
	}

	if (err.code != heif_error_Ok || encoder == nullptr)
	{
		std::cerr << "heif_context_get_encoder_for_format: " << err.message << "\n";
		heif_context_free(ctx);
		return;
	}

	// set the encoder parameters
	heif_encoder_set_lossy_quality(encoder, compression);

	// create the image to encode
	heif_image* image = nullptr;
	err = heif_image_create(source.size().width, source.size().height,
		heif_colorspace_RGB,
		heif_chroma_interleaved_RGBA,
		&image);
	if (err.code != heif_error_Ok)
	{
		std::cerr << "heif_image_create: " << err.message << "\n";
		heif_encoder_release(encoder);
		heif_context_free(ctx);
		return;
	}

	err = heif_image_add_plane(image, heif_channel_interleaved, source.size().width, source.size().height, 32);
	if (err.code != heif_error_Ok)
	{
		std::cerr << "heif_image_add_plane: " << err.message << "\n";
		heif_image_release(image);
		heif_encoder_release(encoder);
		heif_context_free(ctx);
		return;
	}

	// conversion BGRA -> RGBA dans une copie locale, sans modifier le buffer de l'appelant
	cv::Mat rgba;
	if(source.channels() == 3)
		cv::cvtColor(source, rgba, cv::COLOR_BGR2RGBA);
	else
		cv::cvtColor(source, rgba, cv::COLOR_BGRA2RGBA);

	int stride = 0;
	uint8_t* p = heif_image_get_plane(image, heif_channel_interleaved, &stride);
	const uint8_t* data = rgba.data;
	for (int y = 0; y < rgba.rows; y++)
	{
		memcpy(p + static_cast<size_t>(y) * stride,
			data + static_cast<size_t>(y) * rgba.cols * 4,
			static_cast<size_t>(rgba.cols) * 4);
	}

	err = heif_context_encode_image(ctx, image, encoder, nullptr, nullptr);

	heif_encoder_release(encoder);
	heif_image_release(image);

	if (err.code != heif_error_Ok)
	{
		std::cerr << "heif_context_encode_image: " << err.message << "\n";
		heif_context_free(ctx);
		return;
	}

	heif_image_handle* image_handle = nullptr;
	err = heif_context_get_primary_image_handle(ctx, &image_handle);
	if (err.code == heif_error_Ok)
	{
		if (hasExif)
		{
			heif_context_add_exif_metadata(ctx, image_handle, exifBuffer.data(), exifBuffer.size());
		}
		heif_image_handle_release(image_handle);
	}
	else
	{
		std::cerr << "heif_context_get_primary_image_handle: " << err.message << "\n";
	}

	heif_context_write_to_file(ctx, filenameOut);

	heif_context_free(ctx);
}


cv::Mat CHeic::GetPicture(const char* filename, int& delay, const int& numPicture)
{
	cv::Mat picture;

	heif_context* ctx = heif_context_alloc();
	if (ctx == nullptr)
		return picture;

	heif_error err = heif_context_read_from_file(ctx, filename, nullptr);
	if (err.code != heif_error_Ok)
	{
		std::cerr << err.message << "\n";
		heif_context_free(ctx);
		return picture;
	}

	int numImages = heif_context_get_number_of_top_level_images(ctx);
	if (numPicture < 0 || numPicture >= numImages)
	{
		heif_context_free(ctx);
		return picture;
	}

	std::vector<heif_item_id> IDs(numImages);
	heif_context_get_list_of_top_level_image_IDs(ctx, IDs.data(), numImages);

	heif_image_handle* handle = nullptr;
	err = heif_context_get_image_handle(ctx, IDs[numPicture], &handle);
	if (err.code != heif_error_Ok)
	{
		std::cerr << err.message << "\n";
		heif_context_free(ctx);
		return picture;
	}

	// si le decodage echoue, "picture" reste vide (cv::Mat() par defaut)
	DecodeHandleToMat(handle, picture);

	heif_image_handle_release(handle);
	heif_context_free(ctx);

	return picture;
}

int CHeic::GetNbFrame(const char* filename)
{
	int nbId = 0;
	try
	{
		heif_context* ctx = heif_context_alloc();
		if (ctx == nullptr)
			return 0;

		struct heif_error err = heif_context_read_from_file(ctx, filename, nullptr);

		if (err.code != heif_error_Ok) {
			heif_context_free(ctx);
			return 0;
		}

		nbId = heif_context_get_number_of_top_level_images(ctx);
		heif_context_free(ctx);
	}
	catch (...)
	{

	}
	return nbId;
}

cv::Mat CHeic::GetPicture(const char* filename, int& orientation, const bool& isThumbnail)
{
	cv::Mat picture;

	heif_context* ctx = heif_context_alloc();
	if (ctx == nullptr)
		return picture;

	heif_error err = heif_context_read_from_file(ctx, filename, nullptr);
	if (err.code != heif_error_Ok)
	{
		std::cerr << err.message << "\n";
		heif_context_free(ctx);
		return picture;
	}

	// get a handle to the primary image
	heif_image_handle* handle = nullptr;
	err = heif_context_get_primary_image_handle(ctx, &handle);
	if (err.code != heif_error_Ok)
	{
		std::cerr << err.message << "\n";
		heif_context_free(ctx);
		return picture;
	}

	// NOTE: "isThumbnail" et "orientation" ne sont pas geres ici pour l'instant.
	// Si une miniature est demandee, utiliser CHeic::GetThumbnailPicture.
	// Pour l'orientation EXIF, voir heif_image_handle_get_property_user_description /
	// les transformations "irot"/"imir" exposees par libheif si besoin de l'appliquer.
	(void)isThumbnail;

	DecodeHandleToMat(handle, picture);

	heif_image_handle_release(handle);
	heif_context_free(ctx);

	return picture;
}

void CHeic::GetPictureDimension(const char* filename, int& width, int& height)
{
	auto info = imageinfo::parse<imageinfo::FilePathReader>(filename);
	if (info) {
		width = info.size().width;
		height = info.size().height;
	}
	else
	{
		width = 0;
		height = 0;
	}
}

cv::Mat CHeic::GetThumbnailPicture(const char* filename, int& orientation)
{
	cv::Mat picture;

	heif_context* ctx = heif_context_alloc();
	if (ctx == nullptr)
		return picture;

	heif_error err = heif_context_read_from_file(ctx, filename, nullptr);
	if (err.code != heif_error_Ok)
	{
		std::cerr << err.message << "\n";
		heif_context_free(ctx);
		return picture;
	}

	// get a handle to the primary image
	heif_image_handle* handle = nullptr;
	err = heif_context_get_primary_image_handle(ctx, &handle);
	if (err.code != heif_error_Ok)
	{
		std::cerr << err.message << "\n";
		heif_context_free(ctx);
		return picture;
	}

	int nThumbnails = heif_image_handle_get_number_of_thumbnails(handle);
	if (nThumbnails > 0)
	{
		std::vector<heif_item_id> thumbnailIDs(nThumbnails);
		nThumbnails = heif_image_handle_get_list_of_thumbnail_IDs(handle, thumbnailIDs.data(), nThumbnails);

		if (nThumbnails > 0)
		{
			heif_image_handle* thumbnail_handle = nullptr;
			err = heif_image_handle_get_thumbnail(handle, thumbnailIDs[0], &thumbnail_handle);
			if (err.code == heif_error_Ok)
			{
				DecodeHandleToMat(thumbnail_handle, picture);
				heif_image_handle_release(thumbnail_handle);
			}
			else
			{
				std::cerr << err.message << "\n";
			}
		}
	}

	heif_image_handle_release(handle);
	heif_context_free(ctx);

	return picture;
}


#include <heifreader.h>
using namespace HEIF;
/* raw JPEG image data, utilise comme conteneur pour reconstituer un fichier
   exploitable par les lecteurs Exif a partir du bloc Exif brut extrait du HEIC */
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

/* start of JPEG image data section */
static const unsigned int image_data_offset = 20;
#define image_data_len (image_jpg_len - image_data_offset)

void CHeicExif::GetMetadataHeic(const char* filename, uint8_t*& data, unsigned int& size)
{
	auto* reader = Reader::Create();

	if (reader->initialize(filename) != ErrorCode::OK)
	{
		cout << "Can't find input file: " << filename << ". "
			<< "Please download it from https://github.com/nokiatech/heif_conformance "
			<< "and place it in same directory with the executable." << endl;
		Reader::Destroy(reader);
		return;
	}

	FileInformation fileInfo{};
	reader->getFileInformation(fileInfo);

	// Find the primary item ID.
	ImageId primaryItemId;
	reader->getPrimaryItem(primaryItemId);

	// Find item(s) referencing to the primary item with "cdsc" (content describes) item reference.
	Array<ImageId> metadataIds;
	reader->getReferencedToItemListByType(primaryItemId, "cdsc", metadataIds);

	if (metadataIds.size == 0)
	{
		Reader::Destroy(reader);
		return;
	}

	ImageId exifItemId = metadataIds[0];

	// Optional: verify the item ID we got is really of "Exif" type.
	FourCC itemType;
	reader->getItemType(exifItemId, itemType);
	if (itemType != "Exif")
	{
		Reader::Destroy(reader);
		return;
	}

	// Get item size from parsed information. For simplicity, assume it is the first and only
	// non-image item in the file.
	uint64_t itemSize = 1024 * 1024;
	for (const ItemInformation& itemInfo : fileInfo.rootMetaBoxInformation.itemInformations)
	{
		if (itemInfo.itemId == exifItemId)
		{
			itemSize = itemInfo.size;
			break;
		}
	}

	/* raw EXIF header data */
	static const unsigned char exif_header[] = {
		0xff, 0xd8, 0xff, 0xe1
	};
	/* length of data in exif_header */
	static const unsigned int exif_header_len = sizeof(exif_header);

	if (itemSize < 4)
	{
		// item Exif anormalement petit, rien d'exploitable
		Reader::Destroy(reader);
		return;
	}

	// Taille finale du buffer attendu par l'appelant :
	// header(4) + 2 octets de longueur + (itemSize - 4) + JPEG embarque
	const unsigned int requiredSize =
		static_cast<unsigned int>(exif_header_len + 2 + (itemSize - 4) + image_data_len);

	if (size == 0)
	{
		// Premiere passe : on indique a l'appelant la taille a allouer.
		size = requiredSize;
		Reader::Destroy(reader);
		return;
	}

	if (size < requiredSize)
	{
		// Buffer fourni trop petit : on signale la taille necessaire et on s'arrete
		// sans ecrire (evite un debordement de "data").
		size = requiredSize;
		Reader::Destroy(reader);
		return;
	}

	// Deuxieme passe : "data" est suppose avoir ete alloue par l'appelant
	// avec au moins "requiredSize" octets.
	std::vector<uint8_t> memoryBuffer(itemSize);
	reader->getItemData(exifItemId, memoryBuffer.data(), itemSize);

	int pos = 0;
	memcpy(data, exif_header, exif_header_len);
	pos += exif_header_len;

	uint8_t lenHigh = static_cast<uint8_t>((itemSize + 2) >> 8);
	uint8_t lenLow = static_cast<uint8_t>((itemSize + 2) & 0xff);
	memcpy(data + pos, &lenHigh, 1);
	pos++;
	memcpy(data + pos, &lenLow, 1);
	pos++;

	memcpy(data + pos, memoryBuffer.data() + 4, itemSize - 4);
	pos += static_cast<int>(itemSize - 4);

	memcpy(data + pos, image_jpg + image_data_offset, image_data_len);

	size = requiredSize;

	Reader::Destroy(reader);
}


uint32_t CHeic::GetDelay(const char* filename)
{
	int delay = 0;
	auto* reader = Reader::Create();
	Array<uint32_t> itemIds;

	// Input file available from https://github.com/nokiatech/heif_conformance
	if (reader->initialize(filename) == ErrorCode::OK)
	{
		FileInformation info;
		if (reader->getFileInformation(info) == ErrorCode::OK)
		{
			if (info.trackInformation.size > 0)
			{
				// Print information for every track read
				for (const auto& trackProperties : info.trackInformation)
				{
					const auto sequenceId = trackProperties.trackId;
					Array<TimestampIDPair> timestamps;
					reader->getItemTimestamps(sequenceId, timestamps);
					for (const auto& timestamp : timestamps)
					{
						delay = timestamp.timeStamp;
						break;
					}
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

	Reader::Destroy(reader);

	return delay;
}

#endif