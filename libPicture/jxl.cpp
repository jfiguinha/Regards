#include <header.h>
#include "jxl.h"
#include "RegardsFloatBitmap.h"
#include "jxl/decode.h"
#include "jxl/decode_cxx.h"
#include "jxl/resizable_parallel_runner.h"
#include "jxl/resizable_parallel_runner_cxx.h"
#include "jxl/encode.h"
#include "jxl/encode_cxx.h"
#include "jxl/thread_parallel_runner.h"
#include "jxl/thread_parallel_runner_cxx.h"
#include "picture_utility.h"

/** Decodes JPEG XL image to floating point pixels and ICC Profile. Pixel are
 * stored as floating point, as interleaved RGBA (4 floating point values per
 * pixel), line per line from top to bottom.  Pixel values have nominal range
 * 0..1 but may go beyond this range for HDR or wide gamut. The ICC profile
 * describes the color format of the pixel data.
 */
bool CJxl::DecodeJpegXlOneShot(const uint8_t* jxl, size_t size,
    CRegardsFloatBitmap * & bmp, size_t & xsize,
    size_t & ysize, std::vector<uint8_t>* icc_profile) {
    // Multi-threaded parallel runner.
    auto runner = JxlResizableParallelRunnerMake(nullptr);

    auto dec = JxlDecoderMake(nullptr);
    if (JXL_DEC_SUCCESS !=
        JxlDecoderSubscribeEvents(dec.get(), JXL_DEC_BASIC_INFO |
            JXL_DEC_COLOR_ENCODING |
            JXL_DEC_FULL_IMAGE)) {
        fprintf(stderr, "JxlDecoderSubscribeEvents failed\n");
        return false;
    }

    if (JXL_DEC_SUCCESS != JxlDecoderSetParallelRunner(dec.get(),
        JxlResizableParallelRunner,
        runner.get())) {
        fprintf(stderr, "JxlDecoderSetParallelRunner failed\n");
        return false;
    }

    JxlBasicInfo info;
    JxlPixelFormat format = { 4, JXL_TYPE_FLOAT, JXL_NATIVE_ENDIAN, 0 };

    JxlDecoderSetInput(dec.get(), jxl, size);
    JxlDecoderReleaseInput(dec.get());

    for (;;) {
        JxlDecoderStatus status = JxlDecoderProcessInput(dec.get());

        if (status == JXL_DEC_ERROR) {
            fprintf(stderr, "Decoder error\n");
            return false;
        }
        else if (status == JXL_DEC_NEED_MORE_INPUT) {
            fprintf(stderr, "Error, already provided all input\n");
            return false;
        }
        else if (status == JXL_DEC_BASIC_INFO) {
            if (JXL_DEC_SUCCESS != JxlDecoderGetBasicInfo(dec.get(), &info)) {
                fprintf(stderr, "JxlDecoderGetBasicInfo failed\n");
                return false;
            }
            xsize = info.xsize;
            ysize = info.ysize;
            JxlResizableParallelRunnerSetThreads(
                runner.get(),
                JxlResizableParallelRunnerSuggestThreads(info.xsize, info.ysize));
        }
        else if (status == JXL_DEC_COLOR_ENCODING) {
            // Get the ICC color profile of the pixel data
            size_t icc_size;
            if (JXL_DEC_SUCCESS !=
                JxlDecoderGetICCProfileSize(
                    dec.get(), &format, JXL_COLOR_PROFILE_TARGET_DATA, &icc_size)) {
                fprintf(stderr, "JxlDecoderGetICCProfileSize failed\n");
                return false;
            }
            icc_profile->resize(icc_size);
            if (JXL_DEC_SUCCESS != JxlDecoderGetColorAsICCProfile(
                dec.get(), &format,
                JXL_COLOR_PROFILE_TARGET_DATA,
                icc_profile->data(), icc_profile->size())) {
                fprintf(stderr, "JxlDecoderGetColorAsICCProfile failed\n");
                return false;
            }
        }
        else if (status == JXL_DEC_NEED_IMAGE_OUT_BUFFER) {
            size_t buffer_size;
            if (JXL_DEC_SUCCESS !=
                JxlDecoderImageOutBufferSize(dec.get(), &format, &buffer_size)) {
                fprintf(stderr, "JxlDecoderImageOutBufferSize failed\n");
                return false;
            }
            if (buffer_size != xsize * ysize * 16) {
                fprintf(stderr, "Invalid out buffer size \n");
                return false;
            }

            bmp->Resize(xsize,ysize);

            //pixels->resize(xsize * ysize * 4);
            void* pixels_buffer = (void*)bmp->data;
            size_t pixels_buffer_size = bmp->GetSize() * sizeof(float);
            if (JXL_DEC_SUCCESS != JxlDecoderSetImageOutBuffer(dec.get(), &format,
                pixels_buffer,
                pixels_buffer_size)) {
                fprintf(stderr, "JxlDecoderSetImageOutBuffer failed\n");
                return false;
            }
        }
        else if (status == JXL_DEC_FULL_IMAGE) {
            // Nothing to do. Do not yet return. If the image is an animation, more
            // full frames may be decoded. This example only keeps the last one.
        }
        else if (status == JXL_DEC_SUCCESS) {
            // All decoding successfully finished.
            // It's not required to call JxlDecoderReleaseInput(dec.get()) here since
            // the decoder will be destroyed.
            return true;
        }
        else {
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
bool CJxl::DecodeJpegDim(const uint8_t* jxl, size_t size,
    size_t& xsize,
    size_t& ysize) {
    // Multi-threaded parallel runner.
    auto runner = JxlResizableParallelRunnerMake(nullptr);

    auto dec = JxlDecoderMake(nullptr);
    if (JXL_DEC_SUCCESS !=
        JxlDecoderSubscribeEvents(dec.get(), JXL_DEC_BASIC_INFO |
            JXL_DEC_COLOR_ENCODING |
            JXL_DEC_FULL_IMAGE)) {
        fprintf(stderr, "JxlDecoderSubscribeEvents failed\n");
        return false;
    }

    if (JXL_DEC_SUCCESS != JxlDecoderSetParallelRunner(dec.get(),
        JxlResizableParallelRunner,
        runner.get())) {
        fprintf(stderr, "JxlDecoderSetParallelRunner failed\n");
        return false;
    }

    JxlBasicInfo info;
    JxlPixelFormat format = { 4, JXL_TYPE_FLOAT, JXL_NATIVE_ENDIAN, 0 };

    JxlDecoderSetInput(dec.get(), jxl, size);
    JxlDecoderReleaseInput(dec.get());

    for (;;) {
        JxlDecoderStatus status = JxlDecoderProcessInput(dec.get());

        if (status == JXL_DEC_ERROR) {
            fprintf(stderr, "Decoder error\n");
            return false;
        }
        else if (status == JXL_DEC_NEED_MORE_INPUT) {
            fprintf(stderr, "Error, already provided all input\n");
            return false;
        }
        else if (status == JXL_DEC_BASIC_INFO) {
            if (JXL_DEC_SUCCESS != JxlDecoderGetBasicInfo(dec.get(), &info)) {
                fprintf(stderr, "JxlDecoderGetBasicInfo failed\n");
                return false;
            }
            xsize = info.xsize;
            ysize = info.ysize;
            JxlResizableParallelRunnerSetThreads(
                runner.get(),
                JxlResizableParallelRunnerSuggestThreads(info.xsize, info.ysize));
            return true;
        }
        else if (status == JXL_DEC_FULL_IMAGE) {
            // Nothing to do. Do not yet return. If the image is an animation, more
            // full frames may be decoded. This example only keeps the last one.
        }
        else if (status == JXL_DEC_SUCCESS) {
            // All decoding successfully finished.
            // It's not required to call JxlDecoderReleaseInput(dec.get()) here since
            // the decoder will be destroyed.
            return true;
        }
        else {
            fprintf(stderr, "Unknown decoder status\n");
            return false;
        }
    }
}

/**
 * Compresses the provided pixels.
 *
 * @param pixels input pixels
 * @param xsize width of the input image
 * @param ysize height of the input image
 * @param compressed will be populated with the compressed bytes
 */
bool  CJxl::EncodeJxlOneshot(CRegardsFloatBitmap* im, std::vector<uint8_t>* compressed) {
    auto enc = JxlEncoderMake(/*memory_manager=*/nullptr);
    auto runner = JxlThreadParallelRunnerMake(
        /*memory_manager=*/nullptr,
        JxlThreadParallelRunnerDefaultNumWorkerThreads());
    if (JXL_ENC_SUCCESS != JxlEncoderSetParallelRunner(enc.get(),
        JxlThreadParallelRunner,
        runner.get())) {
        fprintf(stderr, "JxlEncoderSetParallelRunner failed\n");
        return false;
    }

    JxlPixelFormat pixel_format = { 3, JXL_TYPE_FLOAT, JXL_NATIVE_ENDIAN, 0 };

    JxlBasicInfo basic_info;
    JxlEncoderInitBasicInfo(&basic_info);
    basic_info.xsize = im->GetWidth();
    basic_info.ysize = im->GetHeight();
    basic_info.bits_per_sample = 32;
    basic_info.exponent_bits_per_sample = 8;
    basic_info.uses_original_profile = JXL_FALSE;
    if (JXL_ENC_SUCCESS != JxlEncoderSetBasicInfo(enc.get(), &basic_info)) {
        fprintf(stderr, "JxlEncoderSetBasicInfo failed\n");
        return false;
    }

    JxlColorEncoding color_encoding = {};
    JxlColorEncodingSetToSRGB(&color_encoding,
        /*is_gray=*/pixel_format.num_channels < 3);
    if (JXL_ENC_SUCCESS !=
        JxlEncoderSetColorEncoding(enc.get(), &color_encoding)) {
        fprintf(stderr, "JxlEncoderSetColorEncoding failed\n");
        return false;
    }

    if (JXL_ENC_SUCCESS !=
        JxlEncoderAddImageFrame(JxlEncoderOptionsCreate(enc.get(), nullptr),
            &pixel_format, (void*)im->data,
           im->GetSize())) {
        fprintf(stderr, "JxlEncoderAddImageFrame failed\n");
        return false;
    }

    compressed->resize(64);
    uint8_t* next_out = compressed->data();
    size_t avail_out = compressed->size() - (next_out - compressed->data());
    JxlEncoderStatus process_result = JXL_ENC_NEED_MORE_OUTPUT;
    while (process_result == JXL_ENC_NEED_MORE_OUTPUT) {
        process_result = JxlEncoderProcessOutput(enc.get(), &next_out, &avail_out);
        if (process_result == JXL_ENC_NEED_MORE_OUTPUT) {
            size_t offset = next_out - compressed->data();
            compressed->resize(compressed->size() * 2);
            next_out = compressed->data() + offset;
            avail_out = compressed->size() - offset;
        }
    }
    compressed->resize(next_out - compressed->data());
    if (JXL_ENC_SUCCESS != process_result) {
        fprintf(stderr, "JxlEncoderProcessOutput failed\n");
        return false;
    }

    return true;
}

void CJxl::WriteFile(CRegardsFloatBitmap* im, const wxString& path)
{
    std::vector<uint8_t> compressed;
    EncodeJxlOneshot(im, &compressed);
    CPictureUtility::writefile(path, compressed.data(), compressed.size());
}

void CJxl::GetDimensions(const wxString& path, int& width, int& height)
{
    size_t xsize = 0, ysize = 0;
    size_t _jpegSize;
    uint8_t* _compressedImage = CPictureUtility::readfile(path, _jpegSize);
    if (DecodeJpegDim(_compressedImage, _jpegSize, xsize, ysize))
    {
        width = xsize;
        height = ysize;
    }

}

CRegardsFloatBitmap* CJxl::GetPicture(const wxString& path)
{
    CRegardsFloatBitmap* bmp = new CRegardsFloatBitmap();
    std::vector<uint8_t> icc_profile;
    size_t xsize = 0, ysize = 0;
    size_t _jpegSize;
    uint8_t* _compressedImage = CPictureUtility::readfile(path, _jpegSize);
    if (!DecodeJpegXlOneShot(_compressedImage, _jpegSize, bmp, xsize,ysize,
        &icc_profile))
        return nullptr;
    
	return bmp;
}

void CJxl::GetMetadata(const wxString& filename, uint8_t*& data, unsigned int& size)
{
   // size_t _jpegSize;
  //  uint8_t* _compressedImage = CPictureUtility::readfile(filename, _jpegSize);
}