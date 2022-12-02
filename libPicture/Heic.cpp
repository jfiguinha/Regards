// ReSharper disable All
#include <header.h>
#include "Heic.h"
#ifdef LIBHEIC
#include <cstdint>
#include <heifreader.h>
#include <libde265/de265.h>
#include "yuv420.h"
#include "yuv422.h"
#include <libheif/heif.h>
#include <FileUtility.h>
#include <map>
using namespace std;
using namespace HEIF;
using namespace Regards::Picture;
static cv::Mat & DecodeFrame(void* data, int length, void* externDecoder = nullptr);

typedef struct x265Frame
{
	uint64_t memoryBufferSize;
	uint8_t* _memoryBuffer;
	cv::Mat picture;
	wxString filename;
};

CHeic::CHeic()
{
}


CHeic::~CHeic()
{
}

void CHeic::Initx265Decoder()
{
	de265_init();
}

void CHeic::Uninitx265Decoder()
{
	de265_free();
}

static uint8_t* convert_to_8bit(const uint8_t* data, int width, int height,
                                int pixelsPerLine, int bit_depth)
{
	auto data16 = reinterpret_cast<const uint16_t*>(data);
	auto out = new uint8_t[pixelsPerLine * height];

	for (auto y = 0; y < height; y++)
	{
		for (auto x = 0; x < width; x++)
		{
			out[y * pixelsPerLine + x] = *(data16 + y * pixelsPerLine + x) >> (bit_depth - 8);
		}
	}

	return out;
}


cv::Mat & GetRGBPicture(const de265_image* img)
{
	int width = de265_get_image_width(img, 0);
	int height = de265_get_image_height(img, 0);

	int chroma_width = de265_get_image_width(img, 1);
	int chroma_height = de265_get_image_height(img, 1);

	de265_chroma chroma = de265_get_chroma_format(img);

	int frame_width = width;
	int frame_height = height;

	frame_width &= ~7;
	frame_height &= ~7;

	cv::Mat picture;
	int stride, chroma_stride;
	const uint8_t* yOrigin = de265_get_image_plane(img, 0, &stride);
	const uint8_t* cbOrigin = de265_get_image_plane(img, 1, &chroma_stride);
	const uint8_t* crOrigin = de265_get_image_plane(img, 2, nullptr);

	int bpp_y = (de265_get_bits_per_pixel(img, 0) + 7) / 8;
	int bpp_c = (de265_get_bits_per_pixel(img, 1) + 7) / 8;
	int ppl_y = stride / bpp_y;
	int ppl_c = chroma_stride / bpp_c;

	uint8_t* y16 = nullptr;
	uint8_t* cb16 = nullptr;
	uint8_t* cr16 = nullptr;
	int bd;

	if ((bd = de265_get_bits_per_pixel(img, 0)) > 8)
	{
		y16 = convert_to_8bit(yOrigin, width, height, ppl_y, bd);
		yOrigin = y16;
	}

	if (chroma != de265_chroma_mono)
	{
		if ((bd = de265_get_bits_per_pixel(img, 1)) > 8)
		{
			cb16 = convert_to_8bit(cbOrigin, chroma_width, chroma_height, ppl_c, bd);
			cbOrigin = cb16;
		}
		if ((bd = de265_get_bits_per_pixel(img, 2)) > 8)
		{
			cr16 = convert_to_8bit(crOrigin, chroma_width, chroma_height, ppl_c, bd);
			crOrigin = cr16;
		}
	}


	switch (chroma)
	{
	case de265_chroma_420:
		{
			auto _y = new uint8_t[frame_width * frame_height];
			auto _cb = new uint8_t[frame_width * frame_height / 4];
			auto _cr = new uint8_t[frame_width * frame_height / 4];
			picture.create(frame_height, frame_width, CV_8UC4);

			if (stride == frame_width && chroma_stride == frame_width / 2)
			{
				// fast copy

				memcpy(_y, yOrigin, frame_width * frame_height);
				memcpy(_cb, cbOrigin, frame_width * frame_height / 4);
				memcpy(_cr, crOrigin, frame_width * frame_height / 4);
			}
			else
			{
				// copy line by line, because sizes are different

				for (auto y = 0; y < frame_height; y++)
				{
					memcpy(_y + y * frame_width, yOrigin + stride * y, frame_width);
				}

				for (auto y = 0; y < frame_height / 2; y++)
				{
					memcpy(_cb + y * frame_width / 2, cbOrigin + chroma_stride * y, frame_width / 2);
					memcpy(_cr + y * frame_width / 2, crOrigin + chroma_stride * y, frame_width / 2);
				}
			}
			yuv420p_to_rgb32(_y, _cb, _cr, picture.data, frame_width, frame_height);
			delete[] _y;
			delete[] _cb;
			delete[] _cr;
		}
		break;
	case de265_chroma_422:
		{
			auto dataYUV = new uint8_t[frame_width * 2 * frame_height];
			picture.create(frame_height, frame_width, CV_8UC4);

			for (auto y = 0; y < frame_height; y++)
			{
				unsigned char* p = dataYUV + y * frame_width * 2;

				const unsigned char* Yp = yOrigin + y * stride;
				const unsigned char* Up = cbOrigin + y * chroma_stride;
				const unsigned char* Vp = crOrigin + y * chroma_stride;

				for (auto x = 0; x < frame_width; x += 2)
				{
					*p++ = Yp[x];
					*p++ = Up[x / 2];
					*p++ = Yp[x + 1];
					*p++ = Vp[x / 2];
				}
			}

			yuv422p_to_rgb32(dataYUV, picture.data, frame_width, frame_height);
			delete[] dataYUV;
		}
		break;
	case de265_chroma_444:
		{
			picture.create(frame_height, frame_width, CV_8UC4);
			auto _y = new uint8_t[frame_width * frame_height];
			auto _cb = new uint8_t[frame_width * frame_height / 4];
			auto _cr = new uint8_t[frame_width * frame_height / 4];

			for (auto y = 0; y < frame_height; y++)
			{
				unsigned char* p = _y + y * frame_width;
				memcpy(p, yOrigin + y * stride, frame_width);
			}

			for (auto y = 0; y < frame_height; y += 2)
			{
				unsigned char* u = _cb + y / 2 * frame_width / 2;
				unsigned char* v = _cr + y / 2 * frame_width / 2;

				for (auto x = 0; x < frame_width; x += 2)
				{
					u[x / 2] = (U[y * chroma_stride + x] + U[y * chroma_stride + x + 1] +
						U[(y + 1) * chroma_stride + x] + U[(y + 1) * chroma_stride + x + 1]) / 4;
					v[x / 2] = (V[y * chroma_stride + x] + V[y * chroma_stride + x + 1] +
						V[(y + 1) * chroma_stride + x] + V[(y + 1) * chroma_stride + x + 1]) / 4;

					//u[x/2] = U[y*chroma_stride + x];
					//v[x/2] = V[y*chroma_stride + x];
				}
			}
			yuv420p_to_rgb32(_y, _cb, _cr, picture.data, frame_width, frame_height);
			delete[] _y;
			delete[] _cb;
			delete[] _cr;
		}
		break;
	case de265_chroma_mono:
		{
			picture.create(frame_height, frame_width, CV_8UC4);
			auto _y = new uint8_t[frame_width * frame_height];
			auto _cb = new uint8_t[frame_width * frame_height / 4];
			auto _cr = new uint8_t[frame_width * frame_height / 4];
			if (stride == frame_width)
			{
				// fast copy

				memcpy(_y, yOrigin, frame_width * frame_height);
			}
			else
			{
				// copy line by line, because sizes are different

				for (auto y = 0; y < frame_height; y++)
				{
					memcpy(_y + y * frame_width, yOrigin + stride * y, frame_width);
				}
			}

			// clear chroma planes

			memset(_cb, 0x80, frame_width * frame_height / 4);
			memset(_cr, 0x80, frame_width * frame_height / 4);

			yuv420p_to_rgb32(_y, _cb, _cr, picture.data, frame_width, frame_height);
			delete[] _y;
			delete[] _cb;
			delete[] _cr;
		}
		break;
	}


	delete[] y16;
	delete[] cb16;
	delete[] cr16;

	return picture;
}


cv::Mat & DecodeFrame(void* data, int length, void* externDecoder)
{
	cv::Mat picture;
	de265_decoder_context* decoderContext = nullptr;
	if (decoderContext == nullptr)
		decoderContext = de265_new_decoder();
	else
		decoderContext = externDecoder;

	de265_error err = de265_push_data(decoderContext, data, length, 0, nullptr);
	if (err == DE265_OK)
	{
		int check_hash = 1;
		int quiet = 0;
		err = de265_flush_data(decoderContext);
		if (err == DE265_OK)
		{
			int more = 1;
			while (more)
			{
				more = 0;

				// decode some more
				try
				{
					err = de265_decode(decoderContext, &more);
				}
				catch (...)
				{
					break;
				}
				if (err != DE265_OK)
				{
					if (quiet <= 1)
						fprintf(stderr, "ERROR: %s\n", de265_get_error_text(err));

					if (check_hash && err == DE265_ERROR_CHECKSUM_MISMATCH)
						break;
					more = 0;
					break;
				}

				// show available images

				const de265_image* img = de265_get_next_picture(decoderContext);
				if (img)
				{
					picture = GetRGBPicture(img);
					cv::flip(picture, picture, 0);
					cvtColor(picture, picture, cv::COLOR_RGBA2BGRA);
					more = 0;
				}

				// show warnings

				for (;;)
				{
					de265_error warning = de265_get_warning(decoderContext);
					if (warning == DE265_OK)
					{
						break;
					}

					if (quiet <= 1)
						fprintf(stderr, "WARNING: %s\n", de265_get_error_text(warning));
				}
			}
		}
	}
	de265_free_decoder(decoderContext);

	return picture;
}

vector<cv::Mat> DecodePictureList(de265_decoder_context* decoderContext, const string& filename)
{
	vector<cv::Mat> listPicture;

	int check_hash = 1;
	int quiet = 0;
	de265_error err = de265_flush_data(decoderContext);
	if (err == DE265_OK)
	{
		int more = 1;
		while (more)
		{
			more = 0;

			// decode some more
			try
			{
				err = de265_decode(decoderContext, &more);
			}
			catch (...)
			{
				break;
			}
			if (err != DE265_OK)
			{
				if (quiet <= 1)
					fprintf(stderr, "ERROR: %s\n", de265_get_error_text(err));

				if (check_hash && err == DE265_ERROR_CHECKSUM_MISMATCH)
					break;
				more = 0;
				break;
			}

			// show available images

			const de265_image* img = de265_get_next_picture(decoderContext);
			if (img)
			{
				cv::Mat picture = GetRGBPicture(img);
				cv::flip(picture, picture, 0);
				cvtColor(picture, picture, cv::COLOR_RGBA2BGRA);
				listPicture.push_back(picture);
			}

			// show warnings

			for (;;)
			{
				de265_error warning = de265_get_warning(decoderContext);
				if (warning == DE265_OK)
				{
					break;
				}

				if (quiet <= 1)
					fprintf(stderr, "WARNING: %s\n", de265_get_error_text(warning));
			}
		}
	}
	return listPicture;
}

vector<cv::Mat> CHeic::GetAllPicture(const string& filename, bool& isMasterSequence, int& delay)
{
	vector<cv::Mat> listPicture;
	auto* reader = Reader::Create();
	Array<uint32_t> item_ids;

	// Input file available from https://github.com/nokiatech/heif_conformance
	if (reader->initialize(filename.c_str()) == ErrorCode::OK)
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
					//cout << "Track ID " << sequenceId << endl;  // Context ID corresponds to the track ID

					if (trackProperties.features & TrackFeatureEnum::IsMasterImageSequence)
					{
						isMasterSequence = true;
						//cout << "This is a master image sequence." << endl;
					}

					if (trackProperties.features & TrackFeatureEnum::IsThumbnailImageSequence)
					{
						// Assume there is only one type track reference, so check reference type and master track ID(s) from
						// the first one.
						isMasterSequence = false;
						const auto tref = trackProperties.referenceTrackIds[0];
						//cout << "Track reference type is '" << tref.type.value << "'" << endl;
						//cout << "This is a thumbnail track for track ID ";
						/*
						for (const auto masterTrackId : tref.trackIds)
						{
							//cout << masterTrackId << endl;
						}*/
					}

					Array<TimestampIDPair> timestamps;
					reader->getItemTimestamps(sequenceId, timestamps);
					//cout << "Sample timestamps:" << endl;
					for (const auto& timestamp : timestamps)
					{
						delay = timestamp.timeStamp;
						//cout << " Timestamp=" << timestamp.timeStamp << "ms, sample ID=" << timestamp.itemId << endl;
					}

					for (const auto& sampleProperties : trackProperties.sampleProperties)
					{
						// A sample might have decoding dependencies. The simplest way to handle this is just to always ask and
						// decode all dependencies.
						Array<SequenceImageId> items_to_decode;
						reader->getDecodeDependencies(sequenceId, sampleProperties.sampleId, items_to_decode);
						for (auto dependencyId : items_to_decode)
						{
							uint64_t size = 1024 * 1024;
							auto sampleData = new uint8_t[size];
							reader->getItemDataWithDecoderParameters(sequenceId, dependencyId, sampleData, size);

							//err = de265_push_data(decoderContext, sampleData, size, 0, NULL);
							cv::Mat bitmapSrc = DecodeFrame(sampleData, size);
							if (!bitmapSrc.empty())
							{
								listPicture.push_back(bitmapSrc);
							}

							delete[] sampleData;
						}
						// Store or show the image...
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


	return listPicture;
}

void CHeic::SavePicture(const string& filenameOut, cv::Mat & source, uint8_t*& data_exif, unsigned int& size, const int& compression, const bool& hasExif)
{
#ifdef HAS_X265
	struct heif_error err{};
	if (source)
	{
		heif_context* ctx = heif_context_alloc();
		if (ctx)
		{
			// get the default encoder
			heif_encoder* encoder;
			heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder);



			// set the encoder parameters
			heif_encoder_set_lossy_quality(encoder, compression);



			// encode the image
			heif_image* image; // code to fill in the image omitted in this example

			err = heif_image_create(source->GetBitmapWidth(), source->GetBitmapHeight(),
			                        heif_colorspace_RGB,
			                        heif_chroma_interleaved_RGBA,
			                        &image);




			heif_image_add_plane(image, heif_channel_interleaved, source->GetBitmapWidth(), source->GetBitmapHeight(),
			                     32);

			int stride;
			uint8_t* p = heif_image_get_plane(image, heif_channel_interleaved, &stride);
			uint8_t* data = source->GetPtBitmap();
			source->ConvertToBgr();
			//source->HorzFlipBuf();
			for (uint32_t y = 0; y < source->GetBitmapHeight(); y++)
			{
				int position = source->GetPosition(0, source->GetBitmapHeight() - y - 1);
				memcpy(p + y * stride, data + position, source->GetBitmapWidth() * 4);
			}

			heif_context_encode_image(ctx, image, encoder, nullptr, nullptr);

			if (encoder != nullptr)
				heif_encoder_release(encoder);
			if (image != nullptr)
				heif_image_release(image);

			heif_image_handle* image_handle;
			heif_context_get_primary_image_handle(ctx, &image_handle);
			if (hasExif)
			{
				heif_context_add_exif_metadata(ctx, image_handle, data_exif, size);
			}
			heif_image_handle_release(image_handle);

			heif_context_write_to_file(ctx, filenameOut.c_str());

			heif_context_free(ctx);
		}
	}
#endif
}


uint32_t CHeic::GetDelay(const string& filename)
{
	uint32_t delay = 0;
	auto* reader = Reader::Create();
	Array<uint32_t> itemIds;

	// Input file available from https://github.com/nokiatech/heif_conformance
	if (reader->initialize(filename.c_str()) == ErrorCode::OK)
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
					//cout << "Sample timestamps:" << endl;
					for (const auto& timestamp : timestamps)
					{
						delay = timestamp.timeStamp;
						break;
						//cout << " Timestamp=" << timestamp.timeStamp << "ms, sample ID=" << timestamp.itemId << endl;
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

cv::Mat CHeic::GetPicture(const string& filename, bool& isMasterSequence, int& delay, const int& numPicture)
{
	cv::Mat bitmapSrc;
	auto* reader = Reader::Create();
	Array<uint32_t> itemIds;

	// Input file available from https://github.com/nokiatech/heif_conformance
	if (reader->initialize(filename.c_str()) == ErrorCode::OK)
	{
		//de265_error err = de265_error::DE265_OK;
		FileInformation info;
		if (reader->getFileInformation(info) == ErrorCode::OK)
		{
			if (info.trackInformation.size > 0)
			{
				// Print information for every track read
				for (const auto& trackProperties : info.trackInformation)
				{
					const auto sequenceId = trackProperties.trackId;
					//cout << "Track ID " << sequenceId << endl;  // Context ID corresponds to the track ID

					if (trackProperties.features & TrackFeatureEnum::IsMasterImageSequence)
					{
						isMasterSequence = true;
						//cout << "This is a master image sequence." << endl;
					}

					if (trackProperties.features & TrackFeatureEnum::IsThumbnailImageSequence)
					{
						// Assume there is only one type track reference, so check reference type and master track ID(s) from
						// the first one.
						isMasterSequence = false;
						const auto tref = trackProperties.referenceTrackIds[0];
						//cout << "Track reference type is '" << tref.type.value << "'" << endl;
						//cout << "This is a thumbnail track for track ID ";
						/*
						for (const auto masterTrackId : tref.trackIds)
						{
							//cout << masterTrackId << endl;
						}
						*/
					}

					Array<TimestampIDPair> timestamps;
					reader->getItemTimestamps(sequenceId, timestamps);
					//cout << "Sample timestamps:" << endl;
					for (const auto& timestamp : timestamps)
					{
						delay = timestamp.timeStamp;
						break;
						//cout << " Timestamp=" << timestamp.timeStamp << "ms, sample ID=" << timestamp.itemId << endl;
					}

					int i = 0;
					for (const auto& sampleProperties : trackProperties.sampleProperties)
					{
						// A sample might have decoding dependencies. The simplest way to handle this is just to always ask and
						// decode all dependencies.
						Array<SequenceImageId> itemsToDecode;
						reader->getDecodeDependencies(sequenceId, sampleProperties.sampleId, itemsToDecode);
						for (auto dependencyId : itemsToDecode)
						{
							if (i == numPicture)
							{
								uint64_t size = 1024 * 1024;
								auto sampleData = new uint8_t[size];
								reader->getItemDataWithDecoderParameters(sequenceId, dependencyId, sampleData, size);

								//err = de265_push_data(decoderContext, sampleData, size, 0, NULL);
								bitmapSrc = DecodeFrame(sampleData, size);

								delete[] sampleData;
								break;
							}
							i++;
						}
						// Store or show the image...
						if (i == numPicture)
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


	return bitmapSrc;
}

int CHeic::GetNbFrame(const string& filename)
{
	int nbId = 0;
	auto* reader = Reader::Create();

	if (reader->initialize(filename.c_str()) == ErrorCode::OK)
	{
		FileInformation info;
		reader->getFileInformation(info);

		// Print information for every track read
		for (const auto& trackProperties : info.trackInformation)
		{
			const auto sequenceId = trackProperties.trackId;
			//cout << "Track ID " << sequenceId << endl;  // Context ID corresponds to the track ID

			if (trackProperties.features & TrackFeatureEnum::IsMasterImageSequence)
			{
				//cout << "This is a master image sequence." << endl;
			}

			if (trackProperties.features & TrackFeatureEnum::IsThumbnailImageSequence)
			{
				// Assume there is only one type track reference, so check reference type and master track ID(s) from
				// the first one.
				const auto tref = trackProperties.referenceTrackIds[0];
				//cout << "Track reference type is '" << tref.type.value << "'" << endl;
				//cout << "This is a thumbnail track for track ID ";
				/*
				for (const auto masterTrackId : tref.trackIds)
				{
					//cout << masterTrackId << endl;
				}
				*/
			}

			Array<TimestampIDPair> timestamps;
			reader->getItemTimestamps(sequenceId, timestamps);
			//cout << "Sample timestamps:" << endl;
			/*
			for (const auto& timestamp : timestamps)
			{
				//cout << " Timestamp=" << timestamp.timeStamp << "ms, sample ID=" << timestamp.itemId << endl;
			}
			*/
			nbId = timestamps.size;
		}

		if (nbId == 0)
		{
			nbId = 1;
		}
	}

	Reader::Destroy(reader);
	return nbId;
}

void CHeic::DecodePictureMultiThread(void* parameter)
{
	auto decoding = static_cast<x265Frame*>(parameter);

	decoding->picture = DecodeFrame(decoding->_memoryBuffer, decoding->memoryBufferSize);


	delete[] decoding->_memoryBuffer;
}

struct mytask
{
	mytask(x265Frame* frame)
	{
		this->frame = frame;
	}


	void ApplyFilter()
	{
		frame->picture = DecodeFrame(frame->_memoryBuffer, frame->memoryBufferSize);

		delete[] frame->_memoryBuffer;
		frame->_memoryBuffer = nullptr;
	}

	void operator()()
	{
		frame->picture = DecodeFrame(frame->_memoryBuffer, frame->memoryBufferSize);

		delete[] frame->_memoryBuffer;
		frame->_memoryBuffer = nullptr;
	}

	cv::Mat GetFrame()
	{
		return frame->picture;
	}

	void Free()
	{
		if (frame->_memoryBuffer != nullptr)
			delete[] frame->_memoryBuffer;
		frame->_memoryBuffer = nullptr;

		if (frame != nullptr)
			delete frame;

		frame = nullptr;
	}

	x265Frame* frame;
};

cv::Mat CHeic::GetPicture(const string& filename, int& orientation)
{
	struct PictureEncoder
	{
		x265Frame* frame = nullptr;
	};
	cv::Mat picture;
	auto* reader = Reader::Create();

	// Input file available from https://github.com/nokiatech/heif_conformance
	if (reader->initialize(filename.c_str()) == ErrorCode::OK)
	{
		FileInformation info;
		if (reader->getFileInformation(info) != ErrorCode::OK)
		{
			goto END;
		}

		// Find the item ID
		ImageId itemId;
		reader->getPrimaryItem(itemId);

		Array<ItemPropertyInfo> propertyInfos;
		reader->getItemProperties(itemId, propertyInfos);

		unsigned int rotation = 0;
		for (const auto& property : propertyInfos)
		{
			// For example, handle 'irot' transformational property is anti-clockwise rotation
			if (property.type == ItemPropertyType::IROT)
			{
				// Get property struct by index to access rotation angle
				Rotate irot;
				reader->getProperty(property.index, irot);
				rotation = irot.angle;
				break; // Assume only one property
			}
		}

		uint32_t _width, _heigth;
		reader->getWidth(itemId, _width);
		reader->getHeight(itemId, _heigth);
		uint64_t memoryBufferSize = 4096 * 4096;
		auto memoryBuffer = new uint8_t[memoryBufferSize];
		ErrorCode err = reader->getItemDataWithDecoderParameters(itemId, memoryBuffer, memoryBufferSize);
		if (err == ErrorCode::OK)
		{
			picture = DecodeFrame(memoryBuffer, memoryBufferSize);
			orientation = rotation;
		}
		else
		{
			unsigned int rotation = 0;
			vector<PictureEncoder> listPicture;
			// Input file available from https://github.com/nokiatech/heif_conformance
			Array<ImageId> itemIds;
			// Find item IDs of master images
			reader->getMasterImages(itemIds);
			//bool firstTime = true;
			// Find thumbnails for each master. There can be several thumbnails for one master image, get narrowest ones here.

			for (const auto masterId : itemIds)
			{
				Array<ItemPropertyInfo> propertyInfos;
				reader->getItemProperties(masterId, propertyInfos);

				for (const auto& property : propertyInfos)
				{
					// For example, handle 'irot' transformational property is anti-clockwise rotation
					if (property.type == ItemPropertyType::IROT)
					{
						// Get property struct by index to access rotation angle
						Rotate irot;
						reader->getProperty(property.index, irot);
						rotation = irot.angle;
						break; // Assume only one property
					}
				}
				break;
			}


#ifdef USE_TBB

			std::vector<mytask> tasks;

			for (const auto masterId : itemIds)
			{
				auto frame = new x265Frame;
				frame->memoryBufferSize = 1024 * 1024;
				frame->_memoryBuffer = new uint8_t[memoryBufferSize];
				frame->filename = filename;
				reader->getItemDataWithDecoderParameters(masterId, frame->_memoryBuffer, frame->memoryBufferSize);
				tasks.push_back(mytask(frame));
			}


			tbb::parallel_for(0, (int)tasks.size(), 1, [=](int k)
				{
					mytask task = tasks[k];
					task.ApplyFilter();
				});

			if (tasks.size() > 0)
			{
				ImageId itemId;
				reader->getPrimaryItem(itemId);

				Array<ItemPropertyInfo> propertyInfos;
				reader->getItemProperties(itemId, propertyInfos);

				uint32_t _width, _heigth;
				reader->getWidth(itemId, _width);
				reader->getHeight(itemId, _heigth);

				cv::Mat bitmapSrc = tasks[0].GetFrame();
				if (!bitmapSrc.empty())
				{
					int boxWidth = bitmapSrc.size().width;
					int boxHeight = bitmapSrc.size().height;

					int nbItemWidth = _width / boxWidth;
					if (nbItemWidth * boxWidth < _width)
						nbItemWidth++;

					int nbItemHeight = _heigth / boxHeight;
					if (nbItemHeight * boxHeight < _heigth)
						nbItemHeight++;

					cv::Mat out = cv::Mat(boxHeight * nbItemHeight, boxWidth * nbItemWidth, CV_8UC4);
					int x = 0;
					int y = (nbItemHeight * boxHeight) - boxHeight;

					for (mytask task : tasks)
					{
						cv::Mat src = task.GetFrame();
						src.copyTo(out(cv::Rect(x, y, src.cols, src.rows)));
						//out->InsertBitmap(task.GetFrame(), x, y, false);
						x += boxWidth;

						if (x > _width)
						{
							x = 0;
							y -= boxHeight;
						}
					}

					picture = out(cv::Rect(0, boxHeight * nbItemHeight - _heigth, _width, _heigth));

					listPicture.clear();

				}
			}


			for (mytask task : tasks)
			{
				task.Free();
			}

			tasks.clear();

#else


			for (const auto masterId : itemIds)
			{
				PictureEncoder picture;
				listPicture.push_back(picture);
			}

			int i = 0;
			int nbProcess = thread::hardware_concurrency();;

			vector<thread *> listThread;
			for (int j = 0; j < nbProcess; j++)
			{
				listThread.push_back(nullptr);
			}


			while (i < listPicture.size())
			{
				for (int j = 0; j < nbProcess; j++)
				{
					if (listThread[j] != nullptr)
					{
						listThread[j]->join();
						delete listThread[j];
						listThread[j] = nullptr;
					}
				}

				if ((i + 1) < listPicture.size())
				{
					for (int j = 0; j < nbProcess; j++)
					{
						if (listThread[j] == nullptr)
						{
							listPicture[i].frame = new x265Frame;
							listPicture[i].frame->memoryBufferSize = 1024 * 1024;
							listPicture[i].frame->_memoryBuffer = new uint8_t[memoryBufferSize];
							listPicture[i].frame->filename = filename;
							reader->getItemDataWithDecoderParameters(itemIds[i], listPicture[i].frame->_memoryBuffer, listPicture[i].frame->memoryBufferSize);
							listThread[j] = new thread(DecodePictureMultiThread, listPicture[i].frame);
							i++;
							if (!(i < listPicture.size()))
								break;
						}
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}


			for (int j = 0; j < nbProcess; j++)
			{
				if (listThread[j] != nullptr)
				{
					listThread[j]->join();
					delete listThread[j];
					listThread[j] = nullptr;
				}
			}



			if (listPicture.size() > 0)
			{
				ImageId itemId;
				reader->getPrimaryItem(itemId);

				Array<ItemPropertyInfo> propertyInfos;
				reader->getItemProperties(itemId, propertyInfos);

				uint32_t _width, _heigth;
				reader->getWidth(itemId, _width);
				reader->getHeight(itemId, _heigth);

				CRegardsBitmap * bitmapSrc = listPicture[0].frame->picture;
				int boxWidth = bitmapSrc->GetBitmapWidth();
				int boxHeight = bitmapSrc->GetBitmapHeight();

				int nbItemWidth = _width / boxWidth;
				if (nbItemWidth * boxWidth < _width)
					nbItemWidth++;

				int nbItemHeight = _heigth / boxHeight;
				if (nbItemHeight * boxHeight < _heigth)
					nbItemHeight++;

				CRegardsBitmap * out = new CRegardsBitmap(boxWidth * nbItemWidth, boxHeight * nbItemHeight);
				int x = 0;
				int y = (nbItemHeight * boxHeight) - boxHeight;

				for (int i = 0; i < listPicture.size(); i++)
				{
					out->InsertBitmap(listPicture[i].frame->picture, x, y, false);
					x += boxWidth;

					if (x > _width)
					{
						x = 0;
						y -= boxHeight;
					}
				}

				picture = out->CropBitmap(0, boxHeight * nbItemHeight - _heigth, _width, _heigth);

				for (PictureEncoder picture : listPicture)
				{
					if(picture.frame->picture != nullptr)
						delete picture.frame->picture;

					delete picture.frame;
				}

				delete out;
				listPicture.clear();
				picture->SetFilename(filename);

			}
#endif
		}

		if (memoryBuffer != nullptr)
			delete[] memoryBuffer;
		memoryBuffer = nullptr;
	}
	else
	{
		cout << "Can't find input file: " << filename << ". "
			<< "Please download it from https://github.com/nokiatech/heif_conformance "
			<< "and place it in same directory with the executable." << endl;
	}


END:


	Reader::Destroy(reader);


	return picture;
}

void CHeic::GetPictureDimension(const string& filename, int& width, int& height)
{
	Array<ImageId> itemIds;
	auto* reader = Reader::Create();
	if (reader->initialize(filename.c_str()) == ErrorCode::OK)
	{
		FileInformation info;
		if (reader->getFileInformation(info) != ErrorCode::OK)
		{
			return;
		}

		// Verify that the file has one or several images in the MetaBox
		if (!(info.features & FileFeatureEnum::HasSingleImage || info.features & FileFeatureEnum::HasImageCollection))
		{
			return;
		}

		// Find the item ID
		// Find the item ID of the first master image
		ImageId itemId;
		reader->getPrimaryItem(itemId);

		uint32_t _width, _heigth;
		reader->getWidth(itemId, _width);
		reader->getHeight(itemId, _heigth);
		width = _width;
		height = _heigth;
	}
	else
	{
		width = 0;
		height = 0;
	}
}

cv::Mat CHeic::GetThumbnailPicture(const string& filename, int & orientation)
{
	cv::Mat picture;
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

		Array<ItemPropertyInfo> propertyInfos;
		reader->getItemProperties(itemId, propertyInfos);

		unsigned int rotation = 0;
		for (const auto& property : propertyInfos)
		{
			// For example, handle 'irot' transformational property is anti-clockwise rotation
			if (property.type == ItemPropertyType::IROT)
			{
				// Get property struct by index to access rotation angle
				Rotate irot;
				reader->getProperty(property.index, irot);
				rotation = irot.angle;
				break; // Assume only one property
			}
		}

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
				picture = DecodeFrame(itemData, itemSize);
				orientation = rotation;
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

void CHeic::GetMetadata(const string& filename, uint8_t*& data, unsigned int& size)
{
	auto* reader = Reader::Create();

	if (reader->initialize(filename.c_str()) != ErrorCode::OK)
	{
		cout << "Can't find input file: " << filename << ". "
			<< "Please download it from https://github.com/nokiatech/heif_conformance "
			<< "and place it in same directory with the executable." << endl;
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
	if (metadataIds.size > 0)
	{
		ImageId exifItemId = metadataIds[0];

		// Optional: verify the item ID we got is really of "Exif" type.
		FourCC itemType;
		reader->getItemType(exifItemId, itemType);
		if (itemType != "Exif")
		{
			return;
		}

		// Get item size from parsed information. For simplicity, assume it is the first and only non-image item in the
		// file.
		uint64_t itemSize = 1024 * 1024;
		for (ItemInformation itemInfo : fileInfo.rootMetaBoxInformation.itemInformations)
		{
			if (itemInfo.itemId != exifItemId)
			{
				continue;
			}
			itemSize = itemInfo.size;
		}
		// Request item data.
		if (size > 0)
		{
			/* raw EXIF header data */
			static const unsigned char exif_header[] = {
				0xff, 0xd8, 0xff, 0xe1
			};
			/* length of data in exif_header */
			static const unsigned int exif_header_len = sizeof(exif_header);

			auto memoryBuffer = new uint8_t[itemSize];
			reader->getItemData(metadataIds[0], memoryBuffer, itemSize);

			char value = 0;
			int pos = 0;
			memcpy(data, exif_header, exif_header_len);
			pos += exif_header_len;
			value = ((itemSize + 2) >> 8);
			memcpy(data + pos, &value, 1);
			pos++;
			value = (itemSize + 2) & 0xff;
			memcpy(data + pos, &value, 1);
			pos++;
			memcpy(data + pos, memoryBuffer + 4, itemSize - 4);
			pos += itemSize - 4;
			memcpy(data + pos, image_jpg + image_data_offset, image_data_len);
			delete[] memoryBuffer;
		}
		else
			size = itemSize + 512;
	}

	Reader::Destroy(reader);
}



#endif
