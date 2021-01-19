#pragma once
#include <RGBAQuad.h>
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
#include "OpenCLParameter.h"
#include <stdint.h>
#include <string>
#include <vector>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <hqdn3d.h>


using namespace std;
using namespace Regards::OpenCL;
class CRegardsBitmap;

namespace Regards
{
	namespace OpenCL
	{

		class COpenCLFilter
		{
		public:

			COpenCLFilter(COpenCLContext * context);

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			};

			virtual ~COpenCLFilter();

			COpenCLProgram * GetProgram(const wxString &numProgram);
			cl_mem HQDn3D(const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp, cl_mem inputData, int width, int height);
			cl_mem Denoise(const wxString &functionName, const float &sigma, const float &threshold, const float &kSigma, cl_mem inputData, int width, int height);
			cl_mem FiltreConvolution(const wxString &programName, const wxString &functionName, cl_mem inputData, int width, int height);
			cl_mem MotionBlurCompute(const vector<double> & kernelMotion, const vector<wxPoint> & offsets, const int &size, cl_mem inputData, int width, int height);
			cl_mem bilat2(int fSize,  float sigmaX, float sigmaP, cl_mem inputData, int width, int height);
			cl_mem run2d(const int & FSIZE, const int & BSIZE,const float & SIGMA, cl_mem inputData, int width, int height);
			cl_mem bilateral_filter(const int & radius, const int & preserve, cl_mem inputData, int width, int height);
			cl_mem SharpenMasking(const float &sharpness, cl_mem inputData, int width, int height);
			cl_mem PhotoFiltre(const CRgbaquad &clValue, const int &intensity, cl_mem inputData, int width, int height);
			cl_mem RGBFilter(const int &red, const int &green, const int &blue, cl_mem inputData, int width, int height);
			cl_mem FiltreMosaic(cl_mem inputData, int width, int height);
			cl_mem Blur(const int &radius, cl_mem inputData, int width, int height);
			cl_mem BoxBlur(const int &coeff, const wxString &functionName, cl_mem inputData, int width, int height, bool noDeleteData = false);
			cl_mem ErodeDilate(const wxString &functionName, cl_mem inputData, int width, int height);
			cl_mem Posterize(const float &level, const float &gamma, cl_mem inputData, int width, int height);
			cl_mem Solarize(const long &threshold, cl_mem inputData, int width, int height);
			cl_mem Median(cl_mem inputData, int width, int height);
			cl_mem Noise(cl_mem inputData, int width, int height);
			cl_mem Flip(const wxString &functionName, cl_mem inputData, int width, int height);
			cl_mem Swirl(const float &radius, const float &angle, cl_mem inputData, int width, int height);
			cl_mem BrightnessAndContrast(const double &brightness, const double &contrast, cl_mem inputData, int width, int height);
			cl_mem ColorEffect(const wxString &functionName, cl_mem inputData, int width, int height);
			cl_mem Rotate(const wxString &functionName, const int &widthOut, const int &heightOut, const double &angle, cl_mem inputData, int width, int height);
			cl_mem Interpolation(const int &widthOut, const int &heightOut, const wxString &functionName, const int& method, cl_mem inputData, int width, int height, int flipH, int flipV, int angle);
			cl_mem Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const wxString &functionName, const int& method, cl_mem inputData, int width, int height, int flipH, int flipV, int angle);
			cl_mem Fusion(cl_mem inputData, cl_mem secondPictureData, const float &pourcentage, int width, int height);
			cl_mem ConvertToY(cl_mem inputData, int width, int height, const wxString & functionName = "ConvertToY");
			cl_mem InsertYValue(cl_mem inputData, cl_mem sourceData, int width, int height, const wxString & functionName = "InsertYValue");
			cl_mem ExtractBlocSize(cl_mem sourceData, const int & size, const int & marge, const int & width, const int & height, const int & x, const int & y);
			void InsertBlockSize(cl_mem sourceData, cl_mem wienerData, const int & size, const int & marge, const int & width, const int & height, const int & x, const int & y);
		protected:

			int GetSizeData();
			cl_mem_flags  flag;
			COpenCLContext * context;
			Chqdn3d * hq3d = nullptr;
			double oldLevelDenoise = 0;
			int oldwidthDenoise = 0;
			int oldheightDenoise = 0;
		};

	}
}
