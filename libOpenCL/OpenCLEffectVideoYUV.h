#pragma once
#include "OpenCLEffectVideo.h"
using namespace Regards::OpenCL;


namespace Regards
{
	namespace OpenCL
	{
		class COpenCLEffectVideoYUV : public COpenCLEffectVideo
		{
		public:

			COpenCLEffectVideoYUV();
			virtual ~COpenCLEffectVideoYUV();
			void SetMemoryData(const cv::Mat& bufferY, const cv::Mat& bufferU,const cv::Mat & bufferV, const int &width, const int &height, const int &lineSize);
			void SetMemoryDataNV12(const cv::Mat& bufferY, const cv::Mat& bufferUV, const int &width, const int &height, const int &lineSize);
			virtual void TranscodePicture(const int &widthOut, const int &heightOut, const int& rgba = 0);
			bool IsOk();

		protected:

			int formatData = 0;
			
			//Bitmap Memory Buffer
			COpenCLParameterInt * paramWidth = nullptr;
			COpenCLParameterInt * paramHeight = nullptr;
			cv::UMat inputY;
			cv::UMat inputU;
			cv::UMat inputV;
			COpenCLParameterInt * paramLineSize = nullptr;

		};

	}
}
