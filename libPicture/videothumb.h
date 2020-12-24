#pragma once

class CImageVideoThumbnail;
class CRegardsBitmap;
class CFFmpegDecodeFrame;

namespace Regards{
	namespace Video{
		class CThumbnailVideo
		{
		public:
			CThumbnailVideo(const wxString & fileName);
			~CThumbnailVideo();
			bool IsHardwareDecoderCompatible();
			CRegardsBitmap * GetVideoFrame(const int & thumbnailWidth, const int & thumbnailHeight, int &rotation, int percent);
			CRegardsBitmap * GetVideoFrame(const int & thumbnailWidth, const int & thumbnailHeight, int &rotation);
			void GetVideoDimensions(int & width, int & height, int &rotation);
			CRegardsBitmap * GetVideoFrame(int &rotation, const int &percent, int &timePosition, const int & thumbnailWidth, const int & thumbnailHeight);
			vector<CImageVideoThumbnail *> GetVideoListFrame(const int &widthThumbnail,const int &heightThumbnail, const bool &compressJpeg);
			int64_t GetMovieDuration();

		private:
			wxString filename;
			CFFmpegDecodeFrame * decodeFrame;
		};
	}
}


