#pragma once
#include "ThumbnailHorizontal.h"
#include <ImageVideoThumbnail.h>

namespace Regards
{
	namespace Control
	{
		class CThumbnailVideo : public CThumbnailHorizontal
		{
		public:
			CThumbnailVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailVideo(void);
			void SetFile(const wxString &videoFile, const int &size = 20);
			void SetVideoPosition(const int64_t &videoPos);

		private:

			void OnTimerProcess(wxTimerEvent& event);
			virtual void ProcessThumbnailIdle();
			int FindNumItem(const int &videoPos);
			void InitWithDefaultPicture(const wxString & szFileName, const int &size = 20);
			int numItemSelected;
			bool process_end;
			wxTimer * processTimer;
			wxString videoFilename;
		};
	}
}

