#include "header.h"
#include "wic.h"
#include "RegardsBitmap.h"
#include <windows.h>

using namespace Regards::Picture;


template <typename T>
inline void SafeRelease(T*& p)
{
    if (NULL != p)
    {
        p->Release();
        p = NULL;
    }
}

CWic::CWic()
{
    // Create WIC factory
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&m_pIWICFactory)
    );
}

CWic::~CWic()
{
    SafeRelease(m_pIWICFactory);
}

CRegardsBitmap * CWic::GetThumbnailMetadata(const string& filename)
{
    HRESULT hr = S_OK;
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapSource* ppIThumbnail = nullptr;
    UINT cx = 0, cy = 0;
    CRegardsBitmap* bitmap = nullptr;

    hr = m_pIWICFactory->CreateDecoderFromFilename(
        std::wstring(filename.begin(), filename.end()).c_str(),                      // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );

    if (SUCCEEDED(hr)) {
        // Create a MetadataQueryReader from the decoder
        hr = pDecoder->GetThumbnail(&ppIThumbnail);
    }

    if (ppIThumbnail != nullptr)
    {
        if (SUCCEEDED(ppIThumbnail->GetSize(&cx, &cy)))
        {
            const UINT stride = cx * sizeof(DWORD);
            const UINT buf_size = cy * stride;
            byte* buf = new byte[buf_size];

            hr = ppIThumbnail->CopyPixels(
                nullptr,
                stride,
                buf_size,
                buf
            );

            if (SUCCEEDED(hr))
            {
                cv::Mat mat(cy, cx, CV_8UC4, buf);
                bitmap = new CRegardsBitmap();
                bitmap->SetMatrix(mat);
                bitmap->VertFlipBuf();
            }
        }
    }


    SafeRelease(pDecoder);
    SafeRelease(ppIThumbnail);
    return bitmap;
}

void CWic::GetPictureDimension(const string& filename, int& width, int& height)
{
    HRESULT hr = S_OK;
    IWICFormatConverter* m_pConvertedSourceBitmap = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    UINT cx = 0, cy = 0;
    CRegardsBitmap* bitmap = nullptr;

    hr = m_pIWICFactory->CreateDecoderFromFilename(
        std::wstring(filename.begin(), filename.end()).c_str(),                      // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );

    // Retrieve the first frame of the image from the decoder
    IWICBitmapFrameDecode* pFrame = NULL;

    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }

    //Step 3: Format convert the frame to 32bppPBGRA
    if (SUCCEEDED(hr))
    {

        hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pConvertedSourceBitmap->Initialize(
            pFrame,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            NULL,                            // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }

    if (SUCCEEDED(hr))
    {
        m_pConvertedSourceBitmap->GetSize(&cx, &cy);
        width = cx;
        height = cy;
    }


    SafeRelease(pDecoder);
    SafeRelease(pFrame);
    SafeRelease(m_pConvertedSourceBitmap);
}

int CWic::GetNbFrame(const string& filename, bool& error)
{
    HRESULT hr = S_OK;
    IWICBitmapDecoder* pDecoder = nullptr;
    UINT count = 0;
    error = false;

    hr = m_pIWICFactory->CreateDecoderFromFilename(
        std::wstring(filename.begin(), filename.end()).c_str(),                      // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );

    // Retrieve the first frame of the image from the decoder
    IWICBitmapFrameDecode* pFrame = NULL;

    if (SUCCEEDED(hr))
        hr = pDecoder->GetFrameCount(&count);
    else
        error = true;

    SafeRelease(pDecoder);
    return count;
}

CRegardsBitmap* CWic::GetPicture(const string& filename, const int& numPicture) {
    HRESULT hr = S_OK;
    IWICFormatConverter* m_pConvertedSourceBitmap = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    UINT cx = 0, cy = 0, count = 0;
    CRegardsBitmap* bitmap = nullptr;

    hr = m_pIWICFactory->CreateDecoderFromFilename(
        std::wstring(filename.begin(), filename.end()).c_str(),                      // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );

    // Retrieve the first frame of the image from the decoder
    IWICBitmapFrameDecode* pFrame = NULL;

    if (SUCCEEDED(hr))
        hr = pDecoder->GetFrameCount(&count);

    if (SUCCEEDED(hr))
    {
        if(numPicture < count)
            hr = pDecoder->GetFrame(numPicture, &pFrame);
    }

    //Step 3: Format convert the frame to 32bppPBGRA
    if (SUCCEEDED(hr))
    {
       
        hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pConvertedSourceBitmap->Initialize(
            pFrame,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            NULL,                            // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }

    if (SUCCEEDED(m_pConvertedSourceBitmap->GetSize(&cx, &cy)))
    {
        const UINT stride = cx * sizeof(DWORD);
        const UINT buf_size = cy * stride;
        byte * buf = new byte[buf_size];

        hr = m_pConvertedSourceBitmap->CopyPixels(
            nullptr,
            stride,
            buf_size,
            buf
        );

        if (SUCCEEDED(hr))
        {
            cv::Mat mat(cy, cx, CV_8UC4, buf);
            bitmap = new CRegardsBitmap();
            bitmap->SetMatrix(mat);
            bitmap->VertFlipBuf();
        }

    }

    SafeRelease(pDecoder);
    SafeRelease(pFrame);
    SafeRelease(m_pConvertedSourceBitmap);

    return bitmap;
}
