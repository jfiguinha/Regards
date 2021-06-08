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
			virtual int GetTypeFilter();
			virtual void GenerateBitmapEffect(CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut);

#ifdef RENDEROPENGL
			virtual void GenerateBitmapOpenCLEffect(CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut);
			virtual void GenerateTexture(CImageLoadingFormat * nextPicture);
			virtual GLTexture * GetTexture(const int &numTexture);
			virtual void DeleteTexture();
#endif

		protected:
			virtual CRegardsBitmap * GenerateBitmapTexture(CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut);
			Regards::OpenGL::GLTexture * pictureNext;
			CRegardsBitmap * bitmapOutCopy;
			CRegardsBitmap * bitmapTemp;		
			int width;
			int height;
		};
	}
}

