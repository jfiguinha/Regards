#pragma once
#include "BitmapFusionFilter.h"

class CRegardsBitmap;
class CImageLoadingFormat;
class IBitmapDisplay;

namespace Regards
{
	namespace Filter
	{
		class CPageCurlFilter : public CBitmapFusionFilter
		{
		public:
			CPageCurlFilter();
			~CPageCurlFilter();
			virtual int GetTypeFilter();
			virtual void GenerateTexture(CImageLoadingFormat * nextPicture, CImageLoadingFormat * source, IBitmapDisplay * bmpViewer);
			virtual GLTexture * GetTexture(const int &numTexture);
			virtual void DeleteTexture();
		private:

			
			Regards::OpenGL::GLTexture * pictureFirst;
			CRegardsBitmap * bitmapNext = nullptr;
			CRegardsBitmap * bitmapFirst = nullptr;
			bool initTexture = false;
		};
	}
}

