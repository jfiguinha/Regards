#pragma once
namespace Regards
{
	namespace FiltreEffet
	{
		class CDraw;
	}
}

class CFiltreEffet;
class CImageLoadingFormat;
class CRgbaquad;

using namespace Regards::FiltreEffet;

class IBitmapDisplay
{
public:
    virtual ~IBitmapDisplay(){};
	virtual int GetHPos() = 0;
	virtual int GetVPos() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual float CalculPictureRatio(const int& pictureWidth, const int& pictureHeight) = 0;
	virtual CDraw * GetDessinPt() = 0;
	virtual float GetRatio() = 0;
	virtual wxString GetFilename() = 0;
	virtual void UpdateBitmap(CImageLoadingFormat* bitmap, const bool& updateAll) = 0;
	virtual CRgbaquad GetBackColor() = 0;
	virtual CFiltreEffet * GetFiltreEffet() = 0;
	virtual int GetOrientation() = 0;
	virtual CImageLoadingFormat * GetBitmap(const bool& source) = 0;
	virtual void StartTransitionEffect(CImageLoadingFormat* bmpSecond, const bool& setPicture) = 0;
	virtual void StopTransitionEffect(CImageLoadingFormat* bmpSecond) = 0;
	virtual void CalculCenterPositionPicture() = 0;
};
