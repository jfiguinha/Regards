#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif


namespace cv
{
	class Mat;
}


namespace Regards
{
	namespace OpenCL
	{

		class COpenCLConvertFormat
		{
		public:
			COpenCLConvertFormat() {};
			virtual ~COpenCLConvertFormat() {};

			static cv::Mat SetNV12(const cv::Mat& yuv);
			static cv::Mat SetNV12(uint8_t* bufferY, int sizeY, uint8_t* bufferUV, int sizeUV, const int& width,
				const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
				const int& colorRange, const int& colorSpace);
			static cv::Mat SetNV12(const cv::Mat& yuv, const int& linesize, const int& nWidth, const int& nHeight);
			static cv::Mat SetYUV420P(uint8_t* bufferY, int sizeY, uint8_t* bufferU, int sizeU, uint8_t* bufferV, int sizeV,
				const int& width, const int& height, const int& lineSize, const int& widthOut,
				const int& heightOut, const int& colorRange, const int& colorSpace);
			static cv::Mat SetYUV420P(const cv::Mat& y, const cv::Mat& u, const cv::Mat& v, const int& linesize,
				const int& nWidth, const int& nHeight);
		};
	}
}
