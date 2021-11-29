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
			GLTexture* GetVideoTexture(const int& width, const int& height);

#ifdef WIN32
			cl_mem GetCopyVideoTexture(cl_context context);
			cl_mem GetOpenCLVideoTexturePt();
#endif
			GLTexture* GetVideoTexturePt();
			GLTexture* GetVideoTextureCopyPt();
			void SetSubtitle(CRegardsBitmap* subtitle);
			void ShowSubtitle();
			void DeleteSubtitle();


			void RenderWithEffect(GLTexture* glTexture, CVideoEffectParameter* effectParameter, const wxFloatRect& rect,
			                      const bool& inverted);
			void RenderWithEffectInterpolation(GLTexture* glTexture, GLTexture* glTextureOutput, const wxRect& rect,
			                                   CVideoEffectParameter* videoEffectParameter, const int& flipH,
			                                   const int& flipV, const int& angle, const int& filterInterpolation,
			                                   const bool& inverted = false);
			//void RenderWithEffect(const int &left, const int &top, GLTexture * glTexture, CVideoEffectParameter * effectParameter, const bool & flipH,const bool & flipV, const bool & inverted = false);
			//void RenderWithoutEffect(const int &left, const int &top, GLTexture * glTexture, const bool & flipH, const bool & flipV, const bool & inverted = false);

			bool IsCopyDirect()
			{
				return directcopytexture;
			}

		private:
			bool directcopytexture = true;
#ifdef WIN32
			void CopyTexture();
#endif

			GLTexture* textureSubtitle;
			GLTexture* textureVideo;
			GLTexture* textureVideoCopy;
			cl_mem cl_textureVideoCopy;
			CRenderOpenGL* renderOpenGL = nullptr;
			GLuint fboId;
		};
	}
}
