#pragma once
#include <SqlExecuteRequest.h>

class CImageVideoThumbnail;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlThumbnailVideo : public CSqlExecuteRequest
		{
		public:
			CSqlThumbnailVideo();
			~CSqlThumbnailVideo() = default;
			int GetNbThumbnail(const wxString& path);
			wxString InsertThumbnail(int photoId, const wxString& path, const int& width, const int& height,
			                     const int& numPicture, const int& rotation, const int& percent,
			                     const int& timePosition);
			cv::Mat GetThumbnail(int photoId, const wxString& path, const int& numVideo, bool& isDefault);
			void GetPictureThumbnail(int photoId, const wxString& path, const int& numVideo, CImageVideoThumbnail* videoThumbnail);

			bool DeleteThumbnail(const int& numPhoto);
			bool EraseThumbnail();
			bool EraseFolderThumbnail(const int& numFolder);
			bool TestThumbnail(const int& numPhoto, const int& numVideo);

		private:
			int TraitementResult(CSqlResult* sqlResult) override;
			CImageVideoThumbnail* videoThumbnail = nullptr;
			vector<int> listPhoto;
			vector<pair<int, int>> listVideo;
			int type;
			int nbElement;
		};
	}
}
