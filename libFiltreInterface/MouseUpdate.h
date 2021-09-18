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

namespace Regards
{
	namespace OpenGL
	{
		class CRenderOpenGL;
	}
}

using namespace Regards::FiltreEffet;
using namespace Regards::OpenGL;

class IMouseUpdate
{
public:
	virtual CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessing) = 0;
	virtual void ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int & widthOutput, int & heightOutput) = 0;
	virtual void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing) = 0;
	virtual void ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID) = 0;
	virtual void DisableOpenGLShader() = 0;
	virtual bool IsOpenGLCompatible() = 0;
	virtual bool IsSourcePreview() = 0;
};
