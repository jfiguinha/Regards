#include <header.h>
#include "DecodeRawPicture.h"
#include <libraw/libraw.h>
#include <DecodeRawParameter.h>
#include <ImageLoadingFormat.h>
#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }
#undef __FREEIMAGE__
#include <ximage.h>
using namespace Regards::Filter;

class CDecodeRawPicturePimpl
{
public:


    CDecodeRawPicturePimpl(const std::string& fileName)
        : rawProcessor(std::make_unique<LibRaw>())
    {
        result = rawProcessor->open_file(fileName.c_str());

        if (result != LIBRAW_SUCCESS)
            return;

        result = rawProcessor->unpack();

        if (result != LIBRAW_SUCCESS)
            return;

        // Keep the original LibRaw parameters.
        originalParams = rawProcessor->imgdata.params;
    }


    ~CDecodeRawPicturePimpl()
    {
        if (rawProcessor)
            rawProcessor->recycle();
    }


    CImageLoadingFormat* DecodePicture(
        CDecodeRawParameter* decodeRawParameter)
    {
        if (rawProcessor == nullptr || result != LIBRAW_SUCCESS)
            return nullptr;

        // Always start from the original LibRaw parameters.
        rawProcessor->imgdata.params = originalParams;

        if (decodeRawParameter != nullptr)
        {
            auto& params = rawProcessor->imgdata.params;
            const auto& source = *decodeRawParameter;

            params.bright = source.bright;
            params.highlight = source.highlight;
            params.threshold = source.threshold;
            params.use_auto_wb = source.use_auto_wb;
            params.use_camera_wb = source.use_camera_wb;
            params.half_size = source.half_size;
            params.use_camera_matrix = source.use_camera_matrix;

            if (source.aberRedEnable)
                params.aber[0] = source.aberRed;

            if (source.aberGreenEnable)
                params.aber[2] = source.aberGreen;

            params.user_mul[0] = source.multiRed;
            params.user_mul[1] = source.multiGreen;
            params.user_mul[2] = source.multiBlue;
            params.user_mul[3] = source.multiOther;

            params.user_flip = source.flip;
            params.user_qual = source.interpolation;

            params.user_black = source.black;

            params.user_cblack[0] = source.blackchannelRed;
            params.user_cblack[1] = source.blackchannelGreen;
            params.user_cblack[2] = source.blackchannelBlue;
            params.user_cblack[3] = source.blackchannelOther;

            params.user_sat = source.saturation;
            params.med_passes = source.medPasses;
            params.no_auto_bright = source.noautobright;
            params.auto_bright_thr = source.autobright;
            params.adjust_maximum_thr = source.adjust_maximum_thr;
            params.use_fuji_rotate = source.use_fuji_rotate;
            params.green_matching = source.green_matching;

            params.dcb_iterations = source.dcb_iterations;
            params.dcb_enhance_fl = source.dcb_enhance_fl;
            params.fbdd_noiserd = source.fbdd_noiserd;

            params.exp_correc = source.exp_correc;
            params.exp_shift = source.exp_shift;
            params.exp_preser = source.exp_preser;
        }

        try
        {
            result = rawProcessor->dcraw_process();
        }
        catch (...)
        {
            return nullptr;
        }

        if (result != LIBRAW_SUCCESS)
            return nullptr;

        int width = 0;
        int height = 0;
        int rawColor = 0;
        int rawBitsize = 0;

        rawProcessor->get_mem_image_format(
            &width,
            &height,
            &rawColor,
            &rawBitsize);

        if (width <= 0 || height <= 0 || rawColor <= 0 || rawBitsize <= 0)
            return nullptr;

        const int bytesPerPixel = rawColor * (rawBitsize / 8);

        if (bytesPerPixel <= 0)
            return nullptr;

        const int stride =
            ((bytesPerPixel * width + bytesPerPixel) & ~bytesPerPixel);


        CxImage image;

        if (!image.Create(width, height, rawBitsize * rawColor))
        {
            return nullptr;
        }

        result = rawProcessor->copy_mem_image(
            image.GetBits(),
            stride,
            1);

        if (result != LIBRAW_SUCCESS)
        {
            return nullptr;
        }

        auto* imageLoadingFormat = new CImageLoadingFormat();
        imageLoadingFormat->SetPicture(image);
        imageLoadingFormat->Flip();

        return imageLoadingFormat;
    }



    std::unique_ptr<LibRaw> rawProcessor;
    int result = LIBRAW_SUCCESS;
    libraw_output_params_t originalParams{};
};

CDecodeRawPicture::CDecodeRawPicture(const std::string& fileName)
    : pimpl(std::make_unique<CDecodeRawPicturePimpl>(fileName))
{}

CDecodeRawPicture::~CDecodeRawPicture() = default;

CImageLoadingFormat* CDecodeRawPicture::DecodePicture(
    CDecodeRawParameter* decodeRawParameter)
{
    return pimpl->DecodePicture(decodeRawParameter);
}