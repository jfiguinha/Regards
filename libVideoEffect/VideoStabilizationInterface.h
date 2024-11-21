#pragma once
#include <PictureArray.h>

namespace Regards
{
	namespace OpenCV
	{
		class COpenCVStabilizationInterface
		{
		public:

			virtual void AddFrame(Regards::Picture::CPictureArray& pictureData) = 0;
			virtual void BufferFrame(Regards::Picture::CPictureArray& pBitmap) = 0;
			virtual Regards::Picture::CPictureArray CorrectFrame(Regards::Picture::CPictureArray& image) = 0;

			virtual void Init() = 0;;
			virtual int GetNbFrame() = 0;
			virtual int GetNbFrameBuffer() = 0;
			virtual void SetNbFrameBuffer(const int& nbFrame) = 0;

		protected:

			int nbFrame = 0;
			int nbFrameBuffer = 0;
		};
	}
}
