#pragma once
#include "AfterEffect.h"
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
namespace Regards
{
	namespace OpenCL
	{
		class COpenCLEffectVideo;
	}
}

using namespace Regards::OpenCL;

namespace Regards
{
	namespace Viewer
	{
		class CBitmapFusionFilter : public IAfterEffect
		{
		public:
			CBitmapFusionFilter();
			~CBitmapFusionFilter();
			int GetTypeFilter();
			CRegardsBitmap * GenerateBitmapEffect(CImageLoadingFormat * nextPicture, CEffectParameter * effectParameter, CBitmapWndViewer * bmpViewer, wxRect &rcOut);
			void GenerateBitmapOpenCLEffect(GLTexture * glPicture, CImageLoadingFormat * nextPicture, CEffectParameter * effectParameter, CBitmapWndViewer * bmpViewer, wxRect &rcOut);
		private:

			CRegardsBitmap * bitmapOutCopy;
			CRegardsBitmap * bitmapTemp;
			COpenCLEffectVideo * openclEffectVideo;
			cl_mem cl_nextPicture;
			int width;
			int height;
		};
	}
}

