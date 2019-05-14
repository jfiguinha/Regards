// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#include <RenderOpenGL.h>
#include <RGBAQuad.h>
#include <EffectParameter.h>
using namespace Regards::OpenGL;

#define RENDEROPENGL WM_USER+1

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

			void ApplyPreviewEffect(const int &left, const int &top, const int &numEffect, CEffectParameter * effectParameter);
            void LoadingResource(const double & scale_factor);
			void ShowArrowNext();
			void ShowArrowPrevious();
			void ShowTransitionBitmap(GLTexture * textureTransition, const int &width, const int &height, const int &left, const int &top, const float &alphaValue);

		private:

			//Effect
			void PhotoFiltre(const int &left, const int &top, CEffectParameter * effectParameter);
			void RGBFiltre(const int &left, const int &top, CEffectParameter * effectParameter);



			GLTexture * textureArrowRight;
			GLTexture * textureArrowLeft;

		};
	}
}
