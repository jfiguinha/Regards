#pragma once
#include <FiltreEffectInterface.h>
#include <TreeElementValue.h>
#include <EffectParameter.h>

class CRegardsBitmap;
class CImageLoadingFormat;
class CFiltreEffet;

namespace Regards
{
	namespace FiltreEffet
	{
		class CDraw;
	}
}

namespace Regards
{
	namespace Control
	{
		class CBitmapWndViewer;
	}
}

using namespace Regards::Control;
using namespace Regards::FiltreEffet;

class IMouseUpdate
{
public:
	virtual CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CDraw * dessing) = 0;
	virtual void ApplyPreviewEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing) = 0;
};
