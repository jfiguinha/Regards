#pragma once
#include <IFiltreEffet.h>
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
#include "OpenCLParameter.h"
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
using namespace Regards::OpenCL;
class CRegardsBitmap;
class CRegardsFloatBitmap;

namespace Regards
{
	namespace FiltreEffet
	{
		
		class COpenCLEffect : public IFiltreEffet
		{
		public:

			COpenCLEffect(const CRgbaquad &backColor, COpenCLContext * context, CImageLoadingFormat * bitmap);

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			};

			virtual ~COpenCLEffect();

            int Bm3d(const int & fSigma);

			int ClaheFilter(int nBins, float clipLevel, int windowSize){ return -1; };
			int BilateralFilter(int fSize,  float sigmaX, float sigmaP);
			int NlmeansFilter(int fsize, int bsize, float sigma);

			void Interpolation(const int &widthOut, const int &heightOut, const int &method, int flipH, int flipV, int angle);
			void Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle);
			
			int NiveauDeGris();
			int RedEye(){ return -1; };
			int HistogramLog(){ return -1; };
			int HistogramNormalize(){ return -1; };
			int HistogramEqualize(){ return -1; };

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

			int GetRgbaBitmap(void * cl_image);
			cl_mem LoadRegardsImage(uint8_t * data, const int &width, const int &height);
			cl_mem LoadCxImageAlpha(uint8_t * data, uint8_t * alpha, const int &width, const int &heighta, const int &effwidth, const bool & RGB = 0);
			cl_mem LoadCxImage(uint8_t * data, const int &width, const int &height, const int &effwidth, const bool & RGB = 0);
			cl_mem LoadWxImageAlpha(uint8_t * data, uint8_t * alpha, const int &width, const int &heighta, const int &effwidth);
			cl_mem LoadWxImage(uint8_t * data, const int &width, const int &height, const int &effwidth);
			cl_mem LoadFloatImage(float* data, const int& width, const int& height);
			CRegardsBitmap* GetBitmap(const bool& source);
			CRegardsFloatBitmap* GetFloatBitmap(const bool& source);

			virtual int GetWidth();
			virtual int GetHeight();

		protected:

			int GetSizeData();

			cl_mem_flags  flag;
			wxString filename;
			COpenCLContext * context;
			void SetOutputValue(cl_mem output, int widthOutput, int heightOutput);
			void RefreshMemoryBitmap(CRegardsBitmap * bitmapOut);
			void RefreshMemoryBitmap(cl_mem bitmapOut, const int & widthOutput, const int &  heightOutput);

			CRegardsFloatBitmap* GetFloatBitmap(cl_mem input, const int& width, const int& height);
			CRegardsBitmap * GetBitmap(cl_mem input, const int &width, const int &height);

			//Bitmap Memory Buffer
			bool dataIsOk ;
			COpenCLParameter * input;
			COpenCLParameterInt * paramWidth;
			COpenCLParameterInt * paramHeight;


			int width;
			int height;

			COpenCLParameterClMem * paramOutput;
			int widthOut;
			int heightOut;
		};

	}
}
