#pragma once
#include <IFiltreEffet.h>
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
#include <stdint.h>
#include <string>
#include <vector>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
using namespace std;
using namespace Regards::OpenCL;
class CRegardsBitmap;

namespace Regards
{
	namespace FiltreEffet
	{
		class COpenCLEffect : public IFiltreEffet
		{
		public:

			COpenCLEffect(CRegardsBitmap * pBitmap, const CRgbaquad &backColor);

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			};

			~COpenCLEffect();

			int RGB24ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size);
			int YUV420ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size);
			int NV12ToRGB32(uint8_t * buffer, const int &width, const int &height, CRegardsBitmap * & bitmapOut, const int &size);
			int InterpolationBicubicNV12ToRGB32(uint8_t * buffer, const int &width, const int &height,
				const int &rectWidth, const int &rectHeight, CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle, const int &size);
			int InterpolationBicubicRGB32Video(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle);
			int NiveauDeGris();
			int RedEye(const wxRect& rSelectionBox){ return -1; };
			int HistogramLog(){ return -1; };
			int HistogramNormalize(){ return -1; };
			int HistogramEqualize(){ return -1; };
			wxImage InterpolationBicubic(const wxImage & imageSrc, const int &widthOut, const int &heightOut);
			int InterpolationBicubic(CRegardsBitmap * & bitmapOut, const int &flipH, const int &flipV, const int &angle);
			int InterpolationBicubic(CRegardsBitmap * & bitmapOut);
			int InterpolationBicubic(CRegardsBitmap * & bitmapOut, const wxRect &rc);
			int InterpolationBilinear(CRegardsBitmap * & bitmapOut);
			int InterpolationBilinear(CRegardsBitmap * & bitmapOut, const wxRect &rc);
			int InterpolationFast(CRegardsBitmap * & bitmapOut);
			int InterpolationFast(CRegardsBitmap * & bitmapOut, const wxRect &rc);
			int FlipVertical();
			int FlipHorizontal();
			int SharpenMasking(const int &sharpness);
			int Rotate90();
			int Rotate270();
			int Negatif();
			int Sepia();
			int NoirEtBlanc();
			int Soften();
			int Blur();
			int GaussianBlur();
			int Emboss();
			int SharpenStrong();
			int Sharpen();
			int RotateFree(const double &angle, const int &widthOut, const int &heightOut);
			int FiltreMosaic();
			int FiltreEdge();
			int Erode();
			int Noise();
			int Median();
			int Dilate();
			int Posterize(const float &level, const float &gamma);
			int Solarize(const long &threshold);
			int PhotoFiltre(const CRgbaquad &clValue, const int &intensity);
			int Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage);
			int BrightnessAndContrast(const double &brightness, const double &contrast);
			int RGBFilter(const int &red, const int &green, const int &blue);
			int Swirl(const float &radius, const float &angle);

			int MotionBlur(const double &radius, const double &sigma, const double &angle) { return  -1; }
			int Resize(const int &imageWidth, const int &imageHeight, const int &interpolation){ return -1; };
			int CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave) { return -1; };
			int Contrast(const double &contrast, const uint8_t &offset) { return -1; };
			int Lightness(const double &factor)  { return -1; };
			int LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)  { return -1; };

			int HistogramLog(CRegardsBitmap * bitmap){ return  -1; }
			int HistogramNormalize(CRegardsBitmap * bitmap){ return  -1; }
			int HistogramEqualize(CRegardsBitmap * bitmap){ return  -1; }

		protected:
			COpenCLProgram * GetProgram(const wxString &numProgram);
			COpenCLContext * GetContext();
			int FiltreConvolution(const wxString &functionName);
			int Flip(const wxString &functionName);
			int ColorEffect(const wxString &functionName);
			int Rotate(const wxString &functionName, const int &widthOut, const int &heightOut, const double &angle);
			int Interpolation(CRegardsBitmap * & bitmapOut, const wxRect &rc, const wxString &functionName);
			int Interpolation(CRegardsBitmap * & bitmapOut, const wxString &functionName);
			cl_mem_flags  flag;
			COpenCLProgram * openCLProgram = nullptr;
		};

	}
}
