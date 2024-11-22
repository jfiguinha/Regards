#pragma once
#include "VideoStabilizationInterface.h"
#ifdef USE_CUDA
class COpenCVStabilizationCudaPimpl_;


namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilizationCuda : public COpenCVStabilizationInterface
		{
		public:
			COpenCVStabilizationCuda(const int& nbFrame);
			~COpenCVStabilizationCuda();

			void AddFrame(Regards::Picture::CPictureArray& pictureData) override;
			void BufferFrame(Regards::Picture::CPictureArray& pBitmap) override;
			Regards::Picture::CPictureArray CorrectFrame(Regards::Picture::CPictureArray& image) override;

			void Init() override;
			int GetNbFrame() override;
			int GetNbFrameBuffer() override;
			void SetNbFrameBuffer(const int& nbFrame) override;

		private:
			COpenCVStabilizationCudaPimpl_* pimpl;

		};
	}
}
#endif