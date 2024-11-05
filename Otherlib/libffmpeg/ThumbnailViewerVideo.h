#pragma once
#include <ThumbnailVideo.h>
using namespace Regards::Control;

namespace Regards::VideoFFmpeg
{
	class CThumbnailViewerVideo : public Regards::Control::CThumbnailVideo
	{
	public:
		CThumbnailViewerVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
		                      const bool& testValidity);
		~CThumbnailViewerVideo(void) override;

		wxString GetFilename()
		{
			return videoFilename;
		}


		int GetHeight() override
		{
			return GetIconeHeight();
		}

	protected:
		void OnPictureClick(CThumbnailData* data) override;
		void InitWithDefaultPicture(const wxString& szFileName, const int& size) override;
	};
}
