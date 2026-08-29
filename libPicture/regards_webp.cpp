#include <header.h>
#include <webp/decode.h>
#include <webp/demux.h>
#include <webp/encode.h>

#include <cstdio>
#include <cstring>
#include <vector>

#include "picture_utility.h"
#include "regards_webp.h"

class CRegardsWebpImpl {
public:
    static bool IsWebP(const WebPData* webp_data);
};

cv::Mat CRegardsWebp::GetPicture(const wxString& filename) {
    std::vector<uint8_t> compressedImage = CPictureUtility::ReadFile(filename);

    if (compressedImage.empty()) return cv::Mat();

    int width = 0;
    int height = 0;

    uint8_t* data = WebPDecodeBGRA(compressedImage.data(), compressedImage.size(),
        &width, &height);

    if (data == nullptr || width <= 0 || height <= 0) return cv::Mat();

    // WebPDecodeBGRA retourne un buffer alloué par libwebp.
    // clone() est nécessaire pour que cv::Mat possède sa propre mémoire.
    cv::Mat mat(height, width, CV_8UC4, data);

    cv::Mat result = mat.clone();

    WebPFree(data);

    return result;
}

void CRegardsWebp::ApplyTransform(cv::Mat& image) {
    if (image.empty()) return;

    if (image.channels() == 3) {
        cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);
    }
    else if (image.channels() == 1) {
        cv::cvtColor(image, image, cv::COLOR_GRAY2BGRA);
    }
    else if (image.channels() != 4) {
        return;
    }

    // WebPAnimDecoder retourne les pixels en RGBA.
    // Conversion vers le format BGRA utilisé par l'application.
    cv::cvtColor(image, image, cv::COLOR_RGBA2BGRA);
}

std::vector<cv::Mat> CRegardsWebp::GetAllPicture(const wxString& filename,
    int& delay) {
    std::vector<cv::Mat> pictureList;

    delay = 0;

    std::vector<uint8_t> compressedImage = CPictureUtility::ReadFile(filename);

    if (compressedImage.empty()) return pictureList;

    WebPData webp_data;
    WebPDataInit(&webp_data);

    webp_data.bytes = compressedImage.data();
    webp_data.size = compressedImage.size();

    if (!CRegardsWebpImpl::IsWebP(&webp_data)) return pictureList;

    WebPAnimDecoderOptions options;
    WebPAnimDecoderOptionsInit(&options);

    WebPAnimDecoder* dec = WebPAnimDecoderNew(&webp_data, &options);

    if (dec == nullptr) return pictureList;

    WebPAnimInfo anim_info;

    if (!WebPAnimDecoderGetInfo(dec, &anim_info)) {
        fprintf(stderr, "Error getting global info about the animation\n");

        WebPAnimDecoderDelete(dec);
        return pictureList;
    }

    int prev_frame_timestamp = 0;

    while (WebPAnimDecoderHasMoreFrames(dec)) {
        uint8_t* frame_rgba = nullptr;
        int timestamp = 0;

        if (!WebPAnimDecoderGetNext(dec, &frame_rgba, &timestamp)) {
            fprintf(stderr, "Error decoding WebP animation frame\n");

            break;
        }

        if (frame_rgba == nullptr) break;

        // Le timestamp est cumulatif.
        // On conserve le comportement original : delay contient
        // la durée de la dernière image décodée.
        delay = timestamp - prev_frame_timestamp;
        prev_frame_timestamp = timestamp;

        cv::Mat frame(anim_info.canvas_height, anim_info.canvas_width, CV_8UC4);

        const size_t frameSize = static_cast<size_t>(anim_info.canvas_width) *
            static_cast<size_t>(anim_info.canvas_height) * 4;

        std::memcpy(frame.data, frame_rgba, frameSize);

        // frame_rgba est RGBA.
        // Conversion directe vers BGRA.
        cv::cvtColor(frame, frame, cv::COLOR_RGBA2BGRA);

        pictureList.push_back(std::move(frame));
    }

    WebPAnimDecoderDelete(dec);

    return pictureList;
}

int CRegardsWebp::GetNbFrame(const wxString& filename) {
    std::vector<uint8_t> compressedImage = CPictureUtility::ReadFile(filename);

    if (compressedImage.empty()) return 0;

    WebPData webp_data;
    WebPDataInit(&webp_data);

    webp_data.bytes = compressedImage.data();
    webp_data.size = compressedImage.size();

    if (!CRegardsWebpImpl::IsWebP(&webp_data)) return 0;

    WebPAnimDecoder* dec = WebPAnimDecoderNew(&webp_data, nullptr);

    if (dec == nullptr) return 0;

    WebPAnimInfo anim_info;

    const bool result = WebPAnimDecoderGetInfo(dec, &anim_info) != 0;

    WebPAnimDecoderDelete(dec);

    if (!result) {
        fprintf(stderr, "Error getting global info about the animation\n");

        return 0;
    }

    return anim_info.frame_count;
}

// Returns true if this is a valid WebP bitstream.
bool CRegardsWebpImpl::IsWebP(const WebPData* webp_data) {
    if (webp_data == nullptr || webp_data->bytes == nullptr ||
        webp_data->size == 0) {
        return false;
    }

    return WebPGetInfo(webp_data->bytes, webp_data->size, nullptr, nullptr) != 0;
}

void CRegardsWebp::SavePicture(const wxString& fileName, cv::Mat& source,
    const int& option) {
    if (source.empty()) return;

    cv::Mat image;

    if (source.channels() == 4) {
        image = source;
    }
    else if (source.channels() == 3) {
        cv::cvtColor(source, image, cv::COLOR_BGR2BGRA);
    }
    else if (source.channels() == 1) {
        cv::cvtColor(source, image, cv::COLOR_GRAY2BGRA);
    }
    else {
        return;
    }

    if (!image.isContinuous()) image = image.clone();

    uint8_t* output = nullptr;

    const size_t size = WebPEncodeBGRA(image.data, image.cols, image.rows,
        static_cast<int>(image.step),
        static_cast<float>(option), &output);

    if (size == 0 || output == nullptr) {
        fprintf(stderr, "WebPEncodeBGRA failed\n");

        return;
    }

    CPictureUtility::writefile(fileName, output, size);

    WebPFree(output);
}

void CRegardsWebp::GetPictureDimension(const wxString& filename, int& width,
    int& height) {
    width = 0;
    height = 0;

    std::vector<uint8_t> compressedImage = CPictureUtility::ReadFile(filename);

    if (compressedImage.empty()) return;

    if (!WebPGetInfo(compressedImage.data(), compressedImage.size(), &width,
        &height)) {
        width = 0;
        height = 0;
    }
}