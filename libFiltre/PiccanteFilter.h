#pragma once

class CRegardsFloatBitmap;
class CImageLoadingFormat;

class CPiccanteFilter
{
public:
    static void BestExposure(CRegardsFloatBitmap * & floatBitmap, const float &tmoValue = 2.2f);
    static void FilterBilateral2DS(CRegardsFloatBitmap * & floatBitmap, const float & sigma_s = 4.0f, const float & sigma_r = 0.05f);
    static void FilterKuwahara(CRegardsFloatBitmap * & floatBitmap, const int &kernelSize = 11);
    static void LoadPicture(const wxString & fileName, const bool &isThumbnail, CImageLoadingFormat * bitmap);
    static void GetPictureDimensions(const wxString & fileName, int & width, int & height);
    static float clamp ( float val, float minval, float maxval );
};