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
	virtual void GenerateBitmapEffect(CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut) = 0;

#ifdef RENDEROPENGL
	virtual void GenerateBitmapOpenCLEffect(CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut) = 0;
	virtual void GenerateTexture(CImageLoadingFormat * nextPicture) = 0;
	virtual GLTexture * GetTexture(const int &numTexture) = 0;
	virtual void DeleteTexture() = 0;
#endif

	
};