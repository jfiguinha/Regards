#pragma once
#include <FiltreEffectInterface.h>
#include <TreeElementValue.h>
#include <EffectParameter.h>

class CRegardsBitmap;
class CImageLoadingFormat;

namespace Regards
{
	namespace Control
	{
		class CBitmapWndViewer;
	}
}

using namespace Regards::Control;

class IMouseUpdate
{
public:
	virtual CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer) = 0;
};
