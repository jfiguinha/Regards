#pragma once
#include "ThumbnailHorizontal.h"
#include <ImageVideoThumbnail.h>


namespace Regards::Scanner
{
	class CThumbnailMultiPage : public Control::CThumbnailHorizontal
	{
	public:
		CThumbnailMultiPage(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
		                    const bool& testValidity);
		~CThumbnailMultiPage(void) = default;
		void SetFile(const wxString& filename, std::vector<std::unique_ptr<CImageVideoThumbnail>>& videoThumbnail);
		void SetVideoPosition(const int64_t& videoPos);

		int GetHeight() override
		{
			return GetIconeHeight();
		}

	private:
		void OnPictureClick(const int &numPhotoId) override;
		int FindNumItem(const int& videoPos);
		void InitWithDefaultPicture(const wxString& filename, std::vector<std::unique_ptr<CImageVideoThumbnail>>& videoThumbnail);
		int numItemSelected;
		bool process_end;
		//wxTimer * processTimer;
		wxString videoFilename;
		wxWindow* parent;
	};
}

#pragma once
