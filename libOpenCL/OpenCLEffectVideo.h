#pragma once
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
#include "OpenCLParameter.h"
#include <EffectVideoParameter.h>
#include <RGBAQuad.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

using namespace Regards::OpenCL;
class CRegardsBitmap;
class CRegardsFloatBitmap;

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLEffectVideo
		{
		public:

			COpenCLEffectVideo(COpenCLContext * context);

			void SetFlag(const bool &useMemory)
			{
				flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
			};

			virtual ~COpenCLEffectVideo();
			bool IsOk();
			void ApplyVideoEffect(CVideoEffectParameter * videoEffectParameter);
			wxImage GetWxImage();
            void GetWxImage(wxImage * image);
			CRegardsBitmap * GetRgbaBitmap();
            void GetRgbaBitmap(CRegardsBitmap * bitmap);
			void GetRgbaBitmap(cl_mem cl_image, int rgba = 0);
			void SetRegardsBitmap(CRegardsBitmap * bitmap);
			void SetRgbaBitmap(cl_mem cl_image, const int &width, const int &height);
			int SetAlphaValue(cl_mem cl_image, const int &width, const int &height, const int &alphaValue);
			int InterpolationBicubicOpenGLTexture(cl_mem cl_image, const int &width, const int &height, const int &widthOutput, const int &heightOutput, const int &angle, const int &bicubic);
            CRegardsFloatBitmap * GetFloatBitmap(const bool &source = true);
            void GetFloatBitmap(CRegardsFloatBitmap *  bitmapOut);
            virtual void InterpolationBicubic(const int &widthOut, const int &heightOut, const int &angle, const int &bicubic);


			int GetWidth();
			int GetHeight();
			int GetThumbnailWidth();
			int GetThumbnailHeight();
		protected:

			int GetSizeData();
            CRegardsBitmap * GetBitmap(cl_mem input, const int &width, const int &height);
            void GetBitmap(CRegardsBitmap * bitmap, cl_mem input, const int &width, const int &height);
			COpenCLProgram * GetProgram(const wxString &numProgram);
			cl_mem_flags  flag;


			COpenCLProgram * openCLProgram;
			COpenCLContext * context;
			bool dataIsOk;
			//Bitmap Memory Buffer
			COpenCLParameterInt * paramWidth;
			COpenCLParameterInt * paramHeight;
			COpenCLParameterByteArray * paramInput;

			int widthOut;
			int heightOut;
			int width;
			int height;
			COpenCLParameterClMem * paramOutput;
		};

	}
}
