// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#include <RGBAQuad.h>
#include <OpenCLEngine.h>
#include <DecodeRawParameter.h>

class CImageLoadingFormat;
class CEffectParameter;
class CFiltreEffet;
using namespace Regards::OpenCL;

namespace Regards
{
	namespace Window
	{

		class CRenderPreviewBitmap
		{
		public:
			CRenderPreviewBitmap();
			virtual ~CRenderPreviewBitmap();
			void SetBackgroundColor(const CRgbaquad &backColor);
			void SetNewBitmap(CImageLoadingFormat * newbitmap, wxWindow * parent, COpenCLContext * openclContext);
			void RenderEffect(const int &numEffect, CEffectParameter * effectParameter);
			void RotateFree(const int &angle);
			void MotionBlur(const double &radius, const double &sigma, const double &angle);
			void WaveFilter(const int &iPosX, const int &iPosY, short height,int radius, int scale, const int &posLeft, const int &posTop);
			void LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity, const int &posLeft, const int &posTop);
			wxImage GetRender();
            wxImage GetLastRender();
            CFiltreEffet * GetFiltre();
            
		private:

			int pictureWidth;
            int pictureHeight;
			CRgbaquad colorBackground;
			CFiltreEffet * filtre;
            wxImage lastRender;
		};
	}
}