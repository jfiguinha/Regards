#pragma once
#include "RenderBitmapOpenGL.h"
;
class CImageLoadingFormat;
class CEffectParameter;
class IBitmapDisplay;

namespace Regards::OpenGL
{
	class GLTexture;
}

using namespace Regards::OpenGL;

class IAfterEffect
{
public:
    
    static void CheckGLError(const char* where)
    {
        GLenum error;

        while ((error = glGetError()) != GL_NO_ERROR)
        {
            printf("%s : OpenGL error = 0x%04X\n",
                   where,
                   static_cast<unsigned int>(error));
        }
    }

	IAfterEffect();

	virtual ~IAfterEffect()
	{
	};

	virtual void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
	{
	};
	virtual int GetTypeFilter() = 0;

	virtual void RenderMoveTexture(int& x, int& y, GLTexture* glTexture, const int& etape, const bool& isNext)
	{
	};

	virtual void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL,
	                         IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext,
	                         float& ratio)
	{
	};

	virtual bool RenderTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source, IBitmapDisplay* bmpViewer,
	                           CRenderBitmapOpenGL* renderOpenGL, const float& scale_factor, const int& etape)
	{
		return false;
	};
	virtual GLTexture* GetTexture(const int& numTexture) = 0;

	virtual bool NeedToReload()
	{
		return true;
	}

protected:
	
	std::unique_ptr<GLTexture> pictureNext;
	std::unique_ptr<GLTexture> pictureFirst;
};
