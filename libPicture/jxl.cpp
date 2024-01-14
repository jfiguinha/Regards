#include <header.h>
#include "jxl.h"
#include "jxl/decode.h"
#include "jxl/decode_cxx.h"
#include "jxl/resizable_parallel_runner.h"
#include "jxl/resizable_parallel_runner_cxx.h"
#include "jxl/encode.h"
#include "jxl/encode_cxx.h"
#include "jxl/thread_parallel_runner.h"
#include "jxl/thread_parallel_runner_cxx.h"
#include "picture_utility.h"
#include <ConvertUtility.h>

/** Decodes JPEG XL image to floating point pixels and ICC Profile. Pixel are
 * stored as floating point, as interleaved RGBA (4 floating point values per
 * pixel), line per line from top to bottom.  Pixel values have nominal range
 * 0..1 but may go beyond this range for HDR or wide gamut. The ICC profile
 * describes the color format of the pixel data.
 */
bool CJxl::DecodeJpegXlOneShot(const uint8_t* jxl, size_t size,
                               cv::Mat& matFloat, size_t& xsize,
                               size_t& ysize, std::vector<uint8_t>* icc_profile)
{
	// Multi-threaded parallel runner.
	// Multi-threaded parallel runner.
	auto runner = JxlResizableParallelRunnerMake(nullptr);

	auto dec = JxlDecoderMake(nullptr);
	if (JXL_DEC_SUCCESS !=
		JxlDecoderSubscribeEvents(dec.get(), JXL_DEC_BASIC_INFO |
		                          JXL_DEC_COLOR_ENCODING |
		                          JXL_DEC_FULL_IMAGE))
	{
		fprintf(stderr, "JxlDecoderSubscribeEvents failed\n");
		return false;
	}

	if (JXL_DEC_SUCCESS != JxlDecoderSetParallelRunner(dec.get(),
	                                                   JxlResizableParallelRunner,
	                                                   runner.get()))
	{
		fprintf(stderr, "JxlDecoderSetParallelRunner failed\n");
		return false;
	}

	JxlBasicInfo info;
	JxlPixelFormat format = {4, JXL_TYPE_FLOAT, JXL_NATIVE_ENDIAN, 0};

	JxlDecoderSetInput(dec.get(), jxl, size);

	for (;;)
	{
		JxlDecoderStatus status = JxlDecoderProcessInput(dec.get());

		if (status == JXL_DEC_ERROR)
		{
			fprintf(stderr, "Decoder error\n");
			return false;
		}
		if (status == JXL_DEC_NEED_MORE_INPUT)
		{
			fprintf(stderr, "Error, already provided all input\n");
			return false;
		}
		if (status == JXL_DEC_BASIC_INFO)
		{
			if (JXL_DEC_SUCCESS != JxlDecoderGetBasicInfo(dec.get(), &info))
			{
				fprintf(stderr, "JxlDecoderGetBasicInfo failed\n");
				return false;
			}
			xsize = info.xsize;
			ysize = info.ysize;
			JxlResizableParallelRunnerSetThreads(
				runner.get(),
				JxlResizableParallelRunnerSuggestThreads(info.xsize, info.ysize));
		}
		else if (status == JXL_DEC_COLOR_ENCODING)
		{
			// Get the ICC color profile of the pixel data
			size_t icc_size;
#ifdef __APPLE__
			if (JXL_DEC_SUCCESS !=
				JxlDecoderGetICCProfileSize(
					dec.get(), JXL_COLOR_PROFILE_TARGET_DATA, &icc_size))
			{
				fprintf(stderr, "JxlDecoderGetICCProfileSize failed\n");
				return false;
			}
            
            icc_profile->resize(icc_size);
			if (JXL_DEC_SUCCESS != JxlDecoderGetColorAsICCProfile(
				dec.get(),
				JXL_COLOR_PROFILE_TARGET_DATA,
				icc_profile->data(), icc_profile->size()))
			{
				fprintf(stderr, "JxlDecoderGetColorAsICCProfile failed\n");
				return false;
			}
#else
			if (JXL_DEC_SUCCESS !=
				JxlDecoderGetICCProfileSize(
					dec.get(), &format, JXL_COLOR_PROFILE_TARGET_DATA, &icc_size))
			{
				fprintf(stderr, "JxlDecoderGetICCProfileSize failed\n");
				return false;
			}
            
            icc_profile->resize(icc_size);
			if (JXL_DEC_SUCCESS != JxlDecoderGetColorAsICCProfile(
				dec.get(), &format,
				JXL_COLOR_PROFILE_TARGET_DATA,
				icc_profile->data(), icc_profile->size()))
			{
				fprintf(stderr, "JxlDecoderGetColorAsICCProfile failed\n");
				return false;
			}
#endif

		}
		else if (status == JXL_DEC_NEED_IMAGE_OUT_BUFFER)
		{
			size_t buffer_size;
			if (JXL_DEC_SUCCESS !=
				JxlDecoderImageOutBufferSize(dec.get(), &format, &buffer_size))
			{
				fprintf(stderr, "JxlDecoderImageOutBufferSize failed\n");
				return false;
			}
			if (buffer_size != xsize * ysize * 16)
			{
				fprintf(stderr, "Invalid out buffer size %zu %zu\n", buffer_size,
				        xsize * ysize * 16);
				return false;
			}

			matFloat.create(ysize, xsize, CV_32FC4);

			//void* pixels_buffer = (void*)matFloat.data;
			size_t pixels_buffer_size = ysize * xsize * 4 * sizeof(float);
			if (JXL_DEC_SUCCESS != JxlDecoderSetImageOutBuffer(dec.get(), &format,
			                                                   matFloat.data,
			                                                   pixels_buffer_size))
			{
				fprintf(stderr, "JxlDecoderSetImageOutBuffer failed\n");
				return false;
			}
		}
		else if (status == JXL_DEC_FULL_IMAGE)
		{
			// Nothing to do. Do not yet return. If the image is an animation, more
			// full frames may be decoded. This example only keeps the last one.
		}
		else if (status == JXL_DEC_SUCCESS)
		{
			// All decoding successfully finished.
			// It's not required to call JxlDecoderReleaseInput(dec.get()) here since
			// the decoder will be destroyed.
			return true;
		}
		else
		{
			fprintf(stderr, "Unknown decoder status\n");
			return false;
		}
	}
}


/** Decodes JPEG XL image to floating point pixels and ICC Profile. Pixel are
 * stored as floating point, as interleaved RGBA (4 floating point values per
 * pixel), line per line from top to bottom.  Pixel values have nominal range
 * 0..1 but may go beyond this range for HDR or wide gamut. The ICC profile
 * describes the color format of the pixel data.
 */
void* CJxl::DecodeJpegDim(FILE* file)
{
	uint8_t* data = nullptr;
	size_t data_size = 0;
	// In how large chunks to read from the file and try decoding the basic info.
	const size_t chunk_size = 64;

	JxlDecoder* dec = JxlDecoderCreate(nullptr);
	if (!dec)
	{
		fprintf(stderr, "JxlDecoderCreate failed\n");
		return nullptr;
	}

	JxlDecoderSetKeepOrientation(dec, 1);

	if (JXL_DEC_SUCCESS !=
		JxlDecoderSubscribeEvents(
			dec, JXL_DEC_BASIC_INFO | JXL_DEC_COLOR_ENCODING | JXL_DEC_FRAME))
	{
		fprintf(stderr, "JxlDecoderSubscribeEvents failed\n");
		JxlDecoderDestroy(dec);
		return nullptr;
	}

	auto info = new JxlBasicInfo();
	int seen_basic_info = 0;
	JxlFrameHeader frame_header;

	for (;;)
	{
		// The first time, this will output JXL_DEC_NEED_MORE_INPUT because no
		// input is set yet, this is ok since the input is set when handling this
		// event.
		JxlDecoderStatus status = JxlDecoderProcessInput(dec);

		if (status == JXL_DEC_ERROR)
		{
			fprintf(stderr, "Decoder error\n");
			break;
		}
		if (status == JXL_DEC_NEED_MORE_INPUT)
		{
			// The first time there is nothing to release and it returns 0, but that
			// is ok.
			size_t remaining = JxlDecoderReleaseInput(dec);
			// move any remaining bytes to the front if necessary
			if (remaining != 0)
			{
				memmove(data, data + data_size - remaining, remaining);
			}
			// resize the buffer to append one more chunk of data
			// TODO(lode): avoid unnecessary reallocations
			data = static_cast<uint8_t*>(realloc(data, remaining + chunk_size));
			// append bytes read from the file behind the remaining bytes
			size_t read_size = fread(data + remaining, 1, chunk_size, file);
			if (read_size == 0 && feof(file))
			{
				fprintf(stderr, "Unexpected EOF\n");
				break;
			}
			data_size = remaining + read_size;
			JxlDecoderSetInput(dec, data, data_size);
		}
		else if (status == JXL_DEC_SUCCESS)
		{
			// Finished all processing.
			break;
		}
		else if (status == JXL_DEC_BASIC_INFO)
		{
			if (JXL_DEC_SUCCESS != JxlDecoderGetBasicInfo(dec, info))
			{
				fprintf(stderr, "JxlDecoderGetBasicInfo failed\n");
				break;
			}

			seen_basic_info = 1;
			printf("dimensions: %ux%u\n", info->xsize, info->ysize);
			printf("have_container: %d\n", info->have_container);
			printf("uses_original_profile: %d\n", info->uses_original_profile);
			printf("bits_per_sample: %d\n", info->bits_per_sample);
			if (info->exponent_bits_per_sample)
				printf("float, with exponent_bits_per_sample: %d\n",
				       info->exponent_bits_per_sample);
			if (info->intensity_target != 255.f || info->min_nits != 0.f ||
				info->relative_to_max_display != 0 ||
				info->relative_to_max_display != 0.f)
			{
				printf("intensity_target: %f\n", info->intensity_target);
				printf("min_nits: %f\n", info->min_nits);
				printf("relative_to_max_display: %d\n", info->relative_to_max_display);
				printf("linear_below: %f\n", info->linear_below);
			}
			printf("have_preview: %d\n", info->have_preview);
			if (info->have_preview)
			{
				printf("preview xsize: %u\n", info->preview.xsize);
				printf("preview ysize: %u\n", info->preview.ysize);
			}
			printf("have_animation: %d\n", info->have_animation);
			if (info->have_animation)
			{
				printf("ticks per second (numerator / denominator): %u / %u\n",
				       info->animation.tps_numerator, info->animation.tps_denominator);
				printf("num_loops: %u\n", info->animation.num_loops);
				printf("have_timecodes: %d\n", info->animation.have_timecodes);
			}
			const char* const orientation_string[8] = {
				"Normal", "Flipped horizontally",
				"Upside down", "Flipped vertically",
				"Transposed", "90 degrees clockwise",
				"Anti-Transposed", "90 degrees counter-clockwise"
			};
			if (info->orientation > 0 && info->orientation < 9)
			{
				printf("orientation: %d (%s)\n", info->orientation,
				       orientation_string[info->orientation - 1]);
			}
			else
			{
				fprintf(stderr, "Invalid orientation\n");
			}
			printf("num_extra_channels: %d\n", info->num_extra_channels);

			const char* const ec_type_names[7] = {
				"Alpha", "Depth",
				"Spot color", "Selection mask",
				"K (of CMYK)", "CFA (Bayer data)",
				"Thermal"
			};
			for (uint32_t i = 0; i < info->num_extra_channels; i++)
			{
				JxlExtraChannelInfo extra;
				if (JXL_DEC_SUCCESS != JxlDecoderGetExtraChannelInfo(dec, i, &extra))
				{
					fprintf(stderr, "JxlDecoderGetExtraChannelInfo failed\n");
					break;
				}
				printf("extra channel %u:\n", i);
				printf("  type: %s\n",
				       (extra.type < 7
					        ? ec_type_names[extra.type]
					        : (extra.type == JXL_CHANNEL_OPTIONAL
						           ? "Unknown but can be ignored"
						           : "Unknown, please update your libjxl")));
				printf("  bits_per_sample: %u\n", extra.bits_per_sample);
				if (extra.exponent_bits_per_sample > 0)
				{
					printf("  float, with exponent_bits_per_sample: %u\n",
					       extra.exponent_bits_per_sample);
				}
				if (extra.dim_shift > 0)
				{
					printf("  dim_shift: %u (upsampled %ux)\n", extra.dim_shift,
					       1 << extra.dim_shift);
				}
				if (extra.name_length)
				{
					auto name = static_cast<char*>(malloc(extra.name_length + 1));
					if (JXL_DEC_SUCCESS != JxlDecoderGetExtraChannelName(
						dec, i, name, extra.name_length + 1))
					{
						fprintf(stderr, "JxlDecoderGetExtraChannelName failed\n");
						free(name);
						break;
					}
					free(name);
					printf("  name: %s\n", name);
				}
				if (extra.type == JXL_CHANNEL_ALPHA)
					printf("  alpha_premultiplied: %d (%s)\n", extra.alpha_premultiplied,
					       extra.alpha_premultiplied
						       ? "Premultiplied"
						       : "Non-premultiplied");
				if (extra.type == JXL_CHANNEL_SPOT_COLOR)
				{
					printf("  spot_color: (%f, %f, %f) with opacity %f\n",
					       extra.spot_color[0], extra.spot_color[1], extra.spot_color[2],
					       extra.spot_color[3]);
				}
				if (extra.type == JXL_CHANNEL_CFA)
					printf("  cfa_channel: %u\n", extra.cfa_channel);
			}
		}
		else if (status == JXL_DEC_COLOR_ENCODING)
		{
			JxlPixelFormat format = {4, JXL_TYPE_FLOAT, JXL_LITTLE_ENDIAN, 0};
			printf("color profile:\n");

#ifdef __APPLE__
			JxlColorEncoding color_encoding;
			if (JXL_DEC_SUCCESS ==
				JxlDecoderGetColorAsEncodedProfile(dec,
				                                   JXL_COLOR_PROFILE_TARGET_ORIGINAL,
				                                   &color_encoding))
#else
			JxlColorEncoding color_encoding;
			if (JXL_DEC_SUCCESS ==
				JxlDecoderGetColorAsEncodedProfile(dec, &format,
				                                   JXL_COLOR_PROFILE_TARGET_ORIGINAL,
				                                   &color_encoding))
#endif
			{
				printf("  format: JPEG XL encoded color profile\n");
				const char* const cs_string[4] = {
					"RGB color", "Grayscale", "XYB",
					"Unknown"
				};
				const char* const wp_string[12] = {
					"", "D65", "Custom", "", "", "",
					"", "", "", "", "E", "P3"
				};
				const char* const pr_string[12] = {
					"", "sRGB", "Custom", "", "", "", "", "", "", "Rec.2100", "", "P3"
				};
				const char* const tf_string[19] = {
					"", "709", "Unknown", "", "", "", "", "", "Linear", "",
					"", "", "", "sRGB", "", "", "PQ", "DCI", "HLG"
				};
				const char* const ri_string[4] = {
					"Perceptual", "Relative",
					"Saturation", "Absolute"
				};
				printf("  color_space: %d (%s)\n", color_encoding.color_space,
				       cs_string[color_encoding.color_space]);
				printf("  white_point: %d (%s)\n", color_encoding.white_point,
				       wp_string[color_encoding.white_point]);
				if (color_encoding.white_point == JXL_WHITE_POINT_CUSTOM)
				{
					printf("  white_point XY: %f %f\n", color_encoding.white_point_xy[0],
					       color_encoding.white_point_xy[1]);
				}
				if (color_encoding.color_space == JXL_COLOR_SPACE_RGB ||
					color_encoding.color_space == JXL_COLOR_SPACE_UNKNOWN)
				{
					printf("  primaries: %d (%s)\n", color_encoding.primaries,
					       pr_string[color_encoding.primaries]);
					if (color_encoding.primaries == JXL_PRIMARIES_CUSTOM)
					{
						printf("  red primaries XY: %f %f\n",
						       color_encoding.primaries_red_xy[0],
						       color_encoding.primaries_red_xy[1]);
						printf("  green primaries XY: %f %f\n",
						       color_encoding.primaries_green_xy[0],
						       color_encoding.primaries_green_xy[1]);
						printf("  blue primaries XY: %f %f\n",
						       color_encoding.primaries_blue_xy[0],
						       color_encoding.primaries_blue_xy[1]);
					}
				}
				if (color_encoding.transfer_function == JXL_TRANSFER_FUNCTION_GAMMA)
				{
					printf("  transfer_function: gamma: %f\n", color_encoding.gamma);
				}
				else
				{
					printf("  transfer_function: %d (%s)\n",
					       color_encoding.transfer_function,
					       tf_string[color_encoding.transfer_function]);
				}
				printf("  rendering_intent: %d (%s)\n", color_encoding.rendering_intent,
				       ri_string[color_encoding.rendering_intent]);
			}
			else
			{
				// The profile is not in JPEG XL encoded form, get as ICC profile
				// instead.
				printf("  format: ICC profile\n");
				size_t profile_size;
#ifdef __APPLE__
				if (JXL_DEC_SUCCESS !=
					JxlDecoderGetICCProfileSize(dec,
					                            JXL_COLOR_PROFILE_TARGET_ORIGINAL,
					                            &profile_size))
				{
					fprintf(stderr, "JxlDecoderGetICCProfileSize failed\n");
					continue;
				}
#else
				if (JXL_DEC_SUCCESS !=
					JxlDecoderGetICCProfileSize(dec, &format,
					                            JXL_COLOR_PROFILE_TARGET_ORIGINAL,
					                            &profile_size))
				{
					fprintf(stderr, "JxlDecoderGetICCProfileSize failed\n");
					continue;
				}
#endif
				printf("  ICC profile size: %zu\n", profile_size);
				if (profile_size < 132)
				{
					fprintf(stderr, "ICC profile too small\n");
					continue;
				}
				auto profile = static_cast<uint8_t*>(malloc(profile_size));
#ifdef __APPLE__
				if (JXL_DEC_SUCCESS !=
					JxlDecoderGetColorAsICCProfile(dec,
					                               JXL_COLOR_PROFILE_TARGET_ORIGINAL,
					                               profile, profile_size))
				{
					fprintf(stderr, "JxlDecoderGetColorAsICCProfile failed\n");
					free(profile);
					continue;
				}
#else
				if (JXL_DEC_SUCCESS !=
					JxlDecoderGetColorAsICCProfile(dec, &format,
					                               JXL_COLOR_PROFILE_TARGET_ORIGINAL,
					                               profile, profile_size))
				{
					fprintf(stderr, "JxlDecoderGetColorAsICCProfile failed\n");
					free(profile);
					continue;
				}
#endif
				printf("  CMM type: \"%.4s\"\n", profile + 4);
				printf("  color space: \"%.4s\"\n", profile + 16);
				printf("  rendering intent: %d\n", static_cast<int>(profile[67]));
				free(profile);
			}
		}
		else if (status == JXL_DEC_FRAME)
		{
			if (JXL_DEC_SUCCESS != JxlDecoderGetFrameHeader(dec, &frame_header))
			{
				fprintf(stderr, "JxlDecoderGetFrameHeader failed\n");
				break;
			}
			printf("frame:\n");
			if (frame_header.name_length)
			{
				auto name = static_cast<char*>(malloc(frame_header.name_length + 1));
				if (JXL_DEC_SUCCESS !=
					JxlDecoderGetFrameName(dec, name, frame_header.name_length + 1))
				{
					fprintf(stderr, "JxlDecoderGetFrameName failed\n");
					free(name);
					break;
				}
				free(name);
				printf("  name: %s\n", name);
			}
			float ms = frame_header.duration * 1000.f *
				info->animation.tps_denominator / info->animation.tps_numerator;
			if (info->have_animation)
			{
				printf("  Duration: %u ticks (%f ms)\n", frame_header.duration, ms);
				if (info->animation.have_timecodes)
				{
					printf("  Time code: %X\n", frame_header.timecode);
				}
			}

			// This is the last expected event, no need to read the rest of the file.
		}
		else
		{
			fprintf(stderr, "Unexpected decoder status\n");
			break;
		}
	}

	JxlDecoderDestroy(dec);
	free(data);

	return info;
}

/**
 * Compresses the provided pixels.
 *
 * @param pixels input pixels
 * @param xsize width of the input image
 * @param ysize height of the input image
 * @param compressed will be populated with the compressed bytes
 */
bool CJxl::EncodeJxlOneshot(cv::Mat& matFloat, std::vector<uint8_t>* compressed)
{
	auto enc = JxlEncoderMake(/*memory_manager=*/nullptr);
	auto runner = JxlThreadParallelRunnerMake(
		/*memory_manager=*/nullptr,
		                   JxlThreadParallelRunnerDefaultNumWorkerThreads());
	if (JXL_ENC_SUCCESS != JxlEncoderSetParallelRunner(enc.get(),
	                                                   JxlThreadParallelRunner,
	                                                   runner.get()))
	{
		fprintf(stderr, "JxlEncoderSetParallelRunner failed\n");
		return false;
	}

	JxlPixelFormat pixel_format = {3, JXL_TYPE_FLOAT, JXL_NATIVE_ENDIAN, 0};

	JxlBasicInfo basic_info;
	JxlEncoderInitBasicInfo(&basic_info);
	basic_info.xsize = matFloat.size().width;
	basic_info.ysize = matFloat.size().height;
	basic_info.bits_per_sample = 32;
	basic_info.exponent_bits_per_sample = 8;
	basic_info.uses_original_profile = JXL_FALSE;
	if (JXL_ENC_SUCCESS != JxlEncoderSetBasicInfo(enc.get(), &basic_info))
	{
		fprintf(stderr, "JxlEncoderSetBasicInfo failed\n");
		return false;
	}

	JxlColorEncoding color_encoding = {};
	JxlColorEncodingSetToSRGB(&color_encoding,
	                          /*is_gray=*/pixel_format.num_channels < 3);
	if (JXL_ENC_SUCCESS !=
		JxlEncoderSetColorEncoding(enc.get(), &color_encoding))
	{
		fprintf(stderr, "JxlEncoderSetColorEncoding failed\n");
		return false;
	}
#ifdef __APPLE__
	if (JXL_ENC_SUCCESS !=
		JxlEncoderAddImageFrame(JxlEncoderFrameSettingsCreate(enc.get(), nullptr),
		                        &pixel_format, matFloat.data,
		                        matFloat.rows * matFloat.cols * 4 * sizeof(float)))
	{
		fprintf(stderr, "JxlEncoderAddImageFrame failed\n");
		return false;
	}
#else
	if (JXL_ENC_SUCCESS !=
		JxlEncoderAddImageFrame(JxlEncoderOptionsCreate(enc.get(), nullptr),
		                        &pixel_format, matFloat.data,
		                        matFloat.rows * matFloat.cols * 4 * sizeof(float)))
	{
		fprintf(stderr, "JxlEncoderAddImageFrame failed\n");
		return false;
	}
#endif
	compressed->resize(64);
	uint8_t* next_out = compressed->data();
	size_t avail_out = compressed->size() - (next_out - compressed->data());
	JxlEncoderStatus process_result = JXL_ENC_NEED_MORE_OUTPUT;
	while (process_result == JXL_ENC_NEED_MORE_OUTPUT)
	{
		process_result = JxlEncoderProcessOutput(enc.get(), &next_out, &avail_out);
		if (process_result == JXL_ENC_NEED_MORE_OUTPUT)
		{
			size_t offset = next_out - compressed->data();
			compressed->resize(compressed->size() * 2);
			next_out = compressed->data() + offset;
			avail_out = compressed->size() - offset;
		}
	}
	compressed->resize(next_out - compressed->data());
	if (JXL_ENC_SUCCESS != process_result)
	{
		fprintf(stderr, "JxlEncoderProcessOutput failed\n");
		return false;
	}

	return true;
}

void CJxl::WriteFile(cv::Mat& matFloat, const wxString& path)
{
	std::vector<uint8_t> compressed;
	EncodeJxlOneshot(matFloat, &compressed);
	CPictureUtility::writefile(path, compressed.data(), compressed.size());
}

void CJxl::GetDimensions(const wxString& jxl_filename, int& width, int& height)
{
	FILE* file = fopen(CConvertUtility::ConvertToUTF8(jxl_filename), "rb");
	if (!file)
	{
		fprintf(stderr, "Failed to read file %s\n", CConvertUtility::ConvertToUTF8(jxl_filename));
		return;
	}

	auto info = static_cast<JxlBasicInfo*>(DecodeJpegDim(file));

	if (info == nullptr)
	{
		fprintf(stderr, "Couldn't print basic info\n");
		return;
	}
	width = info->xsize;
	height = info->ysize;
	delete info;
	fclose(file);
}

cv::Mat CJxl::GetPicture(const wxString& path)
{
	cv::Mat matFloat;
	std::vector<uint8_t> icc_profile;
	size_t xsize = 0, ysize = 0;
	size_t _jpegSize;
	uint8_t* _compressedImage = CPictureUtility::readfile(path, _jpegSize);
	if (!DecodeJpegXlOneShot(_compressedImage, _jpegSize, matFloat, xsize, ysize,
	                         &icc_profile))
		return matFloat;

	return matFloat;
}

void CJxl::GetMetadata(const wxString& filename, uint8_t*& data, unsigned int& size)
{
	// size_t _jpegSize;
	//  uint8_t* _compressedImage = CPictureUtility::readfile(filename, _jpegSize);
}
