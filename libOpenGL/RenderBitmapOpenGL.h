// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#ifdef RENDEROPENGL
#include <RenderOpenGL.h>
#include <RGBAQuad.h>
#include <EffectParameter.h>
using namespace Regards::OpenGL;

//#define RENDEROPENGL WM_USER+1

class CRegardsBitmap;

namespace Regards
{
	namespace OpenGL
	{

		class CRenderBitmapOpenGL : public CRenderOpenGL
		{
		public:

			CRenderBitmapOpenGL(wxGLCanvas *canvas);
			virtual ~CRenderBitmapOpenGL();
            void LoadingResource(const double & scale_factor);
			void ShowArrowNext();
			void ShowArrowPrevious();
			void ShowSecondBitmap(GLTexture * textureTransition, const int &width, const int &height, const int &left, const int &top);
			void RenderWithAlphaChannel(GLTexture * glTexture, const int &alpha, const int &left, const int &top, const bool & flipH, const bool & flipV, const bool & inverted);
			void ShowSecondBitmapWithAlpha(GLTexture * textureTransition, const int &alpha, const int &width, const int &height, const int &left, const int &top);

		private:

			GLTexture * textureArrowRight;
			GLTexture * textureArrowLeft;

		};
	}
}
#endif