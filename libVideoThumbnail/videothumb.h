#pragma once

class CImageVideoThumbnail;
class CRegardsBitmap;
class CxImage;

namespace Regards{
	namespace Video{
		class CThumbnailVideo
		{
		public:
			CThumbnailVideo();
			~CThumbnailVideo();

			CRegardsBitmap * GetVideoFrame(const wxString & fileName, const int & thumbnailWidth, const int & thumbnailHeight, int &rotation);
			void GetVideoDimensions(const wxString & fileName, int & width, int & height, int &rotation);
			CRegardsBitmap * GetVideoFrame(const wxString & fileName, int &rotation, const int &percent, int &timePosition, const int & thumbnailWidth, const int & thumbnailHeight);
			vector<CImageVideoThumbnail *> GetVideoListFrame(const wxString & fileName,const int &widthThumbnail,const int &heightThumbnail);
			int64_t GetMovieDuration(const wxString & fileName);
		};
	}
}


