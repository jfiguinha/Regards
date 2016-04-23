#pragma once
#include "ThumbnailHorizontal.h"

namespace Regards
{
	namespace Control
	{
		class CThumbnailVideo : public CThumbnailHorizontal
		{
		public:
			CThumbnailVideo(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail);
			virtual ~CThumbnailVideo(void);

			void SetVideoFile(const wxString &videoFile);
			void SetVideoPosition(const int64_t &videoPos);

		private:
			int FindNumItem(const int &videoPos);

			int numItemSelected = -1;
		};
	}
}

