///////////////////////////////////////////////////////////////////////////////
// Name:        convertmattowxbmp.cpp
// Purpose:     Converts OpenCV bitmap (Mat) stored as BGR CVU8 to wxBitmap
// Author:      PB
// Created:     2020-09-16
// Copyright:   (c) 2020 PB
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#include "header.h"
#include <wx/wx.h>
#include <wx/rawbmp.h>

#include <opencv2/core/mat.hpp>

#include "convertmattowxbmp.h"

#ifdef __WXMSW__

namespace
{

// Version optimized for Microsoft Windows.
// matBitmap must be continous and matBitmap.cols % 4 must equal 0
// as SetDIBits() requires the DIB rows to be DWORD-aligned.
// Should not be called directly but only from ConvertMatBitmapTowxBitmap()
// which does all the necessary debug checks.
bool ConvertMatBitmapTowxBitmapMSW(const cv::Mat& matBitmap, wxBitmap& bitmap)
{
    const HDC  hScreenDC = ::GetDC(nullptr);
    BITMAPINFO bitmapInfo{0};
    bool       success;

    bitmapInfo.bmiHeader.biSize        = sizeof(BITMAPINFO) - sizeof(RGBQUAD);
    bitmapInfo.bmiHeader.biWidth       = bitmap.GetWidth();
    bitmapInfo.bmiHeader.biHeight      = 0 - bitmap.GetHeight();
    bitmapInfo.bmiHeader.biPlanes      = 1;
    bitmapInfo.bmiHeader.biBitCount    = 24;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    success = ::SetDIBits(hScreenDC, bitmap.GetHBITMAP(), 0, bitmap.GetHeight(),
                          matBitmap.data, &bitmapInfo, DIB_RGB_COLORS) != 0;
    ::ReleaseDC(nullptr, hScreenDC);

    return success;
}

} // unnamed namespace

#endif // #ifndef __WXMSW__

// See the function description in the header file.
bool ConvertMatBitmapTowxBitmap(const cv::Mat& matBitmap, wxBitmap& bitmap)
{
    wxCHECK(!matBitmap.empty(), false);
    wxCHECK(matBitmap.type() == CV_8UC3, false);
    wxCHECK(matBitmap.dims == 2, false);
    wxCHECK(bitmap.IsOk(), false);
    wxCHECK(bitmap.GetWidth() == matBitmap.cols && bitmap.GetHeight() == matBitmap.rows, false);
    wxCHECK(bitmap.GetDepth() == 24, false);

#ifdef __WXMSW__
    if (  bitmap.IsDIB()
          && matBitmap.isContinuous()
          && matBitmap.cols % 4 == 0 )
    {
        return ConvertMatBitmapTowxBitmapMSW(matBitmap, bitmap);
    }
#endif

    wxNativePixelData           pixelData(bitmap);
    wxNativePixelData::Iterator pixelDataIt(pixelData);

    if ( matBitmap.isContinuous() )
    {
        const uchar* bgr = matBitmap.data;

        for ( int row = 0; row < pixelData.GetHeight(); ++row )
        {
            pixelDataIt.MoveTo(pixelData, 0, row);

            for ( int col = 0;
                  col < pixelData.GetWidth();
                  ++col, ++pixelDataIt )
            {
                pixelDataIt.Blue()  = *bgr++;
                pixelDataIt.Green() = *bgr++;
                pixelDataIt.Red()   = *bgr++;
            }
        }
    }
    else // Is it even possible for Mat with image to be not continous?
    {
        auto matBitmapIt = matBitmap.begin<cv::Vec3b>();

        for ( int row = 0; row < pixelData.GetHeight(); ++row )
        {
            pixelDataIt.MoveTo(pixelData, 0, row);

            for ( int col = 0;
                  col < pixelData.GetWidth();
                  ++col, ++pixelDataIt, ++matBitmapIt )
            {
                pixelDataIt.Blue()  = (*matBitmapIt)[0];
                pixelDataIt.Green() = (*matBitmapIt)[1];
                pixelDataIt.Red()   = (*matBitmapIt)[2];
            }
        }
    }

    return bitmap.IsOk();
}