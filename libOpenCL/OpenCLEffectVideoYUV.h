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

			COpenCLEffectVideoYUV(COpenCLContext * context);
			virtual ~COpenCLEffectVideoYUV();
			void SetMemoryData(uint8_t * bufferY, int sizeY, uint8_t * bufferU, int sizeU, uint8_t * bufferV, int sizeV, const int &width, const int &height, const int &lineSize, const int &format = 0);
			virtual void TranscodePicture(const int &widthOut, const int &heightOut);
			bool IsOk();

		protected:

			int GetSizeData();
			bool isOk = false;
			int formatData;
			//Bitmap Memory Buffer
			COpenCLParameterInt * paramWidth = nullptr;
			COpenCLParameterInt * paramHeight = nullptr;
			COpenCLParameterByteArray * inputY = nullptr;
			COpenCLParameterByteArray * inputU = nullptr;
			COpenCLParameterByteArray * inputV = nullptr;
			COpenCLParameterInt * paramLineSize = nullptr;
		};

	}
}
