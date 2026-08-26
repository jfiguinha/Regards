#pragma once
#include "VideoStabilizationInterface.h"
#include "VideoStabilizationPimpl.h"


namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilizationOpenCL : public COpenCVStabilizationInterface
		{
		public:
			COpenCVStabilizationOpenCL(const int& nbFrame);
			~COpenCVStabilizationOpenCL() = default;

			void AddFrame(Regards::Picture::CPictureArray& pictureData) override;
			void BufferFrame(Regards::Picture::CPictureArray& pBitmap) override;
			Regards::Picture::CPictureArray CorrectFrame(Regards::Picture::CPictureArray& image) override;

			void Init() override;
			int GetNbFrame() override;
			int GetNbFrameBuffer() override;
			void SetNbFrameBuffer(const int& nbFrame) override;

		private:
			std::unique_ptr<CVideoStabilizationPimpl_<UMat>> pimpl;
		};
	}
}
