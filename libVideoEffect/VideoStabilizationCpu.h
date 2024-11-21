#pragma once
#include "VideoStabilizationInterface.h"

class COpenCVStabilizationCpuPimpl_;


namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilizationCpu : public COpenCVStabilizationInterface
		{
		public:
			COpenCVStabilizationCpu(const int& nbFrame);
			~COpenCVStabilizationCpu();

			void AddFrame(Regards::Picture::CPictureArray& pictureData) override;
			void BufferFrame(Regards::Picture::CPictureArray& pBitmap) override;
			Regards::Picture::CPictureArray CorrectFrame(Regards::Picture::CPictureArray& image) override;

			void Init() override;
			int GetNbFrame() override;
			int GetNbFrameBuffer() override;
			void SetNbFrameBuffer(const int& nbFrame) override;

		private:
			COpenCVStabilizationCpuPimpl_* pimpl;

		};
	}
}
