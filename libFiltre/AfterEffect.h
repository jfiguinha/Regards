#pragma once

class CRegardsBitmap;
class CImageLoadingFormat;
class CEffectParameter;

namespace Regards
{
	namespace Control
	{
		class CBitmapWndViewer;
	}
}

namespace Regards
{
	namespace OpenGL
	{
		class GLTexture;
	}
}

using namespace Regards::OpenGL;
using namespace Regards::Control;

class IAfterEffect
{
public:
	virtual int GetTypeFilter() = 0;
	virtual CRegardsBitmap * GenerateBitmapEffect(CImageLoadingFormat * nextPicture, CEffectParameter * effectParameter, CBitmapWndViewer * bmpViewer, wxRect &rcOut) = 0;
	virtual void GenerateBitmapOpenCLEffect(GLTexture * glPicture, CImageLoadingFormat * nextPicture, CEffectParameter * effectParameter, CBitmapWndViewer * bmpViewer, wxRect &rcOut) = 0;
};