#include <ConvertUtility.h>
#include <header.h>
#include <ConvertUtility.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "jxl.h"
#include "jxl/decode.h"
#include "jxl/decode_cxx.h"
#include "jxl/encode.h"
#include "jxl/encode_cxx.h"
#include "jxl/resizable_parallel_runner.h"
#include "jxl/resizable_parallel_runner_cxx.h"
#include "jxl/thread_parallel_runner.h"
#include "jxl/thread_parallel_runner_cxx.h"
#include "picture_utility.h"

/**
 * Decodes JPEG XL image to floating point pixels and ICC Profile.
 * Pixels are stored as floating point, as interleaved RGBA
 * (4 floating point values per pixel), line per line from top to bottom.
 * Pixel values have nominal range 0..1 but may go beyond this range
 * for HDR or wide gamut.
 */
bool CJxl::DecodeJpegXlOneShot(const uint8_t* jxl, size_t size,
    cv::Mat& matFloat, size_t& xsize, size_t& ysize,
    std::vector<uint8_t>* icc_profile) {
    if (jxl == nullptr || size == 0 || icc_profile == nullptr) return false;

    xsize = 0;
    ysize = 0;
    matFloat.release();
    icc_profile->clear();

    auto runner = JxlResizableParallelRunnerMake(nullptr);
    if (!runner) {
        fprintf(stderr, "JxlResizableParallelRunnerMake failed\n");
        return false;
    }

    auto dec = JxlDecoderMake(nullptr);
    if (!dec) {
        fprintf(stderr, "JxlDecoderMake failed\n");
        return false;
    }

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

    for (;;) {
        const JxlDecoderStatus status = JxlDecoderProcessInput(dec.get());

        if (status == JXL_DEC_ERROR) {
            fprintf(stderr, "Decoder error\n");
            return false;
        }

        if (status == JXL_DEC_NEED_MORE_INPUT) {
            fprintf(stderr, "Error, already provided all input\n");
            return false;
        }

        if (status == JXL_DEC_BASIC_INFO) {
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
            size_t icc_size = 0;

#ifdef __NEW_JXL__
            if (JXL_DEC_SUCCESS !=
                JxlDecoderGetICCProfileSize(dec.get(), JXL_COLOR_PROFILE_TARGET_DATA,
                    &icc_size)) {
                fprintf(stderr, "JxlDecoderGetICCProfileSize failed\n");
                return false;
            }

            if (icc_size > 0) {
                icc_profile->resize(icc_size);

                if (JXL_DEC_SUCCESS != JxlDecoderGetColorAsICCProfile(
                    dec.get(), JXL_COLOR_PROFILE_TARGET_DATA,
                    icc_profile->data(), icc_profile->size())) {
                    fprintf(stderr, "JxlDecoderGetColorAsICCProfile failed\n");
                    return false;
                }
            }
#else
            if (JXL_DEC_SUCCESS !=
                JxlDecoderGetICCProfileSize(
                    dec.get(), &format, JXL_COLOR_PROFILE_TARGET_DATA, &icc_size)) {
                fprintf(stderr, "JxlDecoderGetICCProfileSize failed\n");
                return false;
            }

            if (icc_size > 0) {
                icc_profile->resize(icc_size);

                if (JXL_DEC_SUCCESS != JxlDecoderGetColorAsICCProfile(
                    dec.get(), &format,
                    JXL_COLOR_PROFILE_TARGET_DATA,
                    icc_profile->data(), icc_profile->size())) {
                    fprintf(stderr, "JxlDecoderGetColorAsICCProfile failed\n");
                    return false;
                }
            }
#endif
        }
        else if (status == JXL_DEC_NEED_IMAGE_OUT_BUFFER) {
            size_t buffer_size = 0;

            if (JXL_DEC_SUCCESS !=
                JxlDecoderImageOutBufferSize(dec.get(), &format, &buffer_size)) {
                fprintf(stderr, "JxlDecoderImageOutBufferSize failed\n");
                return false;
            }

            const size_t expected_size = xsize * ysize * 4 * sizeof(float);

            if (buffer_size != expected_size) {
                fprintf(stderr, "Invalid out buffer size %zu %zu\n", buffer_size,
                    expected_size);
                return false;
            }

            matFloat.create(static_cast<int>(ysize), static_cast<int>(xsize),
                CV_32FC4);

            if (matFloat.empty()) {
                fprintf(stderr, "Failed to allocate output image\n");
                return false;
            }

            const size_t pixels_buffer_size = matFloat.total() * matFloat.elemSize();

            if (JXL_DEC_SUCCESS != JxlDecoderSetImageOutBuffer(dec.get(), &format,
                matFloat.data,
                pixels_buffer_size)) {
                fprintf(stderr, "JxlDecoderSetImageOutBuffer failed\n");
                return false;
            }
        }
        else if (status == JXL_DEC_FULL_IMAGE) {
            // Frame successfully decoded.
        }
        else if (status == JXL_DEC_SUCCESS) {
            return !matFloat.empty();
        }
        else {
            fprintf(stderr, "Unknown decoder status\n");
            return false;
        }
    }
}

/**
 * Reads JPEG XL basic information.
 */
void* CJxl::DecodeJpegDim(FILE* file) {
    if (file == nullptr) return nullptr;

    std::vector<uint8_t> data;
    const size_t chunk_size = 64;

    JxlDecoder* dec = JxlDecoderCreate(nullptr);
    if (dec == nullptr) {
        fprintf(stderr, "JxlDecoderCreate failed\n");
        return nullptr;
    }

    JxlDecoderSetKeepOrientation(dec, 1);

    if (JXL_DEC_SUCCESS !=
        JxlDecoderSubscribeEvents(
            dec, JXL_DEC_BASIC_INFO | JXL_DEC_COLOR_ENCODING | JXL_DEC_FRAME)) {
        fprintf(stderr, "JxlDecoderSubscribeEvents failed\n");
        JxlDecoderDestroy(dec);
        return nullptr;
    }

    auto info = new JxlBasicInfo();
    JxlEncoderInitBasicInfo(info);

    bool success = false;
    JxlFrameHeader frame_header = {};

    for (;;) {
        const JxlDecoderStatus status = JxlDecoderProcessInput(dec);

        if (status == JXL_DEC_ERROR) {
            fprintf(stderr, "Decoder error\n");
            break;
        }

        if (status == JXL_DEC_NEED_MORE_INPUT) {
            const size_t remaining = JxlDecoderReleaseInput(dec);

            if (remaining > data.size()) {
                fprintf(stderr, "Invalid remaining input size\n");
                break;
            }

            if (remaining != 0) {
                memmove(data.data(), data.data() + data.size() - remaining, remaining);
            }

            data.resize(remaining + chunk_size);

            const size_t read_size =
                fread(data.data() + remaining, 1, chunk_size, file);

            if (read_size == 0) {
                if (feof(file))
                    fprintf(stderr, "Unexpected EOF\n");
                else
                    fprintf(stderr, "File read error\n");

                break;
            }

            data.resize(remaining + read_size);

            JxlDecoderSetInput(dec, data.data(), data.size());
        }
        else if (status == JXL_DEC_BASIC_INFO) {
            if (JXL_DEC_SUCCESS != JxlDecoderGetBasicInfo(dec, info)) {
                fprintf(stderr, "JxlDecoderGetBasicInfo failed\n");
                break;
            }
        }
        else if (status == JXL_DEC_COLOR_ENCODING) {
            // Basic dimensions are already available.
            // No need to process the color profile here.
        }
        else if (status == JXL_DEC_FRAME) {
            if (JXL_DEC_SUCCESS != JxlDecoderGetFrameHeader(dec, &frame_header)) {
                fprintf(stderr, "JxlDecoderGetFrameHeader failed\n");
                break;
            }
        }
        else if (status == JXL_DEC_SUCCESS) {
            success = true;
            break;
        }
        else {
            fprintf(stderr, "Unexpected decoder status\n");
            break;
        }
    }

    JxlDecoderDestroy(dec);

    if (!success) {
        delete info;
        return nullptr;
    }

    return info;
}

/**
 * Compresses the provided pixels.
 *
 * Input format: CV_32FC4 (RGBA).
 */
bool CJxl::EncodeJxlOneshot(const cv::Mat& matFloat,
    std::vector<uint8_t>* compressed) {
    if (compressed == nullptr) return false;

    compressed->clear();

    if (matFloat.empty()) return false;

    if (matFloat.type() != CV_32FC4) {
        fprintf(stderr, "Invalid JPEG XL input format: expected CV_32FC4\n");
        return false;
    }

    auto enc = JxlEncoderMake(nullptr);
    if (!enc) {
        fprintf(stderr, "JxlEncoderMake failed\n");
        return false;
    }

    auto runner = JxlThreadParallelRunnerMake(
        nullptr, JxlThreadParallelRunnerDefaultNumWorkerThreads());

    if (!runner) {
        fprintf(stderr, "JxlThreadParallelRunnerMake failed\n");
        return false;
    }

    if (JXL_ENC_SUCCESS != JxlEncoderSetParallelRunner(enc.get(),
        JxlThreadParallelRunner,
        runner.get())) {
        fprintf(stderr, "JxlEncoderSetParallelRunner failed\n");
        return false;
    }

    JxlPixelFormat pixel_format = { 4, JXL_TYPE_FLOAT, JXL_NATIVE_ENDIAN, 0 };

    JxlBasicInfo basic_info;
    JxlEncoderInitBasicInfo(&basic_info);

    basic_info.xsize = static_cast<uint32_t>(matFloat.cols);

    basic_info.ysize = static_cast<uint32_t>(matFloat.rows);

    basic_info.bits_per_sample = 32;
    basic_info.exponent_bits_per_sample = 8;
    basic_info.uses_original_profile = JXL_FALSE;

    if (JXL_ENC_SUCCESS != JxlEncoderSetBasicInfo(enc.get(), &basic_info)) {
        fprintf(stderr, "JxlEncoderSetBasicInfo failed\n");
        return false;
    }

    JxlColorEncoding color_encoding = {};
    JxlColorEncodingSetToSRGB(&color_encoding, JXL_FALSE);

    if (JXL_ENC_SUCCESS !=
        JxlEncoderSetColorEncoding(enc.get(), &color_encoding)) {
        fprintf(stderr, "JxlEncoderSetColorEncoding failed\n");
        return false;
    }

    const size_t data_size = matFloat.total() * matFloat.elemSize();

    JxlEncoderFrameSettings* frame_settings =
        JxlEncoderFrameSettingsCreate(enc.get(), nullptr);

    if (frame_settings == nullptr) {
        fprintf(stderr, "JxlEncoderFrameSettingsCreate failed\n");
        return false;
    }

    if (JXL_ENC_SUCCESS != JxlEncoderAddImageFrame(frame_settings, &pixel_format,
        matFloat.data, data_size)) {
        fprintf(stderr, "JxlEncoderAddImageFrame failed\n");
        return false;
    }

    compressed->resize(64);

    uint8_t* next_out = compressed->data();
    size_t avail_out = compressed->size();

    JxlEncoderStatus process_result = JXL_ENC_NEED_MORE_OUTPUT;

    while (process_result == JXL_ENC_NEED_MORE_OUTPUT) {
        process_result = JxlEncoderProcessOutput(enc.get(), &next_out, &avail_out);

        if (process_result == JXL_ENC_NEED_MORE_OUTPUT) {
            const size_t offset = static_cast<size_t>(next_out - compressed->data());

            const size_t new_size = compressed->empty() ? 64 : compressed->size() * 2;

            compressed->resize(new_size);

            next_out = compressed->data() + offset;

            avail_out = compressed->size() - offset;
        }
    }

    if (process_result != JXL_ENC_SUCCESS) {
        fprintf(stderr, "JxlEncoderProcessOutput failed\n");

        compressed->clear();
        return false;
    }

    compressed->resize(static_cast<size_t>(next_out - compressed->data()));

    return !compressed->empty();
}

void CJxl::WriteFile(const cv::Mat& matFloat, const wxString& path) {
    if (matFloat.empty()) return;

    std::vector<uint8_t> compressed;

    if (!EncodeJxlOneshot(matFloat, &compressed)) {
        return;
    }

    if (compressed.empty()) return;

    CPictureUtility::writefile(path, compressed.data(), compressed.size());
}

void CJxl::GetDimensions(const wxString& jxl_filename, int& width,
    int& height) {
    width = 0;
    height = 0;

    const std::string filename =
        CConvertUtility::ConvertToStdString(jxl_filename);

    FILE* file = fopen(filename.c_str(), "rb");

    if (file == nullptr) {
        fprintf(stderr, "Failed to read file %s\n", filename.c_str());
        return;
    }

    auto info = static_cast<JxlBasicInfo*>(DecodeJpegDim(file));

    if (info == nullptr) {
        fprintf(stderr, "Couldn't read basic info\n");

        fclose(file);
        return;
    }

    width = static_cast<int>(info->xsize);

    height = static_cast<int>(info->ysize);

    delete info;
    fclose(file);
}

cv::Mat CJxl::GetPicture(const wxString& path) {
    cv::Mat matFloat;
    std::vector<uint8_t> icc_profile;

    size_t xsize = 0;
    size_t ysize = 0;

    std::vector<uint8_t> compressedImage = CPictureUtility::ReadFile(path);

    if (compressedImage.empty()) return cv::Mat();

    if (!DecodeJpegXlOneShot(compressedImage.data(), compressedImage.size(),
        matFloat, xsize, ysize, &icc_profile)) {
        return cv::Mat();
    }

    return matFloat;
}
