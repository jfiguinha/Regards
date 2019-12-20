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
	namespace Window
	{

		class CRenderBitmapInterfaceOpenGL : public CRenderOpenGL
		{
		public:

			CRenderBitmapInterfaceOpenGL(wxGLCanvas *canvas);
			virtual ~CRenderBitmapInterfaceOpenGL();
            void LoadingResource(const double & scale_factor);
			void ShowArrowNext();
			void ShowArrowPrevious();
			void ShowSecondBitmap(GLTexture * textureTransition, const int &width, const int &height, const int &left, const int &top);

		private:

			GLTexture * textureArrowRight;
			GLTexture * textureArrowLeft;

		};
	}
}
#endif