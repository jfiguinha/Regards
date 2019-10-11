#pragma once
#include <FiltreEffectInterface.h>
#include <TreeElementValue.h>
#include <EffectParameter.h>

class CRegardsBitmap;
class CImageLoadingFormat;
class CFiltreEffet;
class IBitmapDisplay;

namespace Regards
{
	namespace FiltreEffet
	{
		class CDraw;
	}
}

using namespace Regards::FiltreEffet;

class IMouseUpdate
{
public:
	virtual CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessing) = 0;
	virtual void ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int & widthOutput, int & heightOutput) = 0;
};
