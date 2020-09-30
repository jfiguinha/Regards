#pragma once

class CRegardsBitmap;
class CImageLoadingFormat;
class CEffectParameter;
class IBitmapDisplay;

namespace Regards
{
	namespace OpenGL
	{
		class GLTexture;
	}
}

using namespace Regards::OpenGL;

class IAfterEffect
{
public:
    virtual ~IAfterEffect(){};

	virtual int GetTypeFilter() = 0;
	virtual CRegardsBitmap * GenerateBitmapEffect(CImageLoadingFormat * nextPicture, int etape, IBitmapDisplay * bmpViewer, wxRect &rcOut) = 0;
#ifdef RENDEROPENGL
	virtual void GenerateBitmapOpenCLEffect(Regards::OpenGL::GLTexture * glPicture, CImageLoadingFormat * nextPicture, int etape, IBitmapDisplay * bmpViewer, wxRect &rcOut) = 0;
	virtual void GenerateTexture(Regards::OpenGL::GLTexture * glPicture, CImageLoadingFormat * nextPicture) = 0;
#endif
};