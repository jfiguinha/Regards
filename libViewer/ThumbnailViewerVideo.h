#pragma once
#include <ThumbnailVideo.h>
using namespace Regards::Control;

namespace Regards::Viewer
{
	class CThumbnailViewerVideo : public Regards::Control::CThumbnailVideo
	{
	public:
		CThumbnailViewerVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
		                      const bool& testValidity);
		~CThumbnailViewerVideo(void) = default;

		wxString GetFilename()
		{
			return videoFilename;
		}


		int GetHeight() override
		{
			return GetIconeHeight();
		}

	private:

		void OnScrollBarH(wxCommandEvent& event);
		void OnPictureClick(const int& numPhotoId) override;
		
	};
}
