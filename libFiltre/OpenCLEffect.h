#pragma once
#include "IFiltreEffet.h"
#include <GLTexture.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
class CRegardsBitmap;
class CRegardsFloatBitmap;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLContext;
		class COpenCLProgram;
		class COpenCLParameter;
		class COpenCLParameterInt;
		class COpenCLParameterByteArray;
		class COpenCLParameterClMem;
	}
}

using namespace Regards::OpenCL;
using namespace Regards::OpenGL;
namespace Regards
{
	namespace FiltreEffet
	{
		class COpenCLEffect : public IFiltreEffet
		{
		public:

			COpenCLEffect(const CRgbaquad &backColor, COpenCLContext * context, COpenCLParameterClMem * data_mem, const int &width, const int &height);
			COpenCLEffect(const CRgbaquad &backColor, COpenCLContext * context, CImageLoadingFormat * bitmap);

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			};


			void CopyPictureToTexture2D(GLTexture * texture, const bool& source);
			virtual ~COpenCLEffect();
			int BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect & listFace) { return -1; };
			int VignetteEffect(const double& radius = 1.0, const double& power = 0.8) { return -1; };
            int Bm3d(const int & fSigma);
			int MeanShift(const float& fSpatialRadius, const float& fColorRadius) { return -1; };
			int BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP);
			int NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize, const int& searchWindowSize);
			int OilPaintingEffect(const int &size, const int &dynRatio) { return -1; };
			void Interpolation(const int &widthOut, const int &heightOut, const int &method, int flipH, int flipV, int angle);
			void Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle);
			int CartoonifyImage(const int & mode) { return -1; };
			int NiveauDeGris();
			int RedEye(){ return -1; };
			int HistogramNormalize(){ return -1; };
			int HistogramEqualize(){ return -1; };
			int HQDn3D(const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp);
			wxImage GetwxImage();
            void SetBitmap(CImageLoadingFormat * bitmap);
			int FlipVertical();
			int FlipHorizontal();
			int WaveFilter(int x, int y, short height, int scale, int radius);
			int Rotate90();
			int Rotate270();
			int Negatif();
			int Sepia();
			int NoirEtBlanc();
			int Soften();
			int Blur(const int &radius);
			int GaussianBlur(const int &radius, const int &boxsize);
			int Emboss();
			int SharpenStrong();
			int Sharpen();
			int SharpenMasking(const float &sharpness);
			int FiltreMosaic();
			int FiltreEdge();
			int Erode();
			int Noise();
			int Median();
			int Dilate();
			int GetWidth();
			int GetHeight();

			int RotateFree(const double &angle, const int &widthOut, const int &heightOut);
			int Posterize(const float &level, const float &gamma);
			int Solarize(const long &threshold);
			int PhotoFiltre(const CRgbaquad &clValue, const int &intensity);
			int Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage);
			int BrightnessAndContrast(const double &brightness, const double &contrast);
			int RGBFilter(const int &red, const int &green, const int &blue);
			int Swirl(const float &radius, const float &angle);
			
			
			int MotionBlur(const double &radius, const double &sigma, const double &angle);
			int CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave, const int &intensity) { return -1; };
			int Contrast(const double &contrast, const uint8_t &offset) { return -1; };
			int Lightness(const double &factor)  { return -1; };
			int LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)  { return -1; };

			int HistogramLog(CRegardsBitmap * bitmap){ return  -1; }
			int HistogramNormalize(CRegardsBitmap * bitmap){ return  -1; }
			int HistogramEqualize(CRegardsBitmap * bitmap){ return  -1; }

			int BrightnessAndContrastAuto(float clipHistPercent);
			CRegardsBitmap* GetBitmap(const bool& source);

		protected:


			int GetSizeData() const;
			COpenCLContext* context = nullptr;
			cl_mem_flags  flag;
			wxString filename;

			cv::UMat input;
			cv::UMat paramOutput;
		};

	}
}
