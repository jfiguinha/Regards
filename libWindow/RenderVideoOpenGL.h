// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#include <RenderOpenGL.h>
using namespace Regards::OpenGL;

class CVideoEffectParameter;
class CRegardsBitmap;

namespace Regards
{
	namespace OpenGL
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

			void DeleteVideoTexture();
			GLTexture* GetVideoTexture(const int& width, const int& height, const bool& isOpenCLOpenGLInterop);


			GLTexture* GetVideoTexturePt();
			void SetSubtitle(cv::Mat & subtitle);
			void ShowSubtitle();
			void DeleteSubtitle();


			void RenderWithEffect(GLTexture* glTexture, CVideoEffectParameter* effectParameter, const wxFloatRect& rect, const float &iTime,
			                      const bool& inverted);


		private:
			GLTexture* textureVideo = nullptr;
			GLTexture* textureSubtitle = nullptr;
			CRenderOpenGL* renderOpenGL = nullptr;
			GLuint fboId;
		};
	}
}
