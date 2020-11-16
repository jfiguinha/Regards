#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

class CRegardsBitmap;
class CRegardsFloatBitmap;
class CVideoEffectParameter;

namespace Regards
{
	namespace OpenCL
	{

		class COpenCLContext;
		class COpenCLProgram;
		class COpenCLParameter;
		class COpenCLParameterInt;
		class COpenCLParameterByteArray;
		class COpenCLParameterClMem;

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
			CRegardsBitmap* GetRgbaBitmap();
			virtual void InterpolationBicubic(const int& widthOut, const int& heightOut, const int &flipH, const int &flipV, const int& angle, const int& bicubic);
			virtual void InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect &rc, const int &flipH, const int &flipV, const int& angle, const int& bicubic);
			virtual void TranscodePicture(const int &widthOut, const int &heightOut) {};
			int SetAlphaValue(cl_mem cl_image, const int& width, const int& height, const int& alphaValue);
			void ApplyVideoEffect(CVideoEffectParameter * videoEffectParameter);
			int InterpolationBicubicOpenGLTexture(cl_mem cl_image, const int& width, const int& height, const int& widthOutput, const int& heightOutput, const int & flipH, const int &flipV, const int& angle, const int& bicubic);
			int InterpolationBicubicZoneOpenGLTexture(cl_mem cl_openglTexture, const int& width, const int& height, const int& widthOutput, const int& heightOutput, const wxRect &rc, const int & flipH, const int &flipV, const int& angle, const int& bicubic);

			void GetRgbaBitmap(cl_mem cl_image, int rgba = 0);
			int GetWidth();
			int GetHeight();
			int GetThumbnailWidth();
			int GetThumbnailHeight();

		protected:

			int GetSizeData();
			CRegardsBitmap* GetBitmap(cl_mem input, const int& width, const int& height);
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
