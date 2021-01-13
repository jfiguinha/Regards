#pragma once
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

class CRegardsBitmap;
class CRegardsFloatBitmap;
class CVideoEffectParameter;
class Chqdn3d;
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
			CRegardsBitmap* GetRgbaBitmap(const bool &src = false);
			CRegardsBitmap* GetBitmap(cl_mem cl_image);
			
			virtual void InterpolationBicubic(const int& widthOut, const int& heightOut, const int &flipH, const int &flipV, const int& angle, const int& bicubic);
			virtual void InterpolationZoomBicubic(const int& widthOutput, const int& heightOutput, const wxRect &rc, const int &flipH, const int &flipV, const int& angle, const int& bicubic);
			virtual void TranscodePicture(const int &widthOut, const int &heightOut) {};
			void HQDn3D(Chqdn3d * hq3d, const double & LumSpac, const double & ChromSpac = 4, const double & LumTmp = 3, const double & ChromTmp = 3);
            void FlipVertical();
#if defined(__x86_64__) || defined(_M_AMD64)	
		void GetRgbaBitmap(cl_mem cl_image, int rgba = 0);
#elif defined(__APPLE__)
        void GetRgbaBitmap(cl_mem cl_image, int rgba = 1);
#endif 
			
			int GetSrcWidth();
			int GetSrcHeight();
			int GetThumbnailWidth();
			int GetThumbnailHeight();
			int CopyOpenGLTexture(cl_mem cl_openglTexture, const int& width, const int& height);

			COpenCLParameterClMem * GetPtData();

		protected:

			int GetSizeData();
			CRegardsBitmap* GetBitmap(cl_mem input, const int& width, const int& height);
			COpenCLProgram * GetProgram(const wxString &numProgram);
			cl_mem_flags  flag;


			COpenCLProgram * openCLProgram;
			COpenCLContext * context;
			//bool dataIsOk;
			//Bitmap Memory Buffer

			//COpenCLParameterByteArray * paramInput;

			int widthOut;
			int heightOut;
			int srcwidth;
			int srcheight;
			COpenCLParameterClMem * paramOutput = nullptr;
			COpenCLParameterInt * paramOutWidth = nullptr;
			COpenCLParameterInt * paramOutHeight = nullptr;

			COpenCLParameterInt * paramSrcWidth = nullptr;
			COpenCLParameterInt * paramSrcHeight = nullptr;
			COpenCLParameterClMem * paramSrc = nullptr;
		};

	}
}
