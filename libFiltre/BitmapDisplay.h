#pragma once
#include <RegardsBitmap.h>
namespace Regards
{
	namespace FiltreEffet
	{
		class CDraw;
	}
}

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLContext;
	}
}

class CFiltreEffet;
class CRegardsFloatBitmap;
class CImageLoadingFormat;

using namespace Regards::FiltreEffet;
using namespace Regards::OpenCL;

class IBitmapDisplay
{
public:
    virtual ~IBitmapDisplay(){};
	virtual COpenCLContext * GetOpenCLContext() = 0;
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
	virtual CRegardsBitmap* GetBitmap(const bool& source) = 0;
	virtual CRegardsFloatBitmap* GetFloatBitmap(const bool& source) = 0;
};
