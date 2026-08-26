#pragma once
#include <PictureArray.h>
#include "VideoStabilizationInterface.h"
#define TYPE_CPU 1
#define TYPE_OPENCL 2


namespace cv
{
	class UMat;
	class Mat;
}

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilization
		{
		public:
			COpenCVStabilization(const int& nbFrame, const int &type);
			~COpenCVStabilization() = default;

			void AddFrame(Regards::Picture::CPictureArray& pictureData);
			void BufferFrame(Regards::Picture::CPictureArray& pBitmap);
			Regards::Picture::CPictureArray CorrectFrame(Regards::Picture::CPictureArray& image);

			void Init();
			int GetNbFrame();
			int GetNbFrameBuffer();
			void SetNbFrameBuffer(const int& nbFrame);

		private:
			
			std::unique_ptr<COpenCVStabilizationInterface> opencvStabilization = nullptr;
			int type = 0;
		};
	}
}
