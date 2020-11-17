#pragma once
#include "OpenCLEffectVideo.h"
using namespace Regards::OpenCL;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLEffectVideoNV12 : public COpenCLEffectVideo
		{
		public:

			COpenCLEffectVideoNV12(COpenCLContext * context);
			virtual ~COpenCLEffectVideoNV12();
			bool IsOk();
			void SetMemoryData(uint8_t * bufferY, int sizeY, uint8_t * bufferUV, int sizeUV, const int &width, const int &height, const int &widthPitch, const int &format = 0);
			void SetMemoryData(uint8_t * buffer, int size, const int &width, const int &height, const int &widthPitch, const int &heightSurface, const int &format = 0);
			//void InterpolationBicubic(const int &widthOut, const int &heightOut, const int &angle, const int &bicubic);
			void TranscodePicture(const int &widthOut, const int &heightOut);
		protected:

			int typeData;
			int formatData;
			//Bitmap Memory Buffer
			COpenCLParameterInt * paramWidth = nullptr;
			COpenCLParameterInt * paramHeight = nullptr;
			COpenCLParameterByteArray * inputY = nullptr;
			COpenCLParameterByteArray * inputUV = nullptr;
			COpenCLParameterByteArray * input = nullptr;
			COpenCLParameterInt * paramwidthPitch = nullptr;
			COpenCLParameterInt * paramsurfaceHeight = nullptr;
			bool isOk = false;
		};

	}
}
