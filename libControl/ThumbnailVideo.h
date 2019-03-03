#pragma once
#include "ThumbnailHorizontal.h"
#include <ImageVideoThumbnail.h>

class CThreadVideoData
{
public:

	CThreadVideoData()
	{
		myThread = nullptr;
	}

	~CThreadVideoData()
	{
	}

	vector<CImageVideoThumbnail *> videoThumbnail;
	wxString videoFilename;
    thread * myThread;
	wxWindow * window;
};


namespace Regards
{
	namespace Control
	{
		class CThumbnailVideo : public CThumbnailHorizontal
		{
		public:
			CThumbnailVideo(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailVideo(void);

			void SetVideoThumbnail(const wxString &videoFile, vector<CImageVideoThumbnail *> * pictureThumbnail);
			void SetVideoFile(const wxString &videoFile);
			void SetVideoPosition(const int64_t &videoPos);

		private:
			static void LoadVideoThumbnail(CThreadVideoData * videoData);
			void UpdateVideoThumbnail(wxCommandEvent& event);
			int FindNumItem(const int &videoPos);
			void InitWithDefaultPicture(const wxString & szFileName);
			void StartThreadVideoLoading(const wxString &videoFile);
			int numItemSelected;
			wxString videoFilename;
			bool isThreadStart;
		};
	}
}

