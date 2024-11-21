#pragma once
#include "VideoStabilizationInterface.h"

class COpenCVStabilizationOpenCLPimpl_;

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilizationOpenCL : public COpenCVStabilizationInterface
		{
		public:
			COpenCVStabilizationOpenCL(const int& nbFrame);
			~COpenCVStabilizationOpenCL();

			void AddFrame(Regards::Picture::CPictureArray& pictureData) override;
			void BufferFrame(Regards::Picture::CPictureArray& pBitmap) override;
			Regards::Picture::CPictureArray CorrectFrame(Regards::Picture::CPictureArray& image) override;

			void Init() override;
			int GetNbFrame() override;
			int GetNbFrameBuffer() override;
			void SetNbFrameBuffer(const int& nbFrame) override;

		private:
			COpenCVStabilizationOpenCLPimpl_ * pimpl;
		};
	}
}
