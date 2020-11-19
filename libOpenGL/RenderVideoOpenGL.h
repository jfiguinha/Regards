// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#ifdef RENDEROPENGL
#include <RenderOpenGL.h>
#include "EffectVideoParameter.h"
#include <RGBAQuad.h>
using namespace Regards::OpenGL;

//#define RENDEROPENGL WM_USER+1

class CRegardsBitmap;

namespace Regards
{
	namespace OpenGL
	{
		class CRenderVideoOpenGL : public CRenderOpenGL
		{
		public:

			CRenderVideoOpenGL(wxGLCanvas *canvas);
			virtual ~CRenderVideoOpenGL();

			void DeleteVideoTexture();
			GLTexture * GetVideoTexture(const int &width, const int &height);

#ifdef WIN32
			cl_mem GetCopyVideoTexture(cl_context context);
			cl_mem GetOpenCLVideoTexturePt();
#endif
            GLTexture * GetVideoTexturePt();
			GLTexture * GetVideoTextureCopyPt();
			void SetSubtitle(CRegardsBitmap * subtitle);
			void ShowSubtitle();
			void DeleteSubtitle();
			float GetVersion()
			{
				return myGLVersion;
			}
            
			void RenderWithEffect(GLTexture * glTexture, CVideoEffectParameter * effectParameter, const bool & inverted);
			void RenderWithEffectInterpolation(GLTexture * glTexture, GLTexture * glTextureOutput, const wxRect & rect, CVideoEffectParameter * videoEffectParameter, const int & flipH, const int & flipV, const int & angle, const int & filterInterpolation, const float & zoomRatio, const bool & inverted = false);
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

			GLTexture * textureSubtitle;
			GLTexture * textureVideo;
			GLTexture * textureVideoCopy;
			cl_mem cl_textureVideoCopy;

			GLuint fboId;
		};
	}
}

#endif