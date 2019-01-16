// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#include <RenderOpenGL.h>
#include "EffectVideoParameter.h"
#include <RGBAQuad.h>
using namespace Regards::OpenGL;

#define RENDEROPENGL WM_USER+1

class CRegardsBitmap;

namespace Regards
{
	namespace Video
	{
		class CRenderBitmapInterfaceOpenGL : public CRenderOpenGL
		{
		public:

			CRenderBitmapInterfaceOpenGL(wxGLCanvas *canvas);
			virtual ~CRenderBitmapInterfaceOpenGL();

			void DeleteVideoTexture();
			GLTexture * GetVideoTexture(const int &width, const int &height);
			cl_mem GetCopyVideoTexture(cl_context context);
			cl_mem GetOpenCLVideoTexturePt();
            GLTexture * GetVideoTexturePt();
#ifndef __WXGTK__
			GLTexture * GetCopyTextureBitmap(const int &width, const int &height, cl_context context);
#endif
			void SetSubtitle(CRegardsBitmap * subtitle);
			void ShowSubtitle();
			void DeleteSubtitle();
			float GetVersion()
			{
				return myGLVersion;
			}
            
            void RenderWithEffect(const int &left, const int &top, GLTexture * glTexture, CVideoEffectParameter * effectParameter, const bool & flipH,const bool & flipV, const bool & inverted = false);
          
            bool IsCopyDirect()
            {
                return directcopytexture;
            }
		private:

            bool directcopytexture = true;
#ifndef __WXGTK__
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
