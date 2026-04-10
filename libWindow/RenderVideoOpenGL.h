// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#include <RenderOpenGL.h>
using namespace Regards::OpenGL;

class CVideoEffectParameter;
class CRegardsBitmap;

namespace Regards::OpenGL
{
	struct wxFloatRect
	{
		float left = 0;
		float right = 0;
		float top = 0;
		float bottom = 0;
	};

	class CRenderVideoOpenGL
	{
	public:
		CRenderVideoOpenGL(CRenderOpenGL* renderOpenGL);
		~CRenderVideoOpenGL();
		GLTexture* GetVideoTexture(const int& width, const int& height);
		void SetVideoTexture(Regards::Picture::CPictureArray& pictureArray, const bool& deleteTexture = false);
		GLTexture* GetVideoTexturePt();
		void SetSubtitle(cv::Mat& subtitle);
		void ShowSubtitle();
		void Cleanup();
		void Render(CVideoEffectParameter* effectParameter, wxFloatRect& rect,
			const float& iTime, int& widthOut, const int& heightOut, const bool& flipH, const bool& flipV, const int& angle, wxRect& rc, const bool& inverted);
	private:

		bool RenderShaderInterpolation(const wxRect& rect, const bool& flipH, const bool& flipV, const int& angle, const bool& inverted, const int& interpolation);
		void RenderWithInterpolation(const int& widthOut, const int& heightOut, const bool& flipH, const bool& flipV, const int& angle, wxRect& rc, const bool& inverted);
		void RenderShader(GLSLShader* m_pShader, GLTexture* glTexture, CVideoEffectParameter* effectParameter, const wxFloatRect& rect, const float& iTime);
		GLTexture* textureVideo = nullptr;
		GLTexture* textureSubtitle = nullptr;
		CRenderOpenGL* renderOpenGL = nullptr;
		GLuint fboId;
		GLuint FFrameBuffer = 0;
		GLuint depthrenderbuffer = 0;
		int widthBuffer = 0;
		int heightBuffer = 0;
		bool firstTime = true;
        bool frameBufferSupport = false;
	};
}
