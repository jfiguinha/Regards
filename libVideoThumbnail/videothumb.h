#pragma once

class CImageVideoThumbnail;
class CxImage;

namespace Regards{
	namespace Video{
		class CThumbnailVideo
		{
		public:
			CThumbnailVideo();
			~CThumbnailVideo();

			cv::Mat GetVideoFrame(const wxString & fileName, const int & thumbnailWidth, const int & thumbnailHeight, int &rotation, int percent);
			cv::Mat GetVideoFrame(const wxString & fileName, const int & thumbnailWidth, const int & thumbnailHeight, int &rotation);
			void GetVideoDimensions(const wxString & fileName, int & width, int & height, int &rotation);
			cv::Mat GetVideoFrame(const wxString & fileName, int &rotation, const int &percent, int &timePosition, const int & thumbnailWidth, const int & thumbnailHeight);
			vector<CImageVideoThumbnail *> GetVideoListFrame(const wxString & fileName,const int &widthThumbnail,const int &heightThumbnail, const bool &compressJpeg);
			int64_t GetMovieDuration(const wxString & fileName);
		};
	}
}

