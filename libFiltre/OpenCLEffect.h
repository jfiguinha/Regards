#pragma once
#include "IFiltreEffet.h"
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
		class COpenCLProgram;
		class COpenCLParameter;
		class COpenCLParameterInt;
		class COpenCLParameterByteArray;
		class COpenCLParameterClMem;
		class COpenCLFilter;
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

			COpenCLEffect(const CRgbaquad &backColor,  CImageLoadingFormat * bitmap);

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			};
                
            cv::UMat GetUMat();
			cv::Mat GetMat();
			virtual ~COpenCLEffect();
            int LensDistortionFilter(const int &size);
			int BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect & listFace) { return -1; };
			int VignetteEffect(const double& radius = 1.0, const double& power = 0.8) { return -1; };
			int MeanShift(const float& fSpatialRadius, const float& fColorRadius) { return -1; };
			int BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP);
			int NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize, const int& searchWindowSize);
			int OilPaintingEffect(const int &size, const int &dynRatio) { return -1; };
			void Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle, int ratio);
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
			int Rotate180();
			int Negatif();
			int Sepia();
			int NoirEtBlanc();
			int Soften();
			int Blur(const int &radius);
			int GaussianBlur(const int& radius, const int& boxSize);
			int Emboss();
			int SharpenStrong();
			int Sharpen();
			int SharpenMasking(const float &sharpness);
			int FiltreMosaic(const int& size);
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
			int Fusion(cv::Mat& bitmapSecond, const float &pourcentage);
			int BrightnessAndContrast(const double &brightness, const double &contrast);
			int RGBFilter(const int &red, const int &green, const int &blue);
			int Swirl(const float &radius, const float &angle);
			
			
			int MotionBlur(const double &radius, const double &sigma, const double &angle);
			int CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave, const int &intensity) { return -1; };
			int Contrast(const double &contrast, const uint8_t &offset) { return -1; };
			int Lightness(const double &factor)  { return -1; };
			int LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)  { return -1; };

			int HistogramLog(cv::Mat & bitmap){ return  -1; }
			int HistogramNormalize(cv::Mat& bitmap){ return  -1; }
			int HistogramEqualize(cv::Mat& bitmap){ return  -1; }

			int BrightnessAndContrastAuto(float clipHistPercent);
			cv::Mat GetBitmap(const bool& source);

			bool StabilizeVideo(Regards::OpenCV::COpenCVStabilization* stabilization);

		protected:
			COpenCLFilter * openclFilter;
			wxImage GetwxImage(cv::UMat& input);
			int GetSizeData() const;
			cl_mem_flags  flag;
			wxString filename;
			cv::Mat alphaChannel;
			cv::UMat input;
			cv::UMat paramOutput;
		};

	}
}
