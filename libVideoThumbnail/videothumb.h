#pragma once

class CImageVideoThumbnail;
class CRegardsBitmap;
class CxImage;
class CThumbnailVideoPimpl;

namespace Regards{
	namespace Video{
		class CThumbnailVideo
		{
		public:
			CThumbnailVideo(const wxString& fileName);
			~CThumbnailVideo();

			CRegardsBitmap * GetVideoFramePos(const int64& timePosition, const int & thumbnailWidth, const int & thumbnailHeight);
			CRegardsBitmap * GetVideoFramePercent(const int& percent, const int & thumbnailWidth, const int & thumbnailHeight);
			void GetVideoDimensions(int & width, int & height);
			int GetOrientation();
			static vector<CImageVideoThumbnail *> GetVideoListFrame(const wxString & fileName,const int &widthThumbnail,const int &heightThumbnail);
			int64_t GetMovieDuration();

		private:
			CThumbnailVideoPimpl* pimpl;
			wxString fileName;
		};
	}
}


