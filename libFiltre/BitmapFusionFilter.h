#pragma once
#include "AfterEffect.h"


namespace Regards
{
	namespace Filter
	{
		class CBitmapFusionFilter : public IAfterEffect
		{
		public:
			CBitmapFusionFilter();
			~CBitmapFusionFilter();
			int GetTypeFilter();
			CRegardsBitmap * GenerateBitmapEffect(CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut);

#ifdef RENDEROPENGL
			void GenerateBitmapOpenCLEffect(GLTexture * glPicture, CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut);
			void GenerateTexture(GLTexture * glPicture, CImageLoadingFormat * nextPicture);
#endif
		private:

			CRegardsBitmap * bitmapOutCopy;
			CRegardsBitmap * bitmapTemp;

			int width;
			int height;
		};
	}
}

