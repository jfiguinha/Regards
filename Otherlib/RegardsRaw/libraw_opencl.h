#pragma once
#include <xmmintrin.h>

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLRawDev;

		class CLibRawOpenCL
		{
		public:
			CLibRawOpenCL(void * openclContext);
			~CLibRawOpenCL();
			void InitData(void * dataInput, int sizeInput, int sizeOutput, int _width, int _height, int scale);
			void WaveletDenoiseChannel(int size, float _threshold, float * noise);
			void * GetTempData();
			void * GetOutputData(void * dataOutput, int sizeOutput, int lpass);
			void WaveletDenoiseCol(int hpass, int lpass, int sc);
			void WaveletDenoiseRow(int lpass, int sc);
			void WaveletDenoiseNormalize(int size, int hpass, int lpass, float thold);
			void * GetAlphaChannel(void * dataOutput, int sizeOutput);
		private:
			COpenCLRawDev * openclRawDev;
		};
	}
}