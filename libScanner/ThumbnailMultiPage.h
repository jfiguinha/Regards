#pragma once
#include "ThumbnailHorizontal.h"
#include <ImageVideoThumbnail.h>


namespace Regards
{
	namespace Scanner
	{
		class CThumbnailMultiPage : public Regards::Control::CThumbnailHorizontal
		{
		public:
			CThumbnailMultiPage(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailMultiPage(void);
			void SetFile(const wxString &filename, vector<CImageVideoThumbnail *> & videoThumbnail);
			void SetVideoPosition(const int64_t &videoPos);
			int GetHeight()
			{
				return GetIconeHeight();
			}
		private:
			void OnPictureClick(CThumbnailData * data);
			int FindNumItem(const int &videoPos);
			void InitWithDefaultPicture(const wxString &filename, vector<CImageVideoThumbnail *> & videoThumbnail);
			int numItemSelected;
			bool process_end;
			//wxTimer * processTimer;
			wxString videoFilename;
			wxWindow * parent;
		};
	}
}

#pragma once
