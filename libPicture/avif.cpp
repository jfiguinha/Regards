#include <header.h>
#include "avif.h"
#include "avif/avif.h"
using namespace Regards::Picture;



CAvif::CAvif()
{
    
}

CAvif::~CAvif()
{
}




cv::Mat GetPictureLocal(const char * filename, avifDecoder * decoder)
{
    cv::Mat out;
    if(decoder == nullptr)
        return out;

    avifImage* decoded = avifImageCreateEmpty();
    avifResult result;
    result = avifDecoderSetIOFile(decoder, filename);
        
    if (result != AVIF_RESULT_OK) {
        fprintf(stderr, "Cannot open file for read: %s\n", filename);
        goto cleanup;
    }

    result = avifDecoderParse(decoder);

    if (result != AVIF_RESULT_OK) {
        fprintf(stderr, "Failed to decode image: %s\n", avifResultToString(result));
        goto cleanup;
    }

    if (decoder != nullptr)
    {
        avifResult decodeResult;
        decodeResult = avifDecoderRead(decoder, decoded);

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

                out.create(image_height, image_width, CV_8UC4);
                memcpy(out.data, dstRGB.pixels, image_height * image_width * 4);
                //cv::flip(out, out, 0);
            }
            avifRGBImageFreePixels(&dstRGB);
        }
        
    }
    
cleanup:
    avifImageDestroy(decoded);
        
	return out;
}


cv::Mat GetPictureThumbnail(const char* filename, const int &width, const int &height, avifDecoder* decoder)
{
    cv::Mat out;
    if (decoder == nullptr)
        return out;

    avifImage* decoded = avifImageCreateEmpty();
    avifResult result;
    result = avifDecoderSetIOFile(decoder, filename);

    if (result != AVIF_RESULT_OK) {
        fprintf(stderr, "Cannot open file for read: %s\n", filename);
        goto cleanup;
    }

    result = avifDecoderParse(decoder);

    if (result != AVIF_RESULT_OK) {
        fprintf(stderr, "Failed to decode image: %s\n", avifResultToString(result));
        goto cleanup;
    }

    if (decoder != nullptr)
    {
        avifResult decodeResult;
        decodeResult = avifDecoderRead(decoder, decoded);

        if (decodeResult == AVIF_RESULT_OK)
        {
            avifDiagnostics diag;


            avifBool result = avifImageScale(decoded, width, height, decoder->imageSizeLimit,
                decoder->imageDimensionLimit,
                &decoder->diag);

            avifRGBImage dstRGB;
            avifRGBImageSetDefaults(&dstRGB, decoded);
            dstRGB.format = AVIF_RGB_FORMAT_BGRA; // See choices in avif.h
            dstRGB.depth = 8; // [8, 10, 12, 16]; Does not need to match image->depth.
            avifRGBImageAllocatePixels(&dstRGB);

            if (result == AVIF_TRUE)
            {
                if (avifImageYUVToRGB(decoded, &dstRGB) == AVIF_RESULT_OK)
                {
                    int image_width = dstRGB.width;
                    int image_height = dstRGB.height;

                    out.create(image_height, image_width, CV_8UC4);
                    memcpy(out.data, dstRGB.pixels, image_height * image_width * 4);
                    //cv::flip(out, out, 0);
                }
            }
            avifRGBImageFreePixels(&dstRGB);
        }

    }

cleanup:
    avifImageDestroy(decoded);

    return out;
}


bool CAvif::IsOccupied()
{
    return false;
}

cv::Mat CAvif::GetPicture(const char * filename)
{
    cv::Mat out;

    avifDecoder* decoder = avifDecoderCreate(); 
#if defined(__APPLE_) || defined(__ARM64__) 
    decoder->codecChoice = AVIF_CODEC_CHOICE_AUTO;
#else
    decoder->codecChoice = AVIF_CODEC_CHOICE_DAV1D;
#endif  
    out = GetPictureLocal(filename, decoder);
    avifDecoderDestroy(decoder);

        
	return out;
}



cv::Mat CAvif::GetPictureThumb(const char* filename, const int &width, const int &heigth)
{
    /*
    cv::Mat out;

    muPicture.lock();
    isOccupied = true;
    if (decoderThumb == nullptr)
    {
        decoderThumb = avifDecoderCreate();
#if defined(__APPLE_) || defined(__ARM64__) 
        decoderThumb->codecChoice = AVIF_CODEC_CHOICE_AUTO;
#else
        decoderThumb->codecChoice = AVIF_CODEC_CHOICE_DAV1D;
#endif
    }

    out = GetPictureThumbnail(filename, width, heigth, decoderThumb);
    isOccupied = false;
    muPicture.unlock();

    return out;
    */

    cv::Mat out;

    avifDecoder* decoder = avifDecoderCreate();
#if defined(__APPLE_) || defined(__ARM64__) 
    decoder->codecChoice = AVIF_CODEC_CHOICE_AUTO;
#else
    decoder->codecChoice = AVIF_CODEC_CHOICE_DAV1D;
#endif  
    out = GetPictureThumbnail(filename, width, heigth, decoder);
    avifDecoderDestroy(decoder);


    return out;
}


